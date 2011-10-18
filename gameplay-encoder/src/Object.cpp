#include "Object.h"

namespace gameplay
{

Object::Object(void) : fposition(0)
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
    return id;
}

void Object::setId(const char* idStr)
{
    if (idStr)
        id = idStr;
}

void Object::setId(const std::string& newId)
{
    if (newId.length() > 0)
        id = newId;
}

void Object::fprintElementStart(FILE* file)
{
    if (id.length() > 0)
        fprintf(file, "<%s id=\"%s\">\n", getElementName(), id.c_str());
    else
        fprintf(file, "<%s>\n", getElementName());
}

void Object::fprintElementEnd(FILE* file)
{
    fprintf(file, "</%s>\n", getElementName());
}

unsigned int Object::getFilePosition()
{
    return (unsigned int)fposition;
}

void Object::saveFilePosition(FILE* file)
{
    fposition = ftell(file);
}

void Object::writeBinaryXref(FILE* file)
{
    std::string xref("#");
    xref.append(getId());
    write(xref, file);
}

}