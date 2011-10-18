#include <assert.h>
#include "Camera.h"

namespace gameplay
{

Camera::Camera(void) :
    fieldOfView(0.0f),
    aspectRatio(0.0f),
    nearPlane(0.0f),
    farPlane(0.0f),
    viewportWidth(0.0f),
    viewportHeight(0.0f)
{
}

Camera::~Camera(void)
{
}

void Camera::setFieldOfView(float value)
{
    fieldOfView = value;
}


void Camera::setAspectRatio(float value)
{
    aspectRatio = value;
}


void Camera::setNearPlane(float value)
{
    nearPlane = value;
}


void Camera::setFarPlane(float value)
{
    farPlane = value;
}

unsigned int Camera::getTypeId(void) const
{
    return CAMERA_ID;
}
const char* Camera::getElementName(void) const
{
    return "Camera";
}

void Camera::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    write(cameraType, file);
    write(aspectRatio, file);
    write(nearPlane, file);
    write(farPlane, file);

    if (cameraType == CameraPerspective)
    {
        write(fieldOfView, file);
    }
    else if (cameraType == CameraOrthographic)
    {
        write(getViewPortWidth(), file);
        write(getViewPortHeight(), file);
    }
    else
    {
        assert(false);
    }
}
void Camera::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "cameraType", cameraType == CameraPerspective ? "CameraPerspective" : "CameraOrthographic");
    fprintfElement(file, "aspectRatio", aspectRatio);
    fprintfElement(file, "nearPlane", nearPlane);
    fprintfElement(file, "farPlane", farPlane);

    if (cameraType == CameraPerspective)
    {
        fprintfElement(file, "fieldOfView", fieldOfView);
    }
    else if (cameraType == CameraOrthographic)
    {
        fprintfElement(file, "viewportWidth", viewportWidth);
        fprintfElement(file, "viewportHeight", viewportHeight);
    }
    else
    {
        assert(false);
    }
    fprintElementEnd(file);
}

void Camera::setPerspective()
{
    cameraType = CameraPerspective;
}
void Camera::setOrthographic()
{
    cameraType = CameraOrthographic;
}
void Camera::setViewportWidth(float width)
{
    viewportWidth = width;
}
void Camera::setViewportHeight(float height)
{
    viewportHeight = height;
}

float Camera::getViewPortWidth()
{
    return viewportWidth;
}
float Camera::getViewPortHeight()
{
    return viewportHeight;
}

}
