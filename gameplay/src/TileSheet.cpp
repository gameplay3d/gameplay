#include "Base.h"
#include "TileSheet.h"

//TODO: Need to create Lua binding

namespace gameplay
{

// Global list of active scenes
static std::vector<TileSheet*> _tileSheets;

TileSheet::TileSheet(const char* id)
	: _id(id ? id : ""), _batch(NULL)
{
}

TileSheet::~TileSheet()
{
	SAFE_DELETE(_batch);

	// Remove self from vector.
    std::vector<TileSheet*>::iterator it = std::find(_tileSheets.begin(), _tileSheets.end(), this);
    if (it != _tileSheets.end())
    {
        _tileSheets.erase(it);
    }
}

TileSheet* TileSheet::create(const char* id, Texture* texture, unsigned int initialCapacity)
{
	SpriteBatch* sb = SpriteBatch::create(texture, NULL, initialCapacity);

	TileSheet* tileSheet = new TileSheet(id);
	tileSheet->_batch = sb;

	_tileSheets.push_back(tileSheet);

	return tileSheet;
}

TileSheet* TileSheet::getTileSheet(const char* id)
{
	GP_ASSERT(id);

    // Search the vector for a matching ID.
    std::vector<TileSheet*>::const_iterator it;
    for (it = _tileSheets.begin(); it != _tileSheets.end(); ++it)
    {
        TileSheet* ts = *it;
        GP_ASSERT(ts);
		if (ts->_id == id)
        {
            return ts;
        }
    }
    return NULL;
}

//TODO

const char* TileSheet::getId() const
{
    return _id.c_str();
}

}