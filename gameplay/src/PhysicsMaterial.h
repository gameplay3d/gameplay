#pragma once

#include "Physics.h"

namespace gameplay
{

class PhysicsMaterial
{
public:

    /**
     * Defines combine modes.
     */
    enum CombineMode
    {
        eAverage,
        eMin,
        eMax,
        eMulitply
    };

    /**
     * Constructor.
     */
    PhysicsMaterial();
    
    /**
     * Destructor.
     */
    ~PhysicsMaterial();

    /**
     * Sets the friction used when an object is moving.
     *
     * A value from 0.0 - 1.0. A value of 0.0 feels very slippery and a
     * value of 1.0 makes the object come to rest very quickly.
     *
     * @param dynamicFriction The friction used when an object is moving.
     */
    void setDynamicFriction(float dynamicFriction);

    /**
     * Gets the friction used when an object is moving.
     *
     * A value from 0.0 - 1.0. A value of 0.0 feels very slippery and a
     * value of 1.0 makes the object come to rest very quickly.
     *
     * @return The friction used when an object is moving.
     */
    float getDynamicFriction() const;

    /**
     * Sets the friction used when an object is still.
     *
     * A value from 0.0 - 1.0. A value of 0.0 feels very slippery and a
     * value of 1.0 makes the object hard to start moving.
     *
     * @param staticFriction The friction used when an object is still.
     */
    void setStaticFriction(float staticFriction);

    /**
     * Gets the friction used when an object is still.
     *
     * A value from 0.0 - 1.0. A value of 0.0 feels very slippery and a
     * value of 1.0 makes the object hard to start moving.
     *
     * @return The friction used when an object is still.
     */
    float getStaticFriction() const;

    /**
     * Sets how the friction of the two colliding objects are combined together.
     *
     * @param combineMode How the friction of the two colliding objects are combined together.
     */
    void setFrictionCombineMode(CombineMode combineMode);

    /**
     * Gets how the friction of the two colliding objects are combined together.
     *
     * @return How the friction of the two colliding objects are combined together.
     */
    CombineMode getFrictionCombineMode();

    /**
     * Sets how bouncy the surface is.
     *
     * A value from 0.0 - 1.0. A value of 0.0 will not bound. A value of 1.0 will
     * bounce with no loss of energy.
     *
     * @param bounce How bouncy the surface is.
     */
    void setBounce(float bounce);

    /**
     * Gets how bouncy the surface is.
     *
     * A value from 0.0 - 1.0. A value of 0.0 will not bound. A value of 1.0 will
     * bounce with no loss of energy.
     *
     * @return How bouncy the surface is.
     */
    float getBounce() const;

    /**
     * Sets how the bounce of two colliding objects are combined together.
     *
     * @param combineMode How the bounce of the two colliding objects are combined together.
     */
    void setBounceCombineMode(CombineMode combineMode);

    /**
     * Gets how the bounce of the two colliding objects are combined together.
     *
     * @return How the bounce of the two colliding objects are combined together.
     */
    CombineMode getBounceCombineMode();
};

}
