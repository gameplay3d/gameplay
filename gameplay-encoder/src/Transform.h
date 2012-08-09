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

        /**
         * Rotation + Translation animation property(Rigid Body). Data=qx,qy,qz,qw,tx,ty,tz
         */
        ANIMATE_ROTATE_TRANSLATE = 16,

        /**
         * Scale, Rotation + Translation animation property. Data=sx,sy,sz,qx,qy,qz,qw,tx,ty,tz
         */
        ANIMATE_SCALE_ROTATE_TRANSLATE = 17,

        /**
         * Scale + Translation animation property. Data=sx,sy,sz,tx,ty,tz
         */
        ANIMATE_SCALE_TRANSLATE = 18,

        /**
         * Scale + Rotation animation property. Data=sx,sy,sz,qx,qy,qz,qw
         */
        ANIMATE_SCALE_ROTATE = 19
    };

    /**
     * Returns the string representation of the given TransformProperty.
     */
    static const char* getPropertyString(unsigned int prop);

    /**
     * Returns the number of floats for the given property or zero if not a valid property.
     */
    static unsigned int getPropertySize(unsigned int prop);

};

}

#endif
