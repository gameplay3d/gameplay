#include "Base.h"
#include "Buffer.h"

namespace gameplay
{

Buffer::Buffer()
{
}

Buffer::Buffer(Type type, size_t size, bool dynamic) :
	_type(type), _size(size), _dynamic(dynamic)
{

}

Buffer::~Buffer()
{
}

Buffer::Type Buffer::getType() const
{
	return _type;
}

size_t Buffer::getSize() const
{
	return size_t();
}

bool Buffer::isDynamic() const
{
	return _dynamic;
}

}
