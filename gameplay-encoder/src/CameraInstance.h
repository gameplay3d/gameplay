#ifndef CAMERAINSTANCE_H_
#define CAMERAINSTANCE_H_

#include "Object.h"
#include "Camera.h"

namespace gameplay
{

class CameraInstance : public Object
{
public:

    /**
     * Constructor.
     */
    CameraInstance(void);

    /**
     * Destructor.
     */
    virtual ~CameraInstance(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    Camera* getCamera();
    void setCamera(Camera* camera);

private:

    Camera* _ref;
};

}

#endif
