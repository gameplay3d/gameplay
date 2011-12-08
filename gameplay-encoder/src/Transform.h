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
        ANIMATE_SCALE_X = 2,
        ANIMATE_SCALE_Y = 3,
        ANIMATE_SCALE_Z = 4,
        ANIMATE_SCALE_XY = 5,
        ANIMATE_SCALE_XZ = 6,
        ANIMATE_SCALE_YZ = 7,

        /**
         * Rotation animation property. Data=qx,qy,qz,qw (as quaternion).
         */
        ANIMATE_ROTATE = 8,

        /**
         * Translation animation property. Data=tx,ty,tz
         */
        ANIMATE_TRANSLATE = 9,
        ANIMATE_TRANSLATE_X = 10,
        ANIMATE_TRANSLATE_Y = 11,
        ANIMATE_TRANSLATE_Z = 12,
        ANIMATE_TRANSLATE_XY = 13,
        ANIMATE_TRANSLATE_XZ = 14,
        ANIMATE_TRANSLATE_YZ = 15,

        /**
         * Rotation + Translation animation property(Rigid Body). Data=qx,qy,qz,qw,tx,ty,tz
         */
        ANIMATE_ROTATE_TRANSLATE = 16,
        /**
         * Scale, Rotation + Translation animation property. Data=sx,sy,sz,qx,qy,qz,qw,tx,ty,tz
         */
        ANIMATE_SCALE_ROTATE_TRANSLATE = 17,

        ANIMATE_ROTATE_X = 18,
        ANIMATE_ROTATE_Y = 19,
        ANIMATE_ROTATE_Z = 20
    };
};

}

#endif
