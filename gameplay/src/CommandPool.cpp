#include "Base.h"
#include "CommandPool.h"

namespace gameplay
{

CommandPool::CommandPool()
{
}
CommandPool::~CommandPool()
{
}

bool CommandPool::isTransient() const
{
	return _transient;
}

}
