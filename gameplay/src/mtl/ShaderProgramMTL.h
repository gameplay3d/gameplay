#pragma once

#include "ShaderProgram.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of a ShaderProgram.
 */
class ShaderProgramMTL : public ShaderProgram
{
public:

    /**
     * Constructor.
     */
	ShaderProgramMTL();

    /**
     * Constructor.
     */
    ShaderProgramMTL(unsigned int stages,
					id<MTLDevice> device,
                    id<MTLLibrary> library,
					id<MTLFunction> tescShader,
					id<MTLFunction> teseShader,
					id<MTLFunction> domnShader,
					id<MTLFunction> geomShader,
					id<MTLFunction> fragShader,
					id<MTLFunction> compShader);
    /**
	 * Destructor.
     */
    ~ShaderProgramVK();

	id<MTLDevice>  _device;
    id<MTLLibrary> _library;
	id<MTLFunction> _vertShader;
	id<MTLFunction> _tescShader;
	id<MTLFunction> _teseShader;
	id<MTLFunction> _geomShader;
	id<MTLFunction> _fragShader;
	id<MTLFunction> _compShader;
};

}