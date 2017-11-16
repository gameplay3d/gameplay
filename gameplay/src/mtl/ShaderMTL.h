#pragma once

#include "Shader.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a Shader.
 */
class ShaderMTL : public Shader
{
public:

    /**
     * Constructor.
     */
	ShaderMTL();

    /**
     * Constructor.
     */
    ShaderMTL(id<MTLDevice> device, id<MTLLibrary> library, id<MTLFunction> function);

    /**
	 * Destructor.
     */
    ~ShaderMTL();

	id<MTLDevice>  _device;
    id<MTLLibrary> _library;
	id<MTLFunction> _function;
};

}