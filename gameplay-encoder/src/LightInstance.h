#ifndef LIGHTINSTANCE_H_
#define LIGHTINSTANCE_H_

#include "Object.h"
#include "Light.h"

namespace gameplay
{

class LightInstance : public Object
{
public:

    /**
     * Constructor.
     */
    LightInstance(void);

    /**
     * Destructor.
     */
    virtual ~LightInstance(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    Light* getLight() const;
    void setLight(Light* light);

    bool isAmbient() const;

private:

    Light* _ref;

};

}

#endif
