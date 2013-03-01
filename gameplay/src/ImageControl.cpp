#include "Base.h"
#include "ImageControl.h"

namespace gameplay
{

ImageControl::ImageControl() : _image(NULL), _texture(NULL), _batch(NULL)
{
}

ImageControl::~ImageControl()
{
    SAFE_RELEASE(_image);
    SAFE_RELEASE(_texture);
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
}

void ImageControl::setImage(const char* path)
{
    SAFE_RELEASE(_image);
    SAFE_RELEASE(_texture);
    SAFE_DELETE(_batch);

    _image = Image::create(path);
    _texture = Texture::create(_image);
    _batch = SpriteBatch::create(_texture);
}

void ImageControl::setImage(Image* image)
{
    SAFE_RELEASE(_image);
    SAFE_RELEASE(_texture);
    SAFE_DELETE(_batch);

    image->addRef();
    _image = image;
    _texture = Texture::create(_image);
    _batch = SpriteBatch::create(_texture);
}

void ImageControl::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    spriteBatch->finish();

    // An ImageControl is not part of the texture atlas but should use the same projection matrix.
    _batch->setProjectionMatrix(spriteBatch->getProjectionMatrix());

    _batch->start();
    _batch->draw(_viewportBounds.x, _viewportBounds.y, _viewportBounds.width, _viewportBounds.height,
        0.0f, 0.0f, 1.0f, 1.0f, Vector4::one(), _viewportClipBounds);
    _batch->finish();

    spriteBatch->start();
}

}