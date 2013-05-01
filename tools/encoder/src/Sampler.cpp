#include "Base.h"
#include "Sampler.h"
#include "FileIO.h"

namespace gameplay
{

using std::string;
using std::map;

Sampler::Sampler(const char* id) :
    _id(id)
{
}

Sampler::~Sampler(void)
{
}

const string& Sampler::getId() const
{
    return _id;
}

const char* Sampler::getString(const string& name)
{
    map<string, string>::const_iterator it = props.find(name);
    if (it != props.end())
    {
        return it->second.c_str();
    }
    return NULL;
}

void Sampler::set(const string& name, const string& value)
{
    props[name] = value;
}

void Sampler::writeMaterial(FILE* file, unsigned int indent, Sampler* parent)
{
    writeIndent(indent, file);
    fprintf(file, "sampler %s\n", _id.c_str());
    writeIndent(indent, file);
    fprintf(file, "{\n");
    ++indent;

    const char* relativePath = getString("relativePath");
    if (relativePath)
        set("path", relativePath);

    writeProperty(file, "path", indent, parent);
    writeProperty(file, "mipmap", indent, parent);
    writeProperty(file, "wrapS", indent, parent);
    writeProperty(file, "wrapT", indent, parent);
    writeProperty(file, MIN_FILTER, indent, parent);
    writeProperty(file, MAG_FILTER, indent, parent);
    --indent;
    writeIndent(indent, file);
    fprintf(file, "}\n");
}

void Sampler::writeProperty(FILE* file, const string& name, unsigned int indent, Sampler* parent)
{
    const char* value = getString(name);
    if (value != NULL)
    {
        const char* parentValue = NULL;
        // Don't print the property if it is the same as the parent's property
        if (parent != NULL)
        {
            parentValue = parent->getString(name);
        }
        if (parentValue == NULL || strcmp(value, parentValue) != 0)
        {
            writeIndent(indent, file);
            fprintf(file, "%s = %s\n", name.c_str(), value);
        }
    }
}

}
