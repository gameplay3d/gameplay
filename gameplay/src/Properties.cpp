/*
 * Properties.cpp
 */

#include "Base.h"
#include "Properties.h"
#include "FileSystem.h"

namespace gameplay
{

Properties::Properties(FILE* file)
{
    readProperties(file);
    _propertiesItr = _properties.end();
    _namespacesItr = _namespaces.end();
}

Properties::Properties(FILE* file, const char* name, const char* id) : _namespace(name)
{
    if (id)
    {
        _id = id;
    }
    readProperties(file);
    _propertiesItr = _properties.end();
    _namespacesItr = _namespaces.end();
}

Properties* Properties::create(const char* filePath)
{
    assert(filePath);

    FILE* file = FileSystem::openFile(filePath, "rb");
    if (!file)
    {
        return NULL;
    }

    Properties* properties = new Properties(file);

    fclose(file);

    return properties;
}

void Properties::readProperties(FILE* file)
{
    char line[2048];
    int c;
    char* name;
    char* value;
    char* rc;

    while (!feof(file))
    {
        skipWhiteSpace(file);

        // Read the next line.
        rc = fgets(line, 2048, file);
        if (rc == NULL)
        {
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
                name = strtok(line, " =\t");
                if (name == NULL)
                {
                    LOG_ERROR("Error parsing properties file: value without name.");
                    return;
                }

                // Scan for next token, the property's value.
                value = strtok(NULL, "=");
                if (value == NULL)
                {
                    LOG_ERROR("Error parsing properties file: name without value.");
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
                // This line might begin or end a namespace,
                // or it might be a key/value pair without '='.

                // Check for '{' on same line.
                rc = strchr(line, '{');
            
                // Get the name of the namespace.
                name = strtok(line, " \t\n{");
                name = trimWhiteSpace(name);
                if (name == NULL)
                {
                    LOG_ERROR("Error parsing properties file: unknown error.");
                }
                else if (name[0] == '}')
                {
                    // End of namespace.
                    return;
                }

                // Get its ID if it has one.
                value = strtok(NULL, "{");
                value = trimWhiteSpace(value);
                if (value != NULL && value[0] == '{')
                {
                    // New namespace without an ID.
                    Properties* space = new Properties(file, name, NULL);
                    _namespaces.push_back(space);
                }
                else
                {
                    // If '{' appears on the same line.
                    if (rc != NULL)
                    {
                        // Create new namespace.
                        Properties* space = new Properties(file, name, value);
                        _namespaces.push_back(space);
                    }
                    else
                    {
                        // Find out if the next line starts with "{"
                        skipWhiteSpace(file);
                        c = fgetc(file);
                        if (c == '{')
                        {
                            // Create new namespace.
                            Properties* space = new Properties(file, name, value);
                            _namespaces.push_back(space);
                        }
                        else
                        {
                            // Back up from fgetc()
                            fseek(file, -1, SEEK_CUR);

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
    unsigned int count = _namespaces.size();
    for (unsigned int i = 0; i < count; i++)
    {
        SAFE_DELETE(_namespaces[i]);
    }
}

void Properties::skipWhiteSpace(FILE* file)
{
    int c;

    do
    {
        c = fgetc(file);
    } while (isspace(c));

    // We found a non-whitespace character; put the cursor back in front of it.
    fseek(file, -1, SEEK_CUR);
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

const char* Properties::getNextProperty(const char** value)
{
    if (_propertiesItr == _properties.end())
    {
        // Restart from the beginning
        _propertiesItr = _properties.begin();
    }
    else
    {
        // Move to the next property
        _propertiesItr++;
    }

    if (_propertiesItr != _properties.end())
    {
        const std::string& name = _propertiesItr->first;
        if (!name.empty())
        {
            if (value)
            {
                *value = _propertiesItr->second.c_str();
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
        _namespacesItr++;
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

Properties* Properties::getNamespace(const char* id) const
{
    Properties* ret = NULL;
    std::vector<Properties*>::const_iterator it;
    
    for (it = _namespaces.begin(); it < _namespaces.end(); it++)
    {
        ret = *it;
        if (strcmp(ret->_id.c_str(), id) == 0)
        {
            return ret;
        }
        
        // Search recursively.
        ret = ret->getNamespace(id);
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
    assert(name);
    return _properties.find(name) != _properties.end();
}

bool isStringNumeric(const char* str)
{
    char* ptr = const_cast<char*>(str);

    // First character must be a digit
    if (!isdigit(*ptr))
        return false;
    ptr++;

    // All remaining characters must be digits, with a single decimal (.) permitted
    unsigned int decimalCount = 0;
    while (*ptr)
    {
        if (!isdigit(*ptr))
        {
            if (*ptr == '.' && decimalCount == 0)
            {
                // Max of 1 decimal allowed
                decimalCount++;
            }
            else
            {
                // Not a number
            }
        }
        ptr++;
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
    //unsigned int length = strlen(value);
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

bool Properties::getBool(const char* name) const
{
    if (exists(name))
    {
        if (_properties.find(name)->second == "true")
        {
            return true;
        }
    }

    return false;
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
            LOG_ERROR_VARG("Error parsing property: %s", name);
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
            LOG_ERROR_VARG("Error parsing property: %s", name);
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
            LOG_ERROR_VARG("Error parsing property: %s", name);
            return 0L;
        }
        return value;
    }

    return 0L;
}

bool Properties::getMatrix(const char* name, Matrix* out) const
{
    assert(out);

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
            LOG_ERROR_VARG("Error parsing property: %s", name);
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
    assert(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y;
        int scanned;
        scanned = sscanf(valueString, "%f,%f", &x, &y);
        if (scanned != 2)
        {
            LOG_ERROR_VARG("Error parsing property: %s", name);
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
    assert(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y, z;
        int scanned;
        scanned = sscanf(valueString, "%f,%f,%f", &x, &y, &z);
        if (scanned != 3)
        {
            LOG_ERROR_VARG("Error parsing property: %s", name);
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
    assert(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        float x, y, z, w;
        int scanned;
        scanned = sscanf(valueString, "%f,%f,%f,%f", &x, &y, &z, &w);
        if (scanned != 4)
        {
            LOG_ERROR_VARG("Error parsing property: %s", name);
            out->set(0.0f, 0.0f, 0.0f, 0.0f);
            return false;
        }

        out->set(x, y, z, w);
        return true;
    }
    
    out->set(0.0f, 0.0f, 0.0f, 0.0f);
    return false;
}

bool Properties::getColor(const char* name, Vector3* out) const
{
    assert(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        if (strlen(valueString) != 7 ||
            valueString[0] != '#')
        {
            // Not a color string.
            LOG_ERROR_VARG("Error parsing property: %s", name);
            out->set(0.0f, 0.0f, 0.0f);
            return false;
        }

        // Read the string into an int as hex.
        unsigned int color;
        sscanf(valueString+1, "%x", &color);

        out->set(Vector3::fromColor(color));
        return true;
    }

    out->set(0.0f, 0.0f, 0.0f);
    return false;
}

bool Properties::getColor(const char* name, Vector4* out) const
{
    assert(out);

    const char* valueString = getString(name);
    if (valueString)
    {
        if (strlen(valueString) != 9 ||
            valueString[0] != '#')
        {
            // Not a color string.
            LOG_ERROR_VARG("Error parsing property: %s", name);
            out->set(0.0f, 0.0f, 0.0f, 0.0f);
            return false;
        }

        // Read the string into an int as hex.
        unsigned int color;
        sscanf(valueString+1, "%x", &color);

        out->set(Vector4::fromColor(color));
        return true;
    }

    out->set(0.0f, 0.0f, 0.0f, 0.0f);
    return false;
}

}
