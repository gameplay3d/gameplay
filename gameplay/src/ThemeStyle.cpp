#include "ThemeStyle.h"

namespace gameplay
{

/****************
 * Theme::Style *
 ****************/
Theme::Style::Style(Theme* theme, const char* id, float tw, float th,
        const Theme::Margin& margin, const Theme::Padding& padding,
        Theme::Style::Overlay* normal, Theme::Style::Overlay* focus,
        Theme::Style::Overlay* active, Theme::Style::Overlay* disabled,
        Theme::Style::Overlay* hover)
    : _theme(theme), _id(id), _tw(tw), _th(th), _margin(margin), _padding(padding)
{
    _overlays[OVERLAY_NORMAL] = normal;
    _overlays[OVERLAY_FOCUS] = focus;
    _overlays[OVERLAY_ACTIVE] = active;
    _overlays[OVERLAY_DISABLED] = disabled;
    _overlays[OVERLAY_HOVER] = hover;
}

Theme::Style::Style(const Style& copy)
{
    _id = copy._id;
    _margin = copy._margin;
    _padding = copy._padding;
    _tw = copy._tw;
    _th = copy._th;

    for (int i = 0; i < OVERLAY_MAX; i++)
    {
        if (copy._overlays[i])
            _overlays[i] = new Theme::Style::Overlay(*copy._overlays[i]);
        else
            _overlays[i] = NULL;
    }
}

Theme::Style::~Style()
{
    for (unsigned int i = 0; i < OVERLAY_MAX; i++)
    {
        SAFE_RELEASE(_overlays[i]);
    }
}

Theme* Theme::Style::getTheme() const
{
    return _theme;
}
    
const char* Theme::Style::getId() const
{
    return _id.c_str();
}

Theme::Style::Overlay* Theme::Style::getOverlay(OverlayType overlayType) const
{
    return _overlays[overlayType];
}

void Theme::Style::setMargin(float top, float bottom, float left, float right)
{
    _margin.top = top;
    _margin.bottom = bottom;
    _margin.left = left;
    _margin.right = right;
}

const Theme::Margin& Theme::Style::getMargin() const
{
    return _margin;
}

void Theme::Style::setPadding(float top, float bottom, float left, float right)
{
    _padding.top = top;
    _padding.bottom = bottom;
    _padding.left = left;
    _padding.right = right;
}

const Theme::Padding& Theme::Style::getPadding() const
{
    return _padding;
}
    
/*************************
 * Theme::Style::Overlay *
 *************************/
Theme::Style::Overlay* Theme::Style::Overlay::create()
{
    Overlay* overlay = new Overlay();
    return overlay;
}

Theme::Style::Overlay::Overlay()
    : _skin(NULL), _cursor(NULL), _imageList(NULL), _font(NULL),
    _fontSize(0), _alignment(Font::ALIGN_TOP_LEFT), _textRightToLeft(false), _textColor(Vector4::one()), _opacity(1.0f)
{
}

Theme::Style::Overlay::Overlay(const Overlay& copy) : _skin(NULL), _cursor(NULL), _imageList(NULL), _font(NULL)
{
    if (copy._skin)
    {
        _skin = new Skin(*copy._skin);
    }
    if (copy._cursor)
    {
        _cursor = new ThemeImage(*copy._cursor);
    }
    if (copy._imageList)
    {
        _imageList = new ImageList(*copy._imageList);
    }

    _font = copy._font;
    _fontSize = copy._fontSize;
    _alignment = copy._alignment;
    _textRightToLeft = copy._textRightToLeft;
    _textColor = Vector4(copy._textColor);
    _opacity = copy._opacity;

    if (_font)
    {
        _font->addRef();
    }
}

Theme::Style::Overlay::~Overlay()
{
    SAFE_RELEASE(_skin);
    SAFE_RELEASE(_imageList);
    SAFE_RELEASE(_cursor);
    SAFE_RELEASE(_font);
}

float Theme::Style::Overlay::getOpacity() const
{
    return _opacity;
}

void Theme::Style::Overlay::setOpacity(float opacity)
{
    _opacity = opacity;
}

void Theme::Style::Overlay::setBorder(float top, float bottom, float left, float right)
{
    if (_skin)
    {
        _skin->_border.top = top;
        _skin->_border.bottom = bottom;
        _skin->_border.left = left;
        _skin->_border.right = right;
    }
}

const Theme::Border& Theme::Style::Overlay::getBorder() const
{
    if (_skin)
    {
        return _skin->getBorder();
    }
    else
    {
        return Theme::Border::empty();
    }
}

void Theme::Style::Overlay::setSkinColor(const Vector4& color)
{
    if (_skin)
    {
        _skin->_color.set(color);
    }
}

const Vector4& Theme::Style::Overlay::getSkinColor() const
{
    if (_skin)
    {
        return _skin->getColor();
    }

    return Vector4::one();
}

void Theme::Style::Overlay::setSkinRegion(const Rectangle& region, float tw, float th)
{
    GP_ASSERT(_skin);
    _skin->setRegion(region, tw, th);
}

const Rectangle& Theme::Style::Overlay::getSkinRegion() const
{
    if (_skin)
    {
        return _skin->getRegion();
    }

    return Rectangle::empty();
}

const Theme::UVs& Theme::Style::Overlay::getSkinUVs(Theme::Skin::SkinArea area) const
{
    if (_skin)
    {
        return _skin->_uvs[area];
    }

    return UVs::empty();
}

Font* Theme::Style::Overlay::getFont() const
{
    return _font;
}

void Theme::Style::Overlay::setFont(Font* font)
{
    if (_font != font)
    {
        SAFE_RELEASE(_font);

        _font = font;

        if (_font)
        {
            _font->addRef();
        }
    }
}

unsigned int Theme::Style::Overlay::getFontSize() const
{
    return _fontSize;
}

void Theme::Style::Overlay::setFontSize(unsigned int fontSize)
{
    _fontSize = fontSize;
}

Font::Justify Theme::Style::Overlay::getTextAlignment() const
{
    return _alignment;
}

void Theme::Style::Overlay::setTextAlignment(Font::Justify alignment)
{
    _alignment = alignment;
}

bool Theme::Style::Overlay::getTextRightToLeft() const
{
    return _textRightToLeft;
}

void Theme::Style::Overlay::setTextRightToLeft(bool rightToLeft)
{
    _textRightToLeft = rightToLeft;
}

const Vector4& Theme::Style::Overlay::getTextColor() const
{
    return _textColor;
}

void Theme::Style::Overlay::setTextColor(const Vector4& color)
{
    _textColor = color;
}

const Rectangle& Theme::Style::Overlay::getImageRegion(const char* id) const
{
    if (!_imageList)
    {
        return Rectangle::empty();
    }

    ThemeImage* image = _imageList->getImage(id);
    if (image)
    {
        return image->getRegion();
    }
    else
    {
        return Rectangle::empty();
    }
}
    
void Theme::Style::Overlay::setImageRegion(const char* id, const Rectangle& region, float tw, float th)
{
    GP_ASSERT(_imageList);
    ThemeImage* image = _imageList->getImage(id);
    GP_ASSERT(image);
    image->_region.set(region);
    generateUVs(tw, th, region.x, region.y, region.width, region.height, &(image->_uvs));
}

const Vector4& Theme::Style::Overlay::getImageColor(const char* id) const
{
    GP_ASSERT(_imageList);
    ThemeImage* image = _imageList->getImage(id);
    if (image)
    {
        return image->getColor();
    }
    else
    {
        return Vector4::zero();
    }
}

void Theme::Style::Overlay::setImageColor(const char* id, const Vector4& color)
{
    GP_ASSERT(_imageList);
    ThemeImage* image = _imageList->getImage(id);
    GP_ASSERT(image);
    image->_color.set(color);
}

const Theme::UVs& Theme::Style::Overlay::getImageUVs(const char* id) const
{
    GP_ASSERT(_imageList);
    ThemeImage* image = _imageList->getImage(id);
    if (image)
    {
        return image->getUVs();
    }
    else
    {
        return UVs::empty();
    }
}

const Rectangle& Theme::Style::Overlay::getCursorRegion() const
{
    if (_cursor)
    {
        return _cursor->getRegion();
    }
    else
    {
        return Rectangle::empty();
    }
}
    
void Theme::Style::Overlay::setCursorRegion(const Rectangle& region, float tw, float th)
{
    GP_ASSERT(_cursor);
    _cursor->_region.set(region);
    generateUVs(tw, th, region.x, region.y, region.width, region.height, &(_cursor->_uvs));
}

const Vector4& Theme::Style::Overlay::getCursorColor() const
{
    if (_cursor)
    {
        return _cursor->getColor();
    }
    else
    {
        return Vector4::zero();
    }
}

void Theme::Style::Overlay::setCursorColor(const Vector4& color)
{
    GP_ASSERT(_cursor);
    _cursor->_color.set(color);
}

const Theme::UVs& Theme::Style::Overlay::getCursorUVs() const
{
    if (_cursor)
    {
        return _cursor->getUVs();
    }
    else
    {
        return UVs::empty();
    }
}

void Theme::Style::Overlay::setSkin(Skin* skin)
{
    if (_skin != skin)
    {
        SAFE_RELEASE(_skin);
        _skin = skin;

        if (skin)
        {
            skin->addRef();
        }
    }
}

Theme::Skin* Theme::Style::Overlay::getSkin() const
{
    return _skin;
}

void Theme::Style::Overlay::setCursor(ThemeImage* cursor)
{
    if (_cursor != cursor)
    {
        SAFE_RELEASE(_cursor);
        _cursor = cursor;

        if (cursor)
        {
            cursor->addRef();
        }
    }
}

Theme::ThemeImage* Theme::Style::Overlay::getCursor() const
{
    return _cursor;
}
            
void Theme::Style::Overlay::setImageList(ImageList* imageList)
{
    if (_imageList != imageList)
    {
        SAFE_RELEASE(_imageList);
        _imageList = imageList;

        if (imageList)
        {
            imageList->addRef();
        }
    }
}
    
Theme::ImageList* Theme::Style::Overlay::getImageList() const
{
    return _imageList;
}

// Implementation of AnimationHandler
unsigned int Theme::Style::Overlay::getAnimationPropertyComponentCount(int propertyId) const
{
    switch(propertyId)
    {
    case Theme::Style::Overlay::ANIMATE_OPACITY:
        return 1;
    default:
        return -1;
    }
}

void Theme::Style::Overlay::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    GP_ASSERT(value);

    switch(propertyId)
    {
    case ANIMATE_OPACITY:
        value->setFloat(0, _opacity);
        break;
    default:
        break;
    }
}

void Theme::Style::Overlay::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    GP_ASSERT(value);

    switch(propertyId)
    {
        case ANIMATE_OPACITY:
        {
            _opacity = Curve::lerp(blendWeight, _opacity, value->getFloat(0));
            break;
        }
        default:
            break;
    }
}

}