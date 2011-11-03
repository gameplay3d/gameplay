#include "CameraInstance.h"

namespace gameplay
{

CameraInstance::CameraInstance(void) : _ref(NULL)
{

}

CameraInstance::~CameraInstance(void)
{
}

unsigned int CameraInstance::getTypeId(void) const
{
    return CAMERAINSTANCE_ID;
}
const char* CameraInstance::getElementName(void) const
{
    return "CameraInstance";
}

void CameraInstance::writeBinary(FILE* file)
{
    if (_ref != NULL)
    {
        _ref->writeBinary(file);
    }
    else
    {
        write((unsigned int)0, file);
    }
}
void CameraInstance::writeText(FILE* file)
{
    if (_ref != NULL)
    {
        _ref->writeText(file);
    }
}

Camera* CameraInstance::getCamera()
{
    return _ref;
}

void CameraInstance::setCamera(Camera* camera)
{
    _ref = camera;
}

}