#include "PhysicsDebugDrawer.h"

namespace gameplay
{

// The initial capacity of the debug drawer's vertex batch.
#define INITIAL_CAPACITY 280

PhysicsDebugDrawer::PhysicsDebugDrawer()
    : _mode(btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawConstraintLimits | btIDebugDraw::DBG_DrawConstraints | 
    btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawWireframe), _program(0), _positionAttrib(0),
    _colorAttrib(0), _viewProjectionMatrixUniform(0), _viewProjection(NULL), _vertexData(NULL), _vertexCount(0), _vertexDataSize(0)
{
    // Unused
}

PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
    SAFE_DELETE_ARRAY(_vertexData);
}

void PhysicsDebugDrawer::begin(const Matrix& viewProjection)
{
    _viewProjection = &viewProjection;
    _vertexCount = 0;
}

void PhysicsDebugDrawer::end()
{
    // Lazy load the shader program for drawing.
    if (!_program)
    {
        // Vertex shader for drawing colored lines.
        const char* vs_str = {
            "uniform mat4 u_viewProjectionMatrix;\n"
            "attribute vec4 a_position;\n"
            "attribute vec4 a_color;\n"
            "varying vec4 v_color;\n"
            "void main(void) {\n"
            "    v_color = a_color;\n"
            "    gl_Position = u_viewProjectionMatrix * a_position;\n"
            "}"
        };

        // Fragment shader for drawing colored lines.
        const char* fs_str = {
#ifdef OPENGL_ES
            "precision highp float;\n"
#endif
            "varying vec4 v_color;\n"
            "void main(void) {\n"
            "   gl_FragColor = v_color;"
            "}"
        };

        // Load the vertex shader.
        GLuint vs;
        GL_ASSERT( vs = glCreateShader(GL_VERTEX_SHADER) );
        GLint shader_str_len = strlen(vs_str);
        GL_ASSERT( glShaderSource(vs, 1, &vs_str, &shader_str_len) );
        GL_ASSERT( glCompileShader(vs) );
        GLint status;
        GL_ASSERT( glGetShaderiv(vs, GL_COMPILE_STATUS, &status) );
        if (status == GL_FALSE)
        {
            GLchar errorMessage[512];
            GL_ASSERT( glGetShaderInfoLog(vs, sizeof(errorMessage), 0, errorMessage) );
            WARN_VARG("Physics debug drawing will not work; vertex shader failed to compile with error: '%s'", errorMessage);
            return;
        }

        // Load the fragment shader.
        GLuint fs;
        GL_ASSERT( fs = glCreateShader(GL_FRAGMENT_SHADER) );
        shader_str_len = strlen(fs_str);
        GL_ASSERT( glShaderSource(fs, 1, &fs_str, &shader_str_len) );
        GL_ASSERT( glCompileShader(fs) );
        GL_ASSERT( glGetShaderiv(fs, GL_COMPILE_STATUS, &status) );
        if (status == GL_FALSE)
        {
            GLchar errorMessage[512];
            GL_ASSERT( glGetShaderInfoLog(fs, sizeof(errorMessage), 0, errorMessage) );
            WARN_VARG("Physics debug drawing will not work; fragment shader failed to compile with error: '%s'", errorMessage);
            return;
        }

        // Create the shader program and link it.
        GL_ASSERT( _program = glCreateProgram() );
        GL_ASSERT( glAttachShader(_program, vs) );
        GL_ASSERT( glAttachShader(_program, fs) );
        GL_ASSERT( glLinkProgram(_program) );
        GL_ASSERT( glGetProgramiv(_program, GL_LINK_STATUS, &status) );
        if (status == GL_FALSE)
        {
            GLchar errorMessage[512];
            GL_ASSERT( glGetProgramInfoLog(_program, sizeof(errorMessage), 0, errorMessage) );
            WARN_VARG("Physics debug drawing will not work; shader program failed to link with error: '%s'", errorMessage);
            return;
        }

        // Get the attribute and uniform locations.
        GL_ASSERT( glUseProgram(_program) );
        GL_ASSERT( _positionAttrib = glGetAttribLocation(_program, "a_position") );
        GL_ASSERT( _colorAttrib = glGetAttribLocation(_program, "a_color") );
        GL_ASSERT( _viewProjectionMatrixUniform = glGetUniformLocation(_program, "u_viewProjectionMatrix") );
    }

    // Set the shader program and vertex attributes.
    GL_ASSERT( glUseProgram(_program) );
    GL_ASSERT( glEnableVertexAttribArray(_positionAttrib) );
    GL_ASSERT( glEnableVertexAttribArray(_colorAttrib) );
    GL_ASSERT( glVertexAttribPointer(_positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, _vertexData) );
    GL_ASSERT( glVertexAttribPointer(_colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, &_vertexData[3]) );
    
    // Set the camera's view projection matrix and draw.
    GL_ASSERT( glUniformMatrix4fv(_viewProjectionMatrixUniform, 1, GL_FALSE, _viewProjection->m) );
    GL_ASSERT( glDrawArrays(GL_LINES, 0, _vertexCount / 7) );

    // Reset shader state.
    GL_ASSERT( glDisableVertexAttribArray(_positionAttrib) );
    GL_ASSERT( glDisableVertexAttribArray(_colorAttrib) );
    GL_ASSERT( glUseProgram(0) );
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
    // Allocate extra space in the vertex data batch if it is needed.
    if (_vertexDataSize - _vertexCount < 14)
    {
        if (_vertexDataSize > 0)
        {
            unsigned int newVertexDataSize = _vertexDataSize * 2;
            float* newVertexData = new float[newVertexDataSize];
            memcpy(newVertexData, _vertexData, _vertexDataSize * sizeof(float));
            SAFE_DELETE_ARRAY(_vertexData);
            _vertexData = newVertexData;
            _vertexDataSize = newVertexDataSize;
        }
        else
        {
            _vertexDataSize = INITIAL_CAPACITY;
            _vertexData = new float[_vertexDataSize];
        }
    }
    
    // Create the vertex data for the line and copy it into the batch.
    float vertexData[] = {
        from.getX(), from.getY(), from.getZ(), 
        fromColor.getX(), fromColor.getY(), fromColor.getZ(), 1.0f,
        to.getX(), to.getY(), to.getZ(),
        toColor.getX(), toColor.getY(), toColor.getZ(), 1.0f
    };
    memcpy(&_vertexData[_vertexCount], vertexData, sizeof(float) * 14);
    _vertexCount += 14;
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    drawLine(from, to, color, color);
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    drawLine(pointOnB, pointOnB + normalOnB, color);
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
    WARN(warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
    WARN("Physics debug drawing: 3D text is not supported.");
}

void PhysicsDebugDrawer::setDebugMode(int mode)
{
    _mode = mode;
}

int	PhysicsDebugDrawer::getDebugMode() const
{
    return _mode;
}

}
