#ifndef TRANSFORM_H_
#define TRANSFORM_H_

namespace gameplay
{

class Transform
{
public:

    enum TransformProperties
    {
        /**
         * Scale animation property. Data=sx,sy,sz
         */
        ANIMATE_SCALE = 1,
        ANIMATE_SCALE_X,
        ANIMATE_SCALE_Y,
        ANIMATE_SCALE_Z,
        ANIMATE_SCALE_XY,
        ANIMATE_SCALE_XZ,
        ANIMATE_SCALE_YZ,

        /**
         * Rotation animation property. Data=qx,qy,qz,qw (as quaternion).
         */
        ANIMATE_ROTATE,

        /**
         * Translation animation property. Data=tx,ty,tz
         */
        ANIMATE_TRANSLATE,
        ANIMATE_TRANSLATE_X,
        ANIMATE_TRANSLATE_Y,
        ANIMATE_TRANSLATE_Z,
        ANIMATE_TRANSLATE_XY,
        ANIMATE_TRANSLATE_XZ,
        ANIMATE_TRANSLATE_YZ,

        /**
         * Rotation + Translation animation property(Rigid Body). Data=qx,qy,qz,qw,tx,ty,tz
         */
        ANIMATE_ROTATE_TRANSLATE,
        /**
         * Scale, Rotation + Translation animation property. Data=sx,sy,sz,qx,qy,qz,qw,tx,ty,tz
         */
        ANIMATE_SCALE_ROTATE_TRANSLATE
    };
};

}

#endif
