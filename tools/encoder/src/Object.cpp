#include "Base.h"
#include "Object.h"

namespace gameplay
{

Object::Object(void) : _fposition(0)
{
}

Object::~Object(void)
{
}

unsigned int Object::getTypeId(void) const
{
    return 0;
}

void Object::writeBinary(FILE* file)
{
    saveFilePosition(file);
}

const std::string& Object::getId() const
{
    return _id;
}

void Object::setId(const char* idStr)
{
    if (idStr)
    {
        _id = idStr;
    }
}

void Object::setId(const std::string& newId)
{
    if (newId.length() > 0)
    {
        _id = newId;
    }
}

void Object::fprintElementStart(FILE* file)
{
    if (_id.length() > 0)
    {
        fprintf(file, "<%s id=\"%s\">\n", getElementName(), _id.c_str());
    }
    else
    {
        fprintf(file, "<%s>\n", getElementName());
    }
}

void Object::fprintElementEnd(FILE* file)
{
    fprintf(file, "</%s>\n", getElementName());
}

unsigned int Object::getFilePosition()
{
    return (unsigned int)_fposition;
}

void Object::saveFilePosition(FILE* file)
{
    _fposition = ftell(file);
}

void Object::writeBinaryXref(FILE* file)
{
    std::string xref("#");
    xref.append(getId());
    write(xref, file);
}

}
