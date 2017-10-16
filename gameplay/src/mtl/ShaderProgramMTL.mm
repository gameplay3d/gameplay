#include "Base.h"
#include "ShaderProgramMTL.h"

namespace gameplay
{

ShaderProgramMTL::ShaderProgramMTL() :
	ShaderProgram(),
	_device(nullptr),
	_vertShader(nullptr),
	_tescShader(nullptr),
	_teseShader(nullptr),
	_geomShader(nullptr),
	_fragShader(nullptr),
	_compShader(nullptr)
{
}

ShaderProgramMTL::ShaderProgramMTL(unsigned int stages,
								  id<MTLDevice> device,
								  id<MTLLibrary> library,
								  id<MTLFunction> vertShader,
								  id<MTLFunction> tescShader,
								  id<MTLFunction> teseShader,
								  id<MTLFunction> geomShader,
								  id<MTLFunction> fragShader,
								  id<MTLFunction> compShader) :
	ShaderProgram(stages),
	_device(device),
	_library(library),
	_vertShader(vertShader),
	_tescShader(tescShader),
	_teseShader(teseShader),
	_geomShader(geomShader),
	_fragShader(fragShader),
	_compShader(compShader)
{
}

ShaderProgramMTL::~ShaderProgramMTL()
{
}

}