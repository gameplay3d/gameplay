#ifndef REFERENCE_H_
#define REFERENCE_H_

#include "Object.h"

namespace gameplay
{

class Reference : public Object
{
public:

    /**
     * Constructor.
     */
    Reference(void);
    Reference(std::string _xref, Object* _ref);

    /**
     * Destructor.
     */
    virtual ~Reference(void);

    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    /**
     * Updates the offset of this Reference object if it has already need written to file.
     * @param file The file stream.
     * 
     * @return True if the offset was updates, false otherwise.
     */
    bool updateOffset(FILE* file);

    /**
     * Updates the offset of this Reference object if it has already need written to file.
     * @param file The file stream.
     * @param newOffset The new file offset.
     * 
     * @return True if the offset in the binary file was updated. False if this ref hasn't been written to file yet.
     */
    bool updateOffset(FILE* file, long newOffset);

    Object* getObj();

private:
    std::string _xref;
    unsigned int _type;
    unsigned int _offset;

    Object* _ref;
};

}

#endif
