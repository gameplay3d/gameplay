#include "Base.h"
#include "CommandPoolVK.h"
#include "CommandListVK.h"

namespace gameplay
{

CommandPoolVK::CommandPoolVK() :
	_commandPool(nullptr)
{
}

CommandPoolVK::CommandPoolVK(VkCommandPool pool) :
	_commandPool(pool)
{
}

CommandPoolVK::~CommandPoolVK()
{
}

}
