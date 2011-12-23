#ifndef FONT_H_
#define FONT_H_

#include "Object.h"
#include "Glyph.h"

namespace gameplay
{

class Font : public Object
{
public:

    /**
     * Constructor.
     */
    Font(void);

    /**
     * Destructor.
     */
    virtual ~Font(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    std::string family;
    unsigned int style;
    unsigned int size;
    std::string  charset;
    std::list<Glyph*> glyphs;
    unsigned int texMapWidth;
    unsigned int texMapHeight;
    std::list<unsigned char> texMap;

    enum FontStyle
    {
        PLAIN = 0,
        BOLD = 1,
        ITALIC = 2,
        BOLD_ITALIC = 4
    };
};

}

#endif
