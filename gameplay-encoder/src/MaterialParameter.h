#ifndef MATERIALPARAMETER_H_
#define MATERIALPARAMETER_H_

#include "Object.h"

namespace gameplay
{

class MaterialParameter : public Object
{
public:

    /**
     * Constructor.
     */
    MaterialParameter(void);

    /**
     * Destructor.
     */
    virtual ~MaterialParameter(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    std::vector<float> value;
    unsigned int type;
};


}
#endif

