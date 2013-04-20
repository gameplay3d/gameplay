#include "Base.h"
#include "Properties.h"
#include "FileSystem.h"
#include "Quaternion.h"

namespace gameplay
{

/**
 * Reads the next character from the stream. Returns EOF if the end of the stream is reached.
 */
static signed char readChar(Stream* stream)
{
    if (stream->eof())
        return EOF;
    signed char c;
    if (stream->read(&c, 1, 1) != 1)
        return EOF;
    return c;
}

// Utility functions (shared with SceneLoader).
/** @script{ignore} */
void calculateNamespacePath(const std::string& urlString, std::string& fileString, std::vector<std::string>& namespacePath);
/** @script{ignore} */
Properties* getPropertiesFromNamespacePath(Properties* properties, const std::vector<std::string>& namespacePath);

Properties::Properties()
    : _dirPath(NULL), _parent(NULL)
{
}

Properties::Properties(const Properties& copy)
    : _namespace(copy._namespace), _id(copy._id), _parentID(copy._parentID), _properties(copy._properties), _dirPath(NULL), _parent(copy._parent)
{
    setDirectoryPath(copy._dirPath);
    _namespaces = std::vector<Properties*>();
    std::vector<Properties*>::const_iterator it;
    for (it = copy._namespaces.begin(); it < copy._namespaces.end(); ++it)
    {
        GP_ASSERT(*it);
        _namespaces.push_back(new Properties(**it));
    }
    rewind();
}


Properties::Properties(Stream* stream)
    : _dirPath(NULL), _parent(NULL)
{
    readProperties(stream);
    rewind();
}

Properties::Properties(Stream* stream, const char* name, const char* id, const char* parentID, Properties* parent)
    : _namespace(name), _dirPath(NULL), _parent(parent)
{
    if (id)
    {
        _id = id;
    }
    if (parentID)
    {
        _parentID = parentID;
    }
    readProperties(stream);
    rewind();
}

Properties* Properties::create(const char* url)
{
    if (!url || strlen(url) == 0)
    {
        GP_ERROR("Attempting to create a Properties object from an empty URL!");
        return NULL;
    }

    // Calculate the file and full namespace path from the specified url.
    std::string urlString = url;
    std::string fileString;
    std::vector<std::string> namespacePath;
    calculateNamespacePath(urlString, fileString, namespacePath);

    std::auto_ptr<Stream> stream(FileSystem::open(fileString.c_str()));
    if (stream.get() == NULL)
    {
        GP_ERROR("Failed to open file '%s'.", fileString.c_str());
        return NULL;
    }

    Properties* properties = new Properties(stream.get());
    properties->resolveInheritance();
    stream->close();

    // Get the specified properties object.
    Properties* p = getPropertiesFromNamespacePath(properties, namespacePath);
    if (!p)
    {
        GP_ERROR("Failed to load properties from url '%s'.", url);
        return NULL;
    }

    // If the loaded properties object is not the root namespace,
    // then we have to clone it and delete the root namespace
    // so that we don't leak memory.
    if (p != properties)
    {
        p = p->clone();
        SAFE_DELETE(properties);
    }
    p->setDirectoryPath(FileSystem::getDirectoryName(fileString.c_str()));
    return p;
}

void Properties::readProperties(Stream* stream)
{
    GP_ASSERT(stream);

    char line[2048];
    int c;
    char* name;
    char* value;
    char* parentID;
    char* rc;
    char* rcc;
    char* rccc;

    while (true)
    {
        skipWhiteSpace(stream);

        // Stop when we have reached the end of the file.
        if (stream->eof())
            break;

        // Read the next line.
        rc = stream->readLine(line, 2048);
        if (rc == NULL)
        {
            GP_ERROR("Error reading line from file.");
            return;
        }

        // Ignore comment, skip line.
        if (strncmp(line, "//", 2) != 0)
        {
            // If an '=' appears on this line, parse it as a name/value pair.
            // Note: strchr() has to be called before strtok(), or a backup of line has to be kept.
            rc = strchr(line, '=');
            if (rc != NULL)
            {
                // There could be a '}' at the end of the line, ending a namespace.
                rc = strchr(line, '}');

                // First token should be the property name.
                name = strtok(line, "=");
                if (name == NULL)
                {
                    GP_ERROR("Error parsing properties file: attribute without name.");
                    return;
                }

                // Remove white-space from name.
                name = trimWhiteSpace(name);

                // Scan for next token, the property's value.
                value = strtok(NULL, "=");
                if (value == NULL)
                {
                    GP_ERROR("Error parsing properties file: attribute with name ('%s') but no value.", name);
                    return;
                }

                // Remove white-space from value.
                value = trimWhiteSpace(value);

                // Store name/value pair.
                _properties[name] = value;

                if (rc != NULL)
                {
                    // End of namespace.
                    return;
                }
            }
            else
            {
                parentID = NULL;

                // Get the last character on the line (ignoring whitespace).
                const char* lineEnd = trimWhiteSpace(line) + (strlen(trimWhiteSpace(line)) - 1);

                // This line might begin or end a namespace,
                // or it might be a key/value pair without '='.

                // Check for '{' on same line.
                rc = strchr(line, '{');

                // Check for inheritance: ':'
                rcc = strchr(line, ':');

                // Check for '}' on same line.
                rccc = strchr(line, '}');
            
                // Get the name of the namespace.
                name = strtok(line, " \t\n{");
                name = trimWhiteSpace(name);
                if (name == NULL)
                {
                    GP_ERROR("Error parsing properties file: failed to determine a valid token for line '%s'.", line);
                    return;
                }
                else if (name[0] == '}')
                {
                    // End of namespace.
                    return;
                }

                // Get its ID if it has one.
                value = strtok(NULL, ":{");
                value = trimWhiteSpace(value);

                // Get its parent ID if it has one.
                if (rcc != NULL)
                {
                    parentID = strtok(NULL, "{");
                    parentID = trimWhiteSpace(parentID);
                }

                if (value != NULL && value[0] == '{')
                {
                    // If the namespace ends on this line, seek back to right before the '}' character.
                    if (rccc && rccc == lineEnd)
                    {
                        if (stream->seek(-1, SEEK_CUR) == false)
                        {
                            GP_ERROR("Failed to seek back to before a '}' character in properties file.");
                            return;
                        }
                        while (readChar(stream) != '}')
                        {
                            if (stream->seek(-2, SEEK_CUR) == false)
                            {
                                GP_ERROR("Failed to seek back to before a '}' character in properties file.");
                                return;
                            }
                        }
                        if (stream->seek(-1, SEEK_CUR) == false)
                        {
                            GP_ERROR("Failed to seek back to before a '}' character in properties file.");
                            return;
                        }
                    }

                    // New namespace without an ID.
                    Properties* space = new Properties(stream, name, NULL, parentID, this);
                    _namespaces.push_back(space);

                    // If the namespace ends on this line, seek to right after the '}' character.
                    if (rccc && rccc == lineEnd)
                    {
                        if (stream->seek(1, SEEK_CUR) == false)
                        {
                            GP_ERROR("Failed to seek to immediately after a '}' character in properties file.");
                            return;
                        }
                    }
                }
                else
                {
                    // If '{' appears on the same line.
                    if (rc != NULL)
                    {
                        // If the namespace ends on this line, seek back to right before the '}' character.
                        if (rccc && rccc == lineEnd)
                        {
                            if (stream->seek(-1, SEEK_CUR) == false)
                            {
                                GP_ERROR("Failed to seek back to before a '}' character in properties file.");
                                return;
                            }
                            while (readChar(stream) != '}')
                            {
                                if (stream->seek(-2, SEEK_CUR) == false)
                                {
                                    GP_ERROR("Failed to seek back to before a '}' character in properties file.");
                                    return;
                                }
                            }
                            if (stream->seek(-1, SEEK_CUR) == false)
                            {
                                GP_ERROR("Failed to seek back to before a '}' character in properties file.");
                                return;
                            }
                        }

                        // Create new namespace.
                        Properties* space = new Properties(stream, name, value, parentID, this);
                        _namespaces.push_back(space);

                        // If the namespace ends on this line, seek to right after the '}' character.
                        if (rccc && rccc == lineEnd)
                        {
                            if (stream->seek(1, SEEK_CUR) == false)
                            {
                                GP_ERROR("Failed to seek to immediately after a '}' character in properties file.");
                                return;
                            }
                        }
                    }
                    else
                    {
                        // Find out if the next line starts with "{"
                        skipWhiteSpace(stream);
                        c = readChar(stream);
                        if (c == '{')
                        {
                            // Create new namespace.
                            Properties* space = new Properties(stream, name, value, parentID, this);
                            _namespaces.push_back(space);
                        }
                        else
                        {
                            // Back up from fgetc()
                            if (stream->seek(-1, SEEK_CUR) == false)
                                GP_ERROR("Failed to seek backwards a single character after testing if the next line starts with '{'.");

                            // Store "name value" as a name/value pair, or even just "name".
                            if (value != NULL)
                            {
                                _properties[name] = value;
                            }
                            else
                            {
                                _properties[name] = std::string();
                            }
                        }
                    }
                }
            }
        }
    }
}

Properties::~Properties()
{
    SAFE_DELETE(_dirPath);
    for (size_t i = 0, count = _namespaces.size(); i < count; ++i)
    {
        SAFE_DELETE(_namespaces[i]);
    }
}

void Properties::skipWhiteSpace(Stream* stream)
{
    signed char c;
    do
    {
        c = readChar(stream);
    } while (isspace(c) && c != EOF);

    // If we are not at the end of the file, then since we found a
    // non-whitespace character, we put the cursor back in front of it.
    if (c != EOF)
    {
        if (stream->seek(-1, SEEK_CUR) == false)
        {
            GP_ERROR("Failed to seek backwards one character after skipping whitespace.");
        }
    }
}

char* Properties::trimWhiteSpace(char *str)
{
    if (str == NULL)
    {
        return str;
    }

    char *end;

    // Trim leading space.
    while (isspace(*str))
        str++;

    // All spaces?
    if (*str == 0)  
    {
        return str;
    }

    // Trim trailing space.
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;

    // Write new null terminator.
    *(end+1) = 0;

    return str;
}

void Properties::resolveInheritance(const char* id)
{
    // Namespaces can be defined like so:
    // "name id : parentID { }"
    // This method merges data from the parent namespace into the child.

    // Get a top-level namespace.
    Properties* derived;
    if (id)
    {
        derived = getNamespace(id);
    }
    else
    {
        derived = getNextNamespace();
    }
    while (derived)
    {
        // If the namespace has a parent ID, find the parent.
        if (!derived->_parentID.empty())
        {
            Properties* parent = getNamespace(derived->_parentID.c_str());
            if (parent)
            {
                resolveInheritance(parent->getId());

                // Copy the child.
                Properties* overrides = new Properties(*derived);

                // Delete the child's data.
                for (size_t i = 0, count = derived->_namespaces.size(); i < count; i++)
                {
                    SAFE_DELETE(derived->_namespaces[i]);
                }

                // Copy data from the parent into the child.
                derived->_properties = parent->_properties;
                derived->_namespaces = std::vector<Properties*>();
                std::vector<Properties*>::const_iterator itt;
                for (itt = parent->_namespaces.begin(); itt < parent->_namespaces.end(); ++itt)
                {
                    GP_ASSERT(*itt);
                    derived->_namespaces.push_back(new Properties(**itt));
                }
                derived->rewind();

                // Take the original copy of the child and override the data copied from the parent.
                derived->mergeWith(overrides);

                // Delete the child copy.
                SAFE_DELETE(overrides);
            }
        }

        // Resolve inheritance within this namespace.
        derived->resolveInheritance();

        // Get the next top-level namespace and check again.
        if (!id)
        {
            derived = getNextNamespace();
        }
        else
        {
            derived = NULL;
        }
    }
}

void Properties::mergeWith(Properties* overrides)
{
    GP_ASSERT(overrides);

    // Overwrite or add each property found in child.
    char* value = new char[255];
    overrides->rewind();
    const char* name = overrides->getNextProperty(&value);
    while (name)
    {
        this->_properties[name] = value;
        name = overrides->getNextProperty(&value);
    }
    SAFE_DELETE_ARRAY(value);
    this->_propertiesItr = this->_properties.end();

    // Merge all common nested namespaces, add new ones.
    Properties* overridesNamespace = overrides->getNextNamespace();
    while (overridesNamespace)
    {
        bool merged = false;

        rewind();
        Properties* derivedNamespace = getNextNamespace();
        while (derivedNamespace)
        {
            if (strcmp(derivedNamespace->getNamespace(), overridesNamespace->getNamespace()) == 0 &&
                strcmp(derivedNamespace->getId(), overridesNamespace->getId()) == 0)
            {   
                derivedNamespace->mergeWith(overridesNamespace);
                merged = true;
            }

            derivedNamespace = getNextNamespace();
        }

        if (!merged)
        {
            // Add this new namespace.
            Properties* newNamespace = new Properties(*overridesNamespace);

            this->_namespaces.push_back(newNamespace);
            this->_namespacesItr = this->_namespaces.end();
        }

        overridesNamespace = overrides->getNextNamespace();
    }
}

const char* Properties::getNextProperty(char** value)
{
    if (_propertiesItr == _properties.end())
    {
        // Restart from the beginning
        _propertiesItr = _properties.begin();
    }
    else
    {
        // Move to the next property
        ++_propertiesItr;
    }

    if (_propertiesItr != _properties.end())
    {
        const std::string& name = _propertiesItr->first;
        if (!name.empty())
        {
            if (value)
            {
                strcpy(*value, _propertiesItr->second.c_str());
            }
            return name.c_str();
        }
    }

    return NULL;
}

Properties* Properties::getNextNamespace()
{
    if (_namespacesItr == _namespaces.end())
    {
        // Restart from the beginning
        _namespacesItr = _namespaces.begin();
    }
    else
    {
        ++_namespacesItr;
    }

    if (_namespacesItr != _namespaces.end())
    {
        Properties* ns = *_namespacesItr;
        return ns;
    }

    return NULL;
}

void Properties::rewind()
{
    _propertiesItr = _properties.end();
    _namespacesItr = _namespaces.end();
}

Properties* Properties::getNamespace(const char* id, bool searchNames) const
{
    GP_ASSERT(id);

    Properties* ret = NULL;
    std::vector<Properties*>::const_iterator it;
    
    for (it = _namespaces.begin(); it < _namespaces.end(); ++it)
    {
        ret = *it;
        if (strcmp(searchNames ? ret->_namespace.c_str() : ret->_id.c_str(), id) == 0)
        {
            return ret;
        }
        
        // Search recursively.
        ret = ret->getNamespace(id, searchNames);
        if (ret != NULL)
        {
            return ret;
        }
    }

    return ret;
}

const char* Properties::getNamespace() const
{
    return _namespace.c_str();
}

const char* Properties::getId() const
{
    return _id.c_str();
}

bool Properties::exists(const char* name) const
{
    GP_ASSERT(name);
    return _properties.find(name) != _properties.end();
}

static const bool isStringNumeric(const char* str)
{
    GP_ASSERT(str);

    // The first character may be '-'
    if (*str == '-')
        str++;

    // The first character after the sign must be a digit
    if (!isdigit(*str))
        return false;
    str++;

    // All remaining characters must be digits, with a single decimal (.) permitted
    unsigned int decimalCount = 0;
    while (*str)
    {
        if (!isdigit(*str))
        {
            if (*str == '.' && decimalCount == 0)
            {
                // Max of 1 decimal allowed
                decimalCount++;
            }
            else
            {
                return false;
            }
        }
        str++;
    }
    return true;
}

Properties::Type Properties::getType(const char* name) const
{
    const char* value = getString(name);
    if (!value)
    {
        return Properties::NONE;
    }

    // Parse the value to determine the format
    unsigned int commaCount = 0;
    char* valuePtr = const_cast<char*>(value);
    while (valuePtr = strchr(valuePtr, ','))
    {
        valuePtr++;
        commaCount++;
    }

    switch (commaCount)
    {
    case 0:
        return isStringNumeric(value) ? Properties::NUMBER : Properties::STRING;
    case 1:
        return Properties::VECTOR2;
    case 2:
        return Properties::VECTOR3;
    case 3:
        return Properties::VECTOR4;
    case 15:
        return Properties::MATRIX;
    default:
        return Properties::STRING;
    }
}

const char* Properties::getString(const char* name) const
{
    if (name)
    {
        std::map<std::string, std::string>::const_iterator itr = _properties.find(name);
        if (itr != _properties.end())
        {
            return itr->second.c_str();
        }
    }
    else
    {
        if (_propertiesItr != _properties.end())
        {
            return _propertiesItr->second.c_str();
        }
    }

    return NULL;
}

bool Properties::getBool(const char* name, bool defaultValue) const
{
    const char* valueString = getString(name);
    if (valueString)
    {
        return (strcmp(valueString, "true") == 0);
    }

    return defaultValue;
}

int Properties::getInt(const char* name) const
{
    const char* valueString = getString(name);
    if (valueString)
    {
        int value;
        int scanned;
        scanned = sscanf(valueString, "%d", &value);
        if (scanned != 1)
        {
            GP_ERROR("Error attempting to parse property '%s' as an integer.", name);
            return 0;
        }
        return value;
    }

    return 0;
}

float Properties::getFloat(const char* name) const
{
    const char* valueString = getString(name);
    if (valueString)
    {
        float value;
        int scanned;
        scanned = sscanf(valueString, "%f", &value);
        if (scanned != 1)
        {
            GP_ERROR("Error attempting to parse property '%s' as a float.", name);
            return 0.0f;
        }
        return value;
    }

    return 0.0f;
}

long Properties::getLong(const char* name) const
{
    const char* valueString = getString(name);
    if (valueString)
    {
        long value;
        int scanned;
        scanned = sscanf(valueString, "%ld", &value);
        if (scanned != 1)
        {
            GP_ERROR("Error attempting to parse property '%s' as a long integer.", name);
            return 0L;
        }
        return value;
    }

    return 0L;
}

bool Properties::getMatrix(const char* name, Matrix* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float m[16];
        int scanned;
        scanned = sscanf(valueString, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
                &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7],
                &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15]);

        if (scanned != 16)
        {
            GP_ERROR("Error attempting to parse property '%s' as a matrix.", name);
            out->setIdentity();
            return false;
        }

        out->set(m);
        return true;
    }

    out->setIdentity();
    return false;
}

bool Properties::getVector2(const char* name, Vector2* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y;
        int scanned;
        scanned = sscanf(valueString, "%f,%f", &x, &y);
        if (scanned != 2)
        {
            GP_ERROR("Error attempting to parse property '%s' as a two-dimensional vector.", name);
            out->set(0.0f, 0.0f);
            return false;
        }

        out->set(x, y);
        return true;
    }
    
    out->set(0.0f, 0.0f);
    return false;
}

bool Properties::getVector3(const char* name, Vector3* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y, z;
        int scanned;
        scanned = sscanf(valueString, "%f,%f,%f", &x, &y, &z);
        if (scanned != 3)
        {
            GP_ERROR("Error attempting to parse property '%s' as a three-dimensional vector.", name);
            out->set(0.0f, 0.0f, 0.0f);
            return false;
        }

        out->set(x, y, z);
        return true;
    }
    
    out->set(0.0f, 0.0f, 0.0f);
    return false;
}

bool Properties::getVector4(const char* name, Vector4* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y, z, w;
        int scanned;
        scanned = sscanf(valueString, "%f,%f,%f,%f", &x, &y, &z, &w);
        if (scanned != 4)
        {
            GP_ERROR("Error attempting to parse property '%s' as a four-dimensional vector.", name);
            out->set(0.0f, 0.0f, 0.0f, 0.0f);
            return false;
        }

        out->set(x, y, z, w);
        return true;
    }
    
    out->set(0.0f, 0.0f, 0.0f, 0.0f);
    return false;
}

bool Properties::getQuaternionFromAxisAngle(const char* name, Quaternion* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y, z, theta;
        int scanned;
        scanned = sscanf(valueString, "%f,%f,%f,%f", &x, &y, &z, &theta);
        if (scanned != 4)
        {
            GP_ERROR("Error attempting to parse property '%s' as an axis-angle rotation.", name);
            out->set(0.0f, 0.0f, 0.0f, 1.0f);
            return false;
        }

        out->set(Vector3(x, y, z), MATH_DEG_TO_RAD(theta));
        return true;
    }
    
    out->set(0.0f, 0.0f, 0.0f, 1.0f);
    return false;
}

bool Properties::getColor(const char* name, Vector3* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        if (strlen(valueString) != 7 ||
            valueString[0] != '#')
        {
            // Not a color string.
            GP_ERROR("Error attempting to parse property '%s' as an RGB color (not specified as a color string).", name);
            out->set(0.0f, 0.0f, 0.0f);
            return false;
        }

        // Read the string into an int as hex.
        unsigned int color;
        if (sscanf(valueString+1, "%x", &color) != 1)
        {
            GP_ERROR("Error attempting to parse property '%s' as an RGB color.", name);
            out->set(0.0f, 0.0f, 0.0f);
            return false;
        }

        out->set(Vector3::fromColor(color));
        return true;
    }

    out->set(0.0f, 0.0f, 0.0f);
    return false;
}

bool Properties::getColor(const char* name, Vector4* out) const
{
    GP_ASSERT(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        if (strlen(valueString) != 9 ||
            valueString[0] != '#')
        {
            // Not a color string.
            GP_ERROR("Error attempting to parse property '%s' as an RGBA color (not specified as a color string).", name);
            out->set(0.0f, 0.0f, 0.0f, 0.0f);
            return false;
        }

        // Read the string into an int as hex.
        unsigned int color;
        if (sscanf(valueString+1, "%x", &color) != 1)
        {
            GP_ERROR("Error attempting to parse property '%s' as an RGBA color.", name);
            out->set(0.0f, 0.0f, 0.0f, 0.0f);
            return false;
        }

        out->set(Vector4::fromColor(color));
        return true;
    }

    out->set(0.0f, 0.0f, 0.0f, 0.0f);
    return false;
}

bool Properties::getPath(const char* name, std::string* path) const
{
    GP_ASSERT(name && path);
    const char* valueString = getString(name);
    if (valueString)
    {
        if (FileSystem::fileExists(valueString))
        {
            path->assign(valueString);
            return true;
        }
        else
        {
            const Properties* prop = this;
            while (prop != NULL)
            {
                // Search for the file path relative to the bundle file
                const std::string* dirPath = prop->_dirPath;
                if (dirPath != NULL && !dirPath->empty())
                {
                    std::string relativePath = *dirPath;
                    relativePath.append(valueString);
                    if (FileSystem::fileExists(relativePath.c_str()))
                    {
                        path->assign(relativePath);
                        return true;
                    }
                }
                prop = prop->_parent;
            }
        }
    }
    return false;
}

Properties* Properties::clone()
{
    Properties* p = new Properties();
    
    p->_namespace = _namespace;
    p->_id = _id;
    p->_parentID = _parentID;
    p->_properties = _properties;
    p->_propertiesItr = p->_properties.end();
    p->setDirectoryPath(_dirPath);

    for (size_t i = 0, count = _namespaces.size(); i < count; i++)
    {
        GP_ASSERT(_namespaces[i]);
        Properties* child = _namespaces[i]->clone();
        p->_namespaces.push_back(child);
        child->_parent = p;
    }
    p->_namespacesItr = p->_namespaces.end();

    return p;
}

void Properties::setDirectoryPath(const std::string* path)
{
    if (path)
    {
        setDirectoryPath(*path);
    }
    else
    {
        SAFE_DELETE(_dirPath);
    }
}

void Properties::setDirectoryPath(const std::string& path)
{
    if (_dirPath == NULL)
    {
        _dirPath = new std::string(path);
    }
    else
    {
        _dirPath->assign(path);
    }
}

void calculateNamespacePath(const std::string& urlString, std::string& fileString, std::vector<std::string>& namespacePath)
{
    // If the url references a specific namespace within the file,
    // calculate the full namespace path to the final namespace.
    size_t loc = urlString.rfind("#");
    if (loc != std::string::npos)
    {
        fileString = urlString.substr(0, loc);
        std::string namespacePathString = urlString.substr(loc + 1);
        while ((loc = namespacePathString.find("/")) != std::string::npos)
        {
            namespacePath.push_back(namespacePathString.substr(0, loc));
            namespacePathString = namespacePathString.substr(loc + 1);
        }
        namespacePath.push_back(namespacePathString);
    }
    else
    {
        fileString = urlString;
    }
}

Properties* getPropertiesFromNamespacePath(Properties* properties, const std::vector<std::string>& namespacePath)
{
    // If the url references a specific namespace within the file,
    // return the specified namespace or notify the user if it cannot be found.
    if (namespacePath.size() > 0)
    {
        size_t size = namespacePath.size();
        properties->rewind();
        Properties* iter = properties->getNextNamespace();
        for (size_t i = 0; i < size; )
        {
            while (true)
            {
                if (iter == NULL)
                {
                    GP_ERROR("Failed to load properties object from url.");
                    return NULL;
                }

                if (strcmp(iter->getId(), namespacePath[i].c_str()) == 0)
                {
                    if (i != size - 1)
                    {
                        properties = iter->getNextNamespace();
                        iter = properties;
                    }
                    else
                        properties = iter;

                    i++;
                    break;
                }
                
                iter = properties->getNextNamespace();
            }
        }

        return properties;
    }
    else
        return properties;
}

}
