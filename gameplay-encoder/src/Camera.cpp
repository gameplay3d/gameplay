#include "Base.h"
#include "Camera.h"

namespace gameplay
{

Camera::Camera(void) :
    _cameraType(CameraPerspective), 
    _fieldOfView(0.0f),
    _aspectRatio(0.0f),
    _nearPlane(0.0f),
    _farPlane(0.0f),
    _viewportWidth(0.0f),
    _viewportHeight(0.0f)
{
}

Camera::~Camera(void)
{
}

void Camera::setFieldOfView(float value)
{
    _fieldOfView = value;
}


void Camera::setAspectRatio(float value)
{
    _aspectRatio = value;
}


void Camera::setNearPlane(float value)
{
    _nearPlane = value;
}


void Camera::setFarPlane(float value)
{
    _farPlane = value;
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
    write(_cameraType, file);
    write(_aspectRatio, file);
    write(_nearPlane, file);
    write(_farPlane, file);

    if (_cameraType == CameraPerspective)
    {
        write(_fieldOfView, file);
    }
    else if (_cameraType == CameraOrthographic)
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
    fprintfElement(file, "cameraType", _cameraType == CameraPerspective ? "CameraPerspective" : "CameraOrthographic");
    fprintfElement(file, "aspectRatio", _aspectRatio);
    fprintfElement(file, "nearPlane", _nearPlane);
    fprintfElement(file, "farPlane", _farPlane);

    if (_cameraType == CameraPerspective)
    {
        fprintfElement(file, "fieldOfView", _fieldOfView);
    }
    else if (_cameraType == CameraOrthographic)
    {
        fprintfElement(file, "viewportWidth", _viewportWidth);
        fprintfElement(file, "viewportHeight", _viewportHeight);
    }
    else
    {
        assert(false);
    }
    fprintElementEnd(file);
}

void Camera::setPerspective()
{
    _cameraType = CameraPerspective;
}
void Camera::setOrthographic()
{
    _cameraType = CameraOrthographic;
}
void Camera::setViewportWidth(float width)
{
    _viewportWidth = width;
}
void Camera::setViewportHeight(float height)
{
    _viewportHeight = height;
}

float Camera::getViewPortWidth()
{
    return _viewportWidth;
}
float Camera::getViewPortHeight()
{
    return _viewportHeight;
}

}
