#ifndef CAMERA_H_
#define CAMERA_H_

#include "Object.h"

namespace gameplay
{

class Camera : public Object
{
public:

    /**
     * Constructor.
     */
    Camera(void);

    /**
     * Destructor.
     */
    virtual ~Camera(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    void setPerspective();
    void setOrthographic();
    void setAspectRatio(float value);
    void setNearPlane(float value);
    void setFarPlane(float value);
    void setViewportWidth(float width);
    void setViewportHeight(float height);
    void setFieldOfView(float value);

    float getViewPortWidth();
    float getViewPortHeight();

    enum CameraType
    {
        CameraPerspective = 1,
        CameraOrthographic = 2
    };

private:
    
    unsigned char _cameraType;
    float _fieldOfView;
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;
    float _viewportWidth;
    float _viewportHeight;
};

}

#endif
