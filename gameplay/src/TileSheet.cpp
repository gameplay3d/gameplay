#include "Base.h"
#include "TileSheet.h"

namespace gameplay
{

// Global list of active scenes
static std::vector<TileSheet*> _tileSheets;

TileSheet::TileSheet(const char* id)
	: _id(id ? id : ""), _batch(NULL), _strips()
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

const char* TileSheet::getId() const
{
    return _id.c_str();
}

SpriteBatch* TileSheet::getSpriteBatch() const
{
	return _batch;
}

unsigned int TileSheet::getStripCount() const
{
	return _strips.size();
}

const char* TileSheet::getStripId(unsigned int stripIndex) const
{
	return _strips[stripIndex]._id.c_str();
}

int TileSheet::getStrip(const char* id) const
{
	GP_ASSERT(id);

	std::vector<TileSheet::FrameStrip>::const_iterator it;
	int index;
	for (it = _strips.begin(), index = 0; it != _strips.end(); ++it, ++index)
    {
		if ((*it)._id == id)
        {
            return index;
        }
    }
    return -1;
}

unsigned int TileSheet::getStripFrameCount(const char* id) const
{
	int index = getStrip(id);
	if(index < 0)
	{
		return 0;
	}
	return getStripFrameCount(index);
}

unsigned int TileSheet::getStripFrameCount(unsigned int index) const
{
	return _strips[index]._frameCount;
}

void TileSheet::addStrip(const char* id, unsigned int frameCount)
{
	GP_ASSERT(frameCount > 0);

	_strips.push_back(TileSheet::FrameStrip(id, frameCount));
}

bool TileSheet::removeStrip(unsigned int index)
{
	if(index < _strips.size())
	{
		_strips.erase(_strips.begin() + index);
	}
	return false;
}

bool TileSheet::removeStrip(const char* id)
{
	int index = getStrip(id);
	if(index < 0)
	{
		return false;
	}
	return removeStrip(index);
}

const Rectangle& TileSheet::getStripFrame(unsigned int stripIndex, unsigned int frameIndex) const
{
	return _strips[stripIndex]._frames[frameIndex];
}

const Rectangle& TileSheet::getStripFrame(const char* id, unsigned int frameIndex) const
{
	int index = getStrip(id);
	if(index < 0)
	{
		return Rectangle::empty();
	}
	return getStripFrame(index, frameIndex);
}

void TileSheet::setStripFrame(unsigned int stripIndex, unsigned int frameIndex, const Rectangle& frame)
{
	_strips[stripIndex]._frames[frameIndex] = frame;
}

void TileSheet::setStripFrame(const char* id, unsigned int frameIndex, const Rectangle& frame)
{
	int index = getStrip(id);
	if(index >= 0)
	{
		_strips[index]._frames[frameIndex] = frame;
	}
}

//FrameStrip
TileSheet::FrameStrip::FrameStrip(const char* id, unsigned int frameCount)
	: _id(id ? id : ""), _frames(frameCount), _frameCount(frameCount)
{
}

TileSheet::FrameStrip::~FrameStrip()
{
}

}