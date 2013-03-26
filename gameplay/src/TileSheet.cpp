#include "Base.h"
#include "TileSheet.h"
#include "Camera.h"

namespace gameplay
{

// Global list of active sheets
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
	return getStripFrameCount((unsigned int)getStrip(id));
}

unsigned int TileSheet::getStripFrameCount(unsigned int index) const
{
	GP_ASSERT(index < _strips.size());

	return _strips[index]._frameCount;
}

unsigned int TileSheet::addStrip(const char* id, unsigned int frameCount)
{
	GP_ASSERT(id && frameCount > 0);

	_strips.push_back(TileSheet::FrameStrip(id, frameCount));

	return _strips.size() - 1;
}

bool TileSheet::removeStrip(unsigned int index)
{
	if(index < _strips.size())
	{
		_strips.erase(_strips.begin() + index);
		return true;
	}
	return false;
}

bool TileSheet::removeStrip(const char* id)
{
	return removeStrip((unsigned int)getStrip(id));
}

const Rectangle& TileSheet::getStripFrame(unsigned int stripIndex, unsigned int frameIndex) const
{
	GP_ASSERT(stripIndex < _strips.size() && frameIndex < _strips[stripIndex]._frameCount);

	return _strips[stripIndex]._frames[frameIndex];
}

const Rectangle& TileSheet::getStripFrame(const char* id, unsigned int frameIndex) const
{
	return getStripFrame((unsigned int)getStrip(id), frameIndex);
}

void TileSheet::getStripFrames(unsigned int stripIndex, unsigned int frameIndex, Rectangle* frames, unsigned int frameCount) const
{
	GP_ASSERT(stripIndex < _strips.size() && frameIndex < _strips[stripIndex]._frameCount && frames && (frameIndex + frameCount) <= _strips[stripIndex]._frameCount);

	const FrameStrip& strip = _strips[stripIndex];

	std::copy(strip._frames.begin() + frameIndex, strip._frames.begin() + (frameIndex + frameCount), frames);
}

void TileSheet::getStripFrames(const char* id, unsigned int frameIndex, Rectangle* frames, unsigned int frameCount) const
{
	getStripFrames((unsigned int)getStrip(id), frameIndex, frames, frameCount);
}

void TileSheet::setStripFrame(unsigned int stripIndex, unsigned int frameIndex, const Rectangle& frame)
{
	GP_ASSERT(stripIndex < _strips.size() && frameIndex < _strips[stripIndex]._frameCount);

	_strips[stripIndex]._frames[frameIndex] = frame;
}

void TileSheet::setStripFrame(const char* id, unsigned int frameIndex, const Rectangle& frame)
{
	setStripFrame((unsigned int)getStrip(id), frameIndex, frame);
}

void TileSheet::setStripFrames(unsigned int stripIndex, unsigned int frameIndex, const Rectangle* frames, unsigned int frameCount)
{
	GP_ASSERT(stripIndex < _strips.size() && frameIndex < _strips[stripIndex]._frameCount && frames && (frameIndex + frameCount) <= _strips[stripIndex]._frameCount);

	FrameStrip& strip = _strips[stripIndex];

	std::copy(frames, frames + frameCount, strip._frames.begin() + frameIndex);
}

void TileSheet::setStripFrames(const char* id, unsigned int frameIndex, const Rectangle* frames, unsigned int frameCount)
{
	setStripFrames((unsigned int)getStrip(id), frameIndex, frames, frameCount);
}

void TileSheet::startBatch(const Camera* camera, bool viewProjection)
{
	_batch->start();

	if(camera)
	{
		_batch->setProjectionMatrix(viewProjection ? camera->getViewProjectionMatrix() : camera->getProjectionMatrix());
	}
}

void TileSheet::finishBatch()
{
	_batch->finish();
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