#include "Base.h"
#include "TileSheet.h"

//TODO: Need to create Lua binding

namespace gameplay
{

TileSheet::TileSheet(const char* id)
{
	if (id)
    {
        _id = id;
    }
}

TileSheet::~TileSheet()
{
}

const char* TileSheet::getId() const
{
    return _id.c_str();
}

}