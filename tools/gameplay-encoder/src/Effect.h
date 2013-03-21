#ifndef EFFECT_H_
#define EFFECT_H_

#include "Object.h"

namespace gameplay
{

class Effect : public Object
{
public:

    /**
     * Constructor.
     */
    Effect(void);

    /**
     * Destructor.
     */
    virtual ~Effect(void);

    virtual unsigned int getTypeId(void) const;

    virtual const char* getElementName(void) const;

    virtual void writeBinary(FILE* file);

    virtual void writeText(FILE* file);

private:
    
    std::string _vertexShader;
    std::string _fragmentShader;
};

}

#endif
