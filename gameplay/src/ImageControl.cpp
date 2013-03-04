#include "Base.h"
#include "ImageControl.h"

namespace gameplay
{

ImageControl::ImageControl() :
    _srcRegion(Rectangle::empty()), _dstRegion(Rectangle::empty()), _batch(NULL),
    _tw(0.0f), _th(0.0f), _uvs(Theme::UVs::full())
{
}

ImageControl::~ImageControl()
{
    SAFE_DELETE(_batch);
}

ImageControl* ImageControl::create(const char* id, Theme::Style* style)
{
    GP_ASSERT(style);

    ImageControl* imageControl = new ImageControl();
    if (id)
        imageControl->_id = id;
    imageControl->setStyle(style);

    return imageControl;
}

ImageControl* ImageControl::create(Theme::Style* style, Properties* properties)
{
    ImageControl* imageControl = new ImageControl();
    imageControl->initialize(style, properties);
    return imageControl;
}

void ImageControl::initialize(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);

    Control::initialize(style, properties);

    const char* path = properties->getString("path");
    if (path)
    {
        setImage(path);
    }

    Vector4 region;
    properties->getVector4("srcRegion", &region);
    setSrcRegion(region.x, region.y, region.z, region.w);
    properties->getVector4("dstRegion", &region);
    setDstRegion(region.x, region.y, region.z, region.w);
}

void ImageControl::setImage(const char* path)
{
    SAFE_DELETE(_batch);
    Texture* texture = Texture::create(path);
    _batch = SpriteBatch::create(texture);
    _tw = 1.0f / texture->getWidth();
    _th = 1.0f / texture->getHeight();
    texture->release();
}

void ImageControl::setImage(Image* image)
{
    SAFE_DELETE(_batch);
    Texture* texture = Texture::create(image);
    _batch = SpriteBatch::create(texture);
    _tw = 1.0f / texture->getWidth();
    _th = 1.0f / texture->getHeight();
    texture->release();
}

void ImageControl::setSrcRegion(float x, float y, float width, float height)
{
    _srcRegion.set(x, y, width, height);

    _uvs.u1 = x * _tw;
    _uvs.u2 = (x + width) * _tw;
    _uvs.v1 = 1.0f - (y * _th);
    _uvs.v2 = 1.0f - ((y + height) * _tw);
}

const Rectangle& ImageControl::getSrcRegion() const
{
    return _srcRegion;
}

void ImageControl::setDstRegion(float x, float y, float width, float height)
{
    _dstRegion.set(x, y, width, height);
}

const Rectangle& ImageControl::getDstRegion() const
{
    return _dstRegion;
}

void ImageControl::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    spriteBatch->finish();

    // An ImageControl is not part of the texture atlas but should use the same projection matrix.
    _batch->setProjectionMatrix(spriteBatch->getProjectionMatrix());

    _batch->start();
    if (_dstRegion.isEmpty())
    {
        _batch->draw(_viewportBounds.x, _viewportBounds.y, _viewportBounds.width, _viewportBounds.height,
            _uvs.u1, _uvs.v1, _uvs.u2, _uvs.v2, Vector4::one(), _viewportClipBounds);
    }
    else
    {
        _batch->draw(_viewportBounds.x + _dstRegion.x, _viewportBounds.y + _dstRegion.y,
            _dstRegion.width, _dstRegion.height,
            _uvs.u1, _uvs.v1, _uvs.u2, _uvs.v2, Vector4::one(), _viewportClipBounds);
    }
    _batch->finish();

    spriteBatch->start();
}

}