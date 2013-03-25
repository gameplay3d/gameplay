#ifndef GLYPH_H_
#define GLYPH_H_

#include "Object.h"

namespace gameplay
{

class Glyph : public Object
{
public:

    /**
     * Constructor.
     */
    Glyph(void);

    /**
     * Destructor.
     */
    virtual ~Glyph(void);

    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    unsigned int index;
    unsigned int width;
    float uvCoords[4];
};

}

#endif
