#include "Base.h"
#include "Reference.h"

namespace gameplay
{

Reference::Reference(void) :
    _type(0),
    _offset(0),
    _ref(NULL)
{
}

Reference::Reference(std::string _xref, Object* _ref) :
    _xref(_xref),
    _type(_ref->getTypeId()),
    _offset(0),
    _ref(_ref)
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
    write(_xref, file);
    write(_type, file);
    write(_offset, file);
}
void Reference::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "xref", _xref);
    fprintfElement(file, "type", _type);
    fprintfElement(file, "offset", _offset);
    fprintElementEnd(file);
}

bool Reference::updateOffset(FILE* file)
{
    long newOffset = _ref->getFilePosition();
    return updateOffset(file, newOffset);
}

bool Reference::updateOffset(FILE* file, long newOffset)
{
    if (getFilePosition() > 0)
    {
        // save the current offset
        long savedOffset = ftell(file);

        // update the offset data for this
        _offset = newOffset;
        // seek this Reference object in the file
        fseek(file, getFilePosition(), SEEK_SET);

        // skip over the object type
        //fseek(file, sizeof(unsigned int), SEEK_CUR);
        // skip over the id string
        skipString(file);
        // skip over the type
        //skipUint(file);

        // write over the old offset
        write(_offset, file);

        // restore the offset
        fseek(file, savedOffset, SEEK_SET);
        return true;
    }
    return false;
}

Object* Reference::getObj()
{
    return _ref;
}

}
