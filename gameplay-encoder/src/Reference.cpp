#include "Reference.h"

namespace gameplay
{

Reference::Reference(void) :
    type(0),
    offset(0),
    ref(NULL)
{
}

Reference::Reference(std::string _xref, Object* _ref) :
    xref(_xref),
    type(_ref->getTypeId()),
    offset(0),
    ref(_ref)
{
}

Reference::~Reference(void)
{
}

const char* Reference::getElementName(void) const
{
    return "Reference";
}

void Reference::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(xref, file);
    write(type, file);
    write(offset, file);
}
void Reference::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "xref", xref);
    fprintfElement(file, "type", type);
    fprintfElement(file, "offset", offset);
    fprintElementEnd(file);
}

bool Reference::updateOffset(FILE* file)
{
    long newOffset = ref->getFilePosition();
    return updateOffset(file, newOffset);
}

bool Reference::updateOffset(FILE* file, long newOffset)
{
    if (getFilePosition() > 0)
    {
        // save the current offset
        long savedOffset = ftell(file);

        // update the offset data for this
        offset = newOffset;
        // seek this Reference object in the file
        fseek(file, getFilePosition(), SEEK_SET);

        // skip over the object type
        //fseek(file, sizeof(unsigned int), SEEK_CUR);
        // skip over the id string
        skipString(file);
        // skip over the type
        //skipUint(file);

        // write over the old offset
        write(offset, file);

        // restore the offset
        fseek(file, savedOffset, SEEK_SET);
        return true;
    }
    return false;
}

Object* Reference::getObj()
{
    return ref;
}

}