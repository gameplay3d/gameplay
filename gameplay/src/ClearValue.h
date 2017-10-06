#pragma once

namespace gameplay
{

/**
 * Defines the clear value for clearing either 
 * color or depth/stencil values.
 */
class ClearValue
{
public:
    union 
    {
        struct
        {
            float red;
            float green;
            float blue;
            float alpha;
        };
        struct
        {
            float depth;
            unsigned int stencil;
        };
    };
};

}
