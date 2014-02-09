#include "Base.h"
#include "Effect.h"
#include "FileSystem.h"
#include "Game.h"

#define OPENGL_ES_DEFINE  "OPENGL_ES"

namespace gameplay
{

// Cache of unique effects.
static std::map<std::string, Effect*> __effectCache;
static Effect* __currentEffect = NULL;

Effect::Effect() : _program(0)
{
}

Effect::~Effect()
{
    // Remove this effect from the cache.
    __effectCache.erase(_id);

    // Free uniforms.
    for (std::map<std::string, Uniform*>::iterator itr = _uniforms.begin(); itr != _uniforms.end(); ++itr)
    {
        SAFE_DELETE(itr->second);
    }

    if (_program)
    {
        // If our program object is currently bound, unbind it before we're destroyed.
        if (__currentEffect == this)
        {
            GL_ASSERT( glUseProgram(0) );
            __currentEffect = NULL;
        }

        GL_ASSERT( glDeleteProgram(_program) );
        _program = 0;
    }
}

Effect* Effect::createFromFile(const char* vshPath, const char* fshPath, const char* defines)
{
    GP_ASSERT(vshPath);
    GP_ASSERT(fshPath);

    // Search the effect cache for an identical effect that is already loaded.
    std::string uniqueId = vshPath;
    uniqueId += ';';
    uniqueId += fshPath;
    uniqueId += ';';
    if (defines)
    {
        uniqueId += defines;
    }
    std::map<std::string, Effect*>::const_iterator itr = __effectCache.find(uniqueId);
    if (itr != __effectCache.end())
    {
        // Found an exiting effect with this id, so increase its ref count and return it.
        GP_ASSERT(itr->second);
        itr->second->addRef();
        return itr->second;
    }

    // Read source from file.
    char* vshSource = FileSystem::readAll(vshPath);
    if (vshSource == NULL)
    {
        GP_ERROR("Failed to read vertex shader from file '%s'.", vshPath);
        return NULL;
    }
    char* fshSource = FileSystem::readAll(fshPath);
    if (fshSource == NULL)
    {
        GP_ERROR("Failed to read fragment shader from file '%s'.", fshPath);
        SAFE_DELETE_ARRAY(vshSource);
        return NULL;
    }

    Effect* effect = createFromSource(vshPath, vshSource, fshPath, fshSource, defines);
    
    SAFE_DELETE_ARRAY(vshSource);
    SAFE_DELETE_ARRAY(fshSource);

    if (effect == NULL)
    {
        GP_ERROR("Failed to create effect from shaders '%s', '%s'.", vshPath, fshPath);
    }
    else
    {
        // Store this effect in the cache.
        effect->_id = uniqueId;
        __effectCache[uniqueId] = effect;
    }

    return effect;
}

Effect* Effect::createFromSource(const char* vshSource, const char* fshSource, const char* defines)
{
    return createFromSource(NULL, vshSource, NULL, fshSource, defines);
}

static void replaceDefines(const char* defines, std::string& out)
{
    Properties* graphicsConfig = Game::getInstance()->getConfig()->getNamespace("graphics", true);
    const char* globalDefines = graphicsConfig ? graphicsConfig->getString("shaderDefines") : NULL;

    // Build full semicolon delimited list of defines
#ifdef OPENGL_ES
    out = OPENGL_ES_DEFINE;
#else
    out = "";
#endif
    if (globalDefines && strlen(globalDefines) > 0)
    {
        if (out.length() > 0)
            out += ';';
        out += globalDefines;
    }
    if (defines && strlen(defines) > 0)
    {
        if (out.length() > 0)
            out += ';';
        out += defines;
    }

    // Replace semicolons
    if (out.length() > 0)
    {
        size_t pos;
        out.insert(0, "#define ");
        while ((pos = out.find(';')) != std::string::npos)
        {
            out.replace(pos, 1, "\n#define ");
        }
        out += "\n";
    }
}

static void replaceIncludes(const char* filepath, const char* source, std::string& out)
{
    // Replace the #include "xxxx.xxx" with the sourced file contents of "filepath/xxxx.xxx"
    std::string str = source;
    size_t lastPos = 0;
    size_t headPos = 0;
    size_t fileLen = str.length();
    size_t tailPos = fileLen;
    while (headPos < fileLen)
    {
        lastPos = headPos;
        if (headPos == 0)
        {
            // find the first "#include"
            headPos = str.find("#include");
        }
        else
        {
            // find the next "#include"
            headPos = str.find("#include", headPos + 1);
        }

        // If "#include" is found
        if (headPos != std::string::npos)
        {
            // append from our last position for the legth (head - last position) 
            out.append(str.substr(lastPos,  headPos - lastPos));

            // find the start quote "
            size_t startQuote = str.find("\"", headPos) + 1;
            if (startQuote == std::string::npos)
            {
                // We have started an "#include" but missing the leading quote "
                GP_ERROR("Compile failed for shader '%s' missing leading \".", filepath);
                return;
            }
            // find the end quote "
            size_t endQuote = str.find("\"", startQuote);
            if (endQuote == std::string::npos)
            {
                // We have a start quote but missing the trailing quote "
                GP_ERROR("Compile failed for shader '%s' missing trailing \".", filepath);
                return;
            }

            // jump the head position past the end quote
            headPos = endQuote + 1;
            
            // File path to include and 'stitch' in the value in the quotes to the file path and source it.
            std::string filepathStr = filepath;
            std::string directoryPath = filepathStr.substr(0, filepathStr.rfind('/') + 1);
            size_t len = endQuote - (startQuote);
            std::string includeStr = str.substr(startQuote, len);
            directoryPath.append(includeStr);
            const char* includedSource = FileSystem::readAll(directoryPath.c_str());
            if (includedSource == NULL)
            {
                GP_ERROR("Compile failed for shader '%s' invalid filepath.", filepathStr.c_str());
                return;
            }
            else
            {
                // Valid file so lets attempt to see if we need to append anything to it too (recurse...)
                replaceIncludes(directoryPath.c_str(), includedSource, out);
                SAFE_DELETE_ARRAY(includedSource);
            }
        }
        else
        {
            // Append the remaining
            out.append(str.c_str(), lastPos, tailPos);
        }
    }
}

static void writeShaderToErrorFile(const char* filePath, const char* source)
{
    std::string path = filePath;
    path += ".err";
    std::auto_ptr<Stream> stream(FileSystem::open(path.c_str(), FileSystem::WRITE));
    if (stream.get() != NULL && stream->canWrite())
    {
        stream->write(source, 1, strlen(source));
    }
}

Effect* Effect::createFromSource(const char* vshPath, const char* vshSource, const char* fshPath, const char* fshSource, const char* defines)
{
    GP_ASSERT(vshSource);
    GP_ASSERT(fshSource);

    const unsigned int SHADER_SOURCE_LENGTH = 3;
    const GLchar* shaderSource[SHADER_SOURCE_LENGTH];
    char* infoLog = NULL;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    GLint length;
    GLint success;

    // Replace all comma separated definitions with #define prefix and \n suffix
    std::string definesStr = "";
    replaceDefines(defines, definesStr);
    
    shaderSource[0] = definesStr.c_str();
    shaderSource[1] = "\n";
    std::string vshSourceStr = "";
    if (vshPath)
    {
        // Replace the #include "xxxxx.xxx" with the sources that come from file paths
        replaceIncludes(vshPath, vshSource, vshSourceStr);
        if (vshSource && strlen(vshSource) != 0)
            vshSourceStr += "\n";
    }
    shaderSource[2] = vshPath ? vshSourceStr.c_str() :  vshSource;
    GL_ASSERT( vertexShader = glCreateShader(GL_VERTEX_SHADER) );
    GL_ASSERT( glShaderSource(vertexShader, SHADER_SOURCE_LENGTH, shaderSource, NULL) );
    GL_ASSERT( glCompileShader(vertexShader) );
    GL_ASSERT( glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success) );
    if (success != GL_TRUE)
    {
        GL_ASSERT( glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length) );
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT( glGetShaderInfoLog(vertexShader, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }

        // Write out the expanded shader file.
        if (vshPath)
            writeShaderToErrorFile(vshPath, shaderSource[2]);

        GP_ERROR("Compile failed for vertex shader '%s' with error '%s'.", vshPath == NULL ? vshSource : vshPath, infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT( glDeleteShader(vertexShader) );

        return NULL;
    }

    // Compile the fragment shader.
    std::string fshSourceStr;
    if (fshPath)
    {
        // Replace the #include "xxxxx.xxx" with the sources that come from file paths
        replaceIncludes(fshPath, fshSource, fshSourceStr);
        if (fshSource && strlen(fshSource) != 0)
            fshSourceStr += "\n";
    }
    shaderSource[2] = fshPath ? fshSourceStr.c_str() : fshSource;
    GL_ASSERT( fragmentShader = glCreateShader(GL_FRAGMENT_SHADER) );
    GL_ASSERT( glShaderSource(fragmentShader, SHADER_SOURCE_LENGTH, shaderSource, NULL) );
    GL_ASSERT( glCompileShader(fragmentShader) );
    GL_ASSERT( glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success) );
    if (success != GL_TRUE)
    {
        GL_ASSERT( glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length) );
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT( glGetShaderInfoLog(fragmentShader, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        
        // Write out the expanded shader file.
        if (fshPath)
            writeShaderToErrorFile(fshPath, shaderSource[2]);

        GP_ERROR("Compile failed for fragment shader (%s): %s", fshPath == NULL ? fshSource : fshPath, infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT( glDeleteShader(vertexShader) );
        GL_ASSERT( glDeleteShader(fragmentShader) );

        return NULL;
    }

    // Link program.
    GL_ASSERT( program = glCreateProgram() );
    GL_ASSERT( glAttachShader(program, vertexShader) );
    GL_ASSERT( glAttachShader(program, fragmentShader) );
    GL_ASSERT( glLinkProgram(program) );
    GL_ASSERT( glGetProgramiv(program, GL_LINK_STATUS, &success) );

    // Delete shaders after linking.
    GL_ASSERT( glDeleteShader(vertexShader) );
    GL_ASSERT( glDeleteShader(fragmentShader) );

    // Check link status.
    if (success != GL_TRUE)
    {
        GL_ASSERT( glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) );
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            GL_ASSERT( glGetProgramInfoLog(program, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        GP_ERROR("Linking program failed (%s,%s): %s", vshPath == NULL ? "NULL" : vshPath, fshPath == NULL ? "NULL" : fshPath, infoLog == NULL ? "" : infoLog);
        SAFE_DELETE_ARRAY(infoLog);

        // Clean up.
        GL_ASSERT( glDeleteProgram(program) );

        return NULL;
    }

    // Create and return the new Effect.
    Effect* effect = new Effect();
    effect->_program = program;

    // Query and store vertex attribute meta-data from the program.
    // NOTE: Rather than using glBindAttribLocation to explicitly specify our own
    // preferred attribute locations, we're going to query the locations that were
    // automatically bound by the GPU. While it can sometimes be convenient to use
    // glBindAttribLocation, some vendors actually reserve certain attribute indices
    // and therefore using this function can create compatibility issues between
    // different hardware vendors.
    GLint activeAttributes;
    GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes) );
    if (activeAttributes > 0)
    {
        GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length) );
        if (length > 0)
        {
            GLchar* attribName = new GLchar[length + 1];
            GLint attribSize;
            GLenum attribType;
            GLint attribLocation;
            for (int i = 0; i < activeAttributes; ++i)
            {
                // Query attribute info.
                GL_ASSERT( glGetActiveAttrib(program, i, length, NULL, &attribSize, &attribType, attribName) );
                attribName[length] = '\0';

                // Query the pre-assigned attribute location.
                GL_ASSERT( attribLocation = glGetAttribLocation(program, attribName) );

                // Assign the vertex attribute mapping for the effect.
                effect->_vertexAttributes[attribName] = attribLocation;
            }
            SAFE_DELETE_ARRAY(attribName);
        }
    }

    // Query and store uniforms from the program.
    GLint activeUniforms;
    GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &activeUniforms) );
    if (activeUniforms > 0)
    {
        GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length) );
        if (length > 0)
        {
            GLchar* uniformName = new GLchar[length + 1];
            GLint uniformSize;
            GLenum uniformType;
            GLint uniformLocation;
            unsigned int samplerIndex = 0;
            for (int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                GL_ASSERT( glGetActiveUniform(program, i, length, NULL, &uniformSize, &uniformType, uniformName) );
                uniformName[length] = '\0';  // null terminate
                if (length > 3)
                {
                    // If this is an array uniform, strip array indexers off it since GL does not
                    // seem to be consistent across different drivers/implementations in how it returns
                    // array uniforms. On some systems it will return "u_matrixArray", while on others
                    // it will return "u_matrixArray[0]".
                    char* c = strrchr(uniformName, '[');
                    if (c)
                    {
                        *c = '\0';
                    }
                }

                // Query the pre-assigned uniform location.
                GL_ASSERT( uniformLocation = glGetUniformLocation(program, uniformName) );

                Uniform* uniform = new Uniform();
                uniform->_effect = effect;
                uniform->_name = uniformName;
                uniform->_location = uniformLocation;
                uniform->_type = uniformType;
                if (uniformType == GL_SAMPLER_2D)
                {
                    uniform->_index = samplerIndex;
                    samplerIndex += uniformSize;
                }
                else
                {
                    uniform->_index = 0;
                }

                effect->_uniforms[uniformName] = uniform;
            }
            SAFE_DELETE_ARRAY(uniformName);
        }
    }

    return effect;
}

const char* Effect::getId() const
{
    return _id.c_str();
}

VertexAttribute Effect::getVertexAttribute(const char* name) const
{
    std::map<std::string, VertexAttribute>::const_iterator itr = _vertexAttributes.find(name);
    return (itr == _vertexAttributes.end() ? -1 : itr->second);
}

Uniform* Effect::getUniform(const char* name) const
{
    std::map<std::string, Uniform*>::const_iterator itr = _uniforms.find(name);

	if (itr != _uniforms.end()) {
		// Return cached uniform variable
		return itr->second;
	}

    GLint uniformLocation;
    GL_ASSERT( uniformLocation = glGetUniformLocation(_program, name) );
    if (uniformLocation > -1)
	{
		// Check for array uniforms ("u_directionalLightColor[0]" -> "u_directionalLightColor")
		char* parentname = new char[strlen(name)+1];
		strcpy(parentname, name);
		if (strtok(parentname, "[") != NULL) {
			std::map<std::string, Uniform*>::const_iterator itr = _uniforms.find(parentname);
			if (itr != _uniforms.end()) {
				Uniform* puniform = itr->second;

				Uniform* uniform = new Uniform();
				uniform->_effect = const_cast<Effect*>(this);
				uniform->_name = name;
				uniform->_location = uniformLocation;
				uniform->_index = 0;
				uniform->_type = puniform->getType();
				_uniforms[name] = uniform;

				delete parentname;
				return uniform;
			}
		}
		delete parentname;
    }

	// No uniform variable found - return NULL
	return NULL;
}

Uniform* Effect::getUniform(unsigned int index) const
{
    unsigned int i = 0;
    for (std::map<std::string, Uniform*>::const_iterator itr = _uniforms.begin(); itr != _uniforms.end(); ++itr, ++i)
    {
        if (i == index)
        {
            return itr->second;
        }
    }
    return NULL;
}

unsigned int Effect::getUniformCount() const
{
    return (unsigned int)_uniforms.size();
}

void Effect::setValue(Uniform* uniform, float value)
{
    GP_ASSERT(uniform);
    GL_ASSERT( glUniform1f(uniform->_location, value) );
}

void Effect::setValue(Uniform* uniform, const float* values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(values);
    GL_ASSERT( glUniform1fv(uniform->_location, count, values) );
}

void Effect::setValue(Uniform* uniform, int value)
{
    GP_ASSERT(uniform);
    GL_ASSERT( glUniform1i(uniform->_location, value) );
}

void Effect::setValue(Uniform* uniform, const int* values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(values);
    GL_ASSERT( glUniform1iv(uniform->_location, count, values) );
}

void Effect::setValue(Uniform* uniform, const Matrix& value)
{
    GP_ASSERT(uniform);
    GL_ASSERT( glUniformMatrix4fv(uniform->_location, 1, GL_FALSE, value.m) );
}

void Effect::setValue(Uniform* uniform, const Matrix* values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(values);
    GL_ASSERT( glUniformMatrix4fv(uniform->_location, count, GL_FALSE, (GLfloat*)values) );
}

void Effect::setValue(Uniform* uniform, const Vector2& value)
{
    GP_ASSERT(uniform);
    GL_ASSERT( glUniform2f(uniform->_location, value.x, value.y) );
}

void Effect::setValue(Uniform* uniform, const Vector2* values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(values);
    GL_ASSERT( glUniform2fv(uniform->_location, count, (GLfloat*)values) );
}

void Effect::setValue(Uniform* uniform, const Vector3& value)
{
    GP_ASSERT(uniform);
    GL_ASSERT( glUniform3f(uniform->_location, value.x, value.y, value.z) );
}

void Effect::setValue(Uniform* uniform, const Vector3* values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(values);
    GL_ASSERT( glUniform3fv(uniform->_location, count, (GLfloat*)values) );
}

void Effect::setValue(Uniform* uniform, const Vector4& value)
{
    GP_ASSERT(uniform);
    GL_ASSERT( glUniform4f(uniform->_location, value.x, value.y, value.z, value.w) );
}

void Effect::setValue(Uniform* uniform, const Vector4* values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(values);
    GL_ASSERT( glUniform4fv(uniform->_location, count, (GLfloat*)values) );
}

void Effect::setValue(Uniform* uniform, const Texture::Sampler* sampler)
{
    GP_ASSERT(uniform);
    GP_ASSERT(uniform->_type == GL_SAMPLER_2D);
    GP_ASSERT(sampler);

    GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index) );

    // Bind the sampler - this binds the texture and applies sampler state
    const_cast<Texture::Sampler*>(sampler)->bind();

    GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );
}

void Effect::setValue(Uniform* uniform, const Texture::Sampler** values, unsigned int count)
{
    GP_ASSERT(uniform);
    GP_ASSERT(uniform->_type == GL_SAMPLER_2D);
    GP_ASSERT(values);

    // Set samplers as active and load texture unit array
    GLint units[32];
    for (unsigned int i = 0; i < count; ++i)
    {
        GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index + i) );

        // Bind the sampler - this binds the texture and applies sampler state
        const_cast<Texture::Sampler*>(values[i])->bind();

        units[i] = uniform->_index + i;
    }

    // Pass texture unit array to GL
    GL_ASSERT( glUniform1iv(uniform->_location, count, units) );
}

void Effect::bind()
{
   GL_ASSERT( glUseProgram(_program) );

    __currentEffect = this;
}

Effect* Effect::getCurrentEffect()
{
    return __currentEffect;
}

Uniform::Uniform() :
    _location(-1), _type(0), _index(0)
{
}

Uniform::~Uniform()
{
    // hidden
}

Effect* Uniform::getEffect() const
{
    return _effect;
}

const char* Uniform::getName() const
{
    return _name.c_str();
}

const GLenum Uniform::getType() const
{
    return _type;
}

}
