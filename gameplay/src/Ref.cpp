/*
 * Ref.cpp
 */

#include "Base.h"
#include "Ref.h"

namespace gameplay
{

Ref::Ref() :
    _refCount(1)
{
}

Ref::~Ref()
{
}

void Ref::addRef()
{
    ++_refCount;
}

void Ref::release()
{
    if ((--_refCount) <= 0)
    {
        delete this;
    }
}

}
