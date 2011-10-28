/*
 * Properties.cpp
 */

#include "Properties.h"
#include "FileSystem.h"
#include <xtree>

namespace gameplay
{

Properties::Properties(FILE* file)
{
    readProperties(file);
    _mapIt = _map.begin();
    _propertiesIt = _properties.begin();
}

Properties::Properties(FILE* file, const char* name, const char* id) : _namespace(name)
{
    if (id)
    {
        _id = id;
    }
    readProperties(file);
    _mapIt = _map.begin();
    _propertiesIt = _properties.begin();
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

    while(!feof(file))
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
                _map[name] = value;

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
                    _properties.push_back(space);
                }
                else
                {
                    // If '{' appears on the same line.
                    if (rc != NULL)
                    {
                        // Create new namespace.
                        Properties* space = new Properties(file, name, value);
                        _properties.push_back(space);
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
                            _properties.push_back(space);
                        }
                        else
                        {
                            // Back up from fgetc()
                            fseek(file, -1, SEEK_CUR);

                            // Store "name value" as a name/value pair, or even just "name".
                            if (value != NULL)
                            {
                                _map[name] = value;
                            }
                            else
                            {
                                _map[name] = std::string();
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
    unsigned int count = _properties.size();
    for (unsigned int i = 0; i < count; i++)
    {
        SAFE_DELETE(_properties[i]);
    }
}

void Properties::skipWhiteSpace(FILE* file)
{
    int c;

    do
    {
        c = fgetc(file);
    } while (c == ' ' || c == 0x0D || c == '\n');

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
    while(isspace(*str)) str++;

    // All spaces?
    if(*str == 0)  
    {
        return str;
    }

    // Trim trailing space.
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // Write new null terminator.
    *(end+1) = 0;

    return str;
}

const char* Properties::getNextProperty()
{
    if (_mapIt != _map.end())
    {
        if (!_mapIt->first.empty())
        {
            const char* c_str = _mapIt->first.c_str();
            _mapIt++;
            return c_str; //_mapIt++->first.c_str();
        }
    }
    return NULL;
}

Properties* Properties::getNextNamespace()
{
    if (_propertiesIt < _properties.end())
    {
        Properties* space = *_propertiesIt;
        _propertiesIt++;
        return space;
    }
    return NULL;
}

void Properties::rewind()
{
    _mapIt = _map.begin();
    _propertiesIt = _properties.begin();
}

Properties* Properties::getNamespace(const char* id) const
{
    Properties* ret = NULL;
    std::vector<Properties*>::const_iterator it;
    
    for (it = _properties.begin(); it < _properties.end(); it++)
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

const char* Properties::getID() const
{
    return _id.c_str();
}

bool Properties::exists(const char* name) const
{
    assert(name);
    return _map.find(name) != _map.end();
}

const char* Properties::getString(const char* name) const
{
    if (exists(name))
    {
        return (_map.find(name)->second).c_str();
    }

    return NULL;
}

bool Properties::getBool(const char* name) const
{
    if (exists(name))
    {
        if (_map.find(name)->second == "true")
        {
            return true;
        }
    }

    return false;
}

int Properties::getInt(const char* name) const
{
    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        int value;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%d", &value);
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
    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        float value;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%f", &value);
        if (scanned != 1)
        {
            LOG_ERROR_VARG("Error parsing property: %s", name);
            return 0.0f;
        }
        return value;
    }

    return 0.0f;
}

bool Properties::getFloatArray(const char* name, float* out, unsigned int count) const
{
    assert(out);

    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        for (unsigned int i = 0; i < count; i++)
        {
            int scanned;
            scanned = sscanf(valueString.c_str(), "%f", &out[i]);
            if (scanned != 1)
            {
                LOG_ERROR_VARG("Error parsing property: %s", name);
                return false;
            }

            int position = valueString.find(',');
            valueString.erase(0, position + 1);
            return true;
        }
    }
    
    return false;
}

long Properties::getLong(const char* name) const
{
    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        long value;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%ld", &value);
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

    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        float m[16];
        int scanned;
        scanned = sscanf(valueString.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
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

    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        float x, y;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%f,%f", &x, &y);
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

    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        float x, y, z;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%f,%f,%f", &x, &y, &z);
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

    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        float x, y, z, w;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%f,%f,%f,%f", &x, &y, &z, &w);
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

bool Properties::getColor(const char* name, Color* out) const
{
    assert(out);

    if (exists(name))
    {
        std::string valueString = _map.find(name)->second;

        float r, g, b, a;
        int scanned;
        scanned = sscanf(valueString.c_str(), "%f,%f,%f,%f", &r, &g, &b, &a);
        if (scanned != 4)
        {
            LOG_ERROR_VARG("Error parsing property: %s", name);
            out->set(0.0f, 0.0f, 0.0f, 1.0f);
            return false;
        }

        out->set(r, g, b, a);
        return true;
    }
    
    out->set(0.0f, 0.0f, 0.0f, 1.0f);
    return false;
}

}