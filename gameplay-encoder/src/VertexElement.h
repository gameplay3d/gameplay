#ifndef VERTEXELEMENT_H_
#define VERTEXELEMENT_H_

#include "Object.h"

namespace gameplay
{

class VertexElement : public Object
{
public:

    /**
     * Constructor.
     */
    VertexElement(unsigned int t, unsigned int c);

    /**
     * Destructor.
     */
    virtual ~VertexElement(void);

    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    static const char* usageStr(unsigned int usage);

    unsigned int usage;
    unsigned int size;
};

}

#endif
