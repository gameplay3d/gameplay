#include "Base.h"
#include "Transform.h"

namespace gameplay
{

const char* Transform::getPropertyString(unsigned int prop)
{
    switch (prop)
    {
        case ANIMATE_SCALE:
            return "ANIMATE_SCALE";
        case ANIMATE_SCALE_X:
            return "ANIMATE_SCALE_X";
        case ANIMATE_SCALE_Y: 
            return "ANIMATE_SCALE_Y";
        case ANIMATE_SCALE_Z:
            return "ANIMATE_SCALE_Z";
        case ANIMATE_SCALE_XY: 
            return "ANIMATE_SCALE_XY";
        case ANIMATE_SCALE_XZ: 
            return "ANIMATE_SCALE_XZ";
        case ANIMATE_SCALE_YZ:
            return "ANIMATE_SCALE_YZ";
        case ANIMATE_ROTATE:
            return "ANIMATE_ROTATE";
        case ANIMATE_TRANSLATE: 
            return "ANIMATE_TRANSLATE";
        case ANIMATE_TRANSLATE_X:
            return "ANIMATE_TRANSLATE_X";
        case ANIMATE_TRANSLATE_Y: 
            return "ANIMATE_TRANSLATE_Y";
        case ANIMATE_TRANSLATE_Z: 
            return "ANIMATE_TRANSLATE_Z";
        case ANIMATE_TRANSLATE_XY: 
            return "ANIMATE_TRANSLATE_XY";
        case ANIMATE_TRANSLATE_XZ: 
            return "ANIMATE_TRANSLATE_XZ";
        case ANIMATE_TRANSLATE_YZ: 
            return "ANIMATE_TRANSLATE_YZ";
        case ANIMATE_ROTATE_TRANSLATE: 
            return "ANIMATE_ROTATE_TRANSLATE";
        case ANIMATE_SCALE_ROTATE_TRANSLATE: 
            return "ANIMATE_SCALE_ROTATE_TRANSLATE";
        case ANIMATE_ROTATE_X: 
            return "ANIMATE_ROTATE_X";
        case ANIMATE_ROTATE_Y: 
            return "ANIMATE_ROTATE_Y";
        case ANIMATE_ROTATE_Z: 
            return "ANIMATE_ROTATE_Z";
        default:
            return "";
    }
}

unsigned int Transform::getPropertySize(unsigned int prop)
{
    switch (prop)
    {
        case ANIMATE_SCALE_ROTATE_TRANSLATE: 
            return 10;
        case ANIMATE_ROTATE_TRANSLATE: 
            return 7;
        case ANIMATE_ROTATE:
            return 4;
        case ANIMATE_SCALE:
        case ANIMATE_TRANSLATE: 
            return 3;   
        case ANIMATE_SCALE_XY: 
        case ANIMATE_SCALE_XZ: 
        case ANIMATE_SCALE_YZ:
        case ANIMATE_TRANSLATE_XY: 
        case ANIMATE_TRANSLATE_XZ: 
        case ANIMATE_TRANSLATE_YZ: 
            return 2;
        case ANIMATE_SCALE_X:
        case ANIMATE_SCALE_Y:
        case ANIMATE_SCALE_Z:
        case ANIMATE_TRANSLATE_X:
        case ANIMATE_TRANSLATE_Y:
        case ANIMATE_TRANSLATE_Z:
        case ANIMATE_ROTATE_X: 
        case ANIMATE_ROTATE_Y: 
        case ANIMATE_ROTATE_Z: 
            return 1;
        default:
            return 0;
    }
}

}