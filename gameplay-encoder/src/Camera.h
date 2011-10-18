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
    void setFieldOfView(float value);
    void setAspectRatio(float value);
    void setNearPlane(float value);
    void setFarPlane(float value);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    void setPerspective();
    void setOrthographic();
    void setViewportWidth(float width);
    void setViewportHeight(float height);

    float getViewPortWidth();
    float getViewPortHeight();

    enum CameraType
    {
        CameraPerspective = 1,
        CameraOrthographic = 2
    };

private:
    unsigned char cameraType;
    float viewport[4];
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float viewportWidth;
    float viewportHeight;
};

}
#endif
