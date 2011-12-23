#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Object.h"
#include "MaterialParameter.h"
#include "Effect.h"

namespace gameplay
{

class Material : public Object
{
public:

    /**
     * Constructor.
     */
    Material(void);

    /**
     * Destructor.
     */
    virtual ~Material(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

private:
    std::list<MaterialParameter> _parameters;
    Effect* _effect;
};

}

#endif
