#include "Base.h"
#include "Theme.h"

namespace gameplay
{
    static std::vector<Theme*> __themeCache;

    Theme::Theme()
    {
    }

    Theme::Theme(const Theme* theme)
    {
    }

    Theme::~Theme()
    {
        // Destroy all the cursors, styles and , fonts.
        for (unsigned int i = 0, count = _styles.size(); i < count; ++i)
        {
            Style* style = _styles[i];
            SAFE_DELETE(style);
        }

        for (unsigned int i = 0, count = _images.size(); i < count; ++i)
        {
            Image* image = _images[i];
            SAFE_RELEASE(image);
        }

        for (unsigned int i = 0, count = _imageLists.size(); i < count; ++i)
        {
            ImageList* imageList = _imageLists[i];
            SAFE_RELEASE(imageList);
        }

        for (unsigned int i = 0, count = _skins.size(); i < count; ++i)
        {
            Skin* skin = _skins[i];
            SAFE_RELEASE(skin);
        }

        SAFE_DELETE(_spriteBatch);
        SAFE_RELEASE(_texture);

        // Remove ourself from the theme cache.
        std::vector<Theme*>::iterator itr = find(__themeCache.begin(), __themeCache.end(), this);
        if (itr != __themeCache.end())
        {
            __themeCache.erase(itr);
        }
    }

    Theme* Theme::create(const char* path)
    {
        assert(path);

        // Search theme cache first.
        for (unsigned int i = 0, count = __themeCache.size(); i < count; ++i)
        {
            Theme* t = __themeCache[i];
            if (t->_path == path)
            {
                // Found a match.
                t->addRef();

                return t;
            }
        }

        // Load theme properties from file path.
        Properties* properties = Properties::create(path);
        assert(properties);
        if (properties == NULL)
        {
            return NULL;
        }

        // Check if the Properties is valid and has a valid namespace.
        Properties* themeProperties = properties->getNextNamespace();
        assert(themeProperties);
        if (!themeProperties || !(strcmp(themeProperties->getNamespace(), "theme") == 0))
        {
            SAFE_DELETE(properties);
            return NULL;
        }

        // Create a new theme.
        Theme* theme = new Theme();
        theme->_path = path;
        
        // Parse the Properties object and set up the theme.
        const char* textureFile = themeProperties->getString("texture");
        theme->_texture = Texture::create(textureFile, false);
        theme->_spriteBatch = SpriteBatch::create(theme->_texture);

        float tw = 1.0f / theme->_texture->getWidth();
        float th = 1.0f / theme->_texture->getHeight();

        Properties* space = themeProperties->getNextNamespace();
        while (space != NULL)
        {
            // First load all cursors, checkboxes etc. that can be referred to by styles.
            const char* spacename = space->getNamespace();
            
            if (strcmp(spacename, "image") == 0)
            {
                theme->_images.push_back(Image::create(tw, th, space, Vector4::one()));
            }
            else if (strcmp(spacename, "imageList") == 0)
            {
                theme->_imageLists.push_back(ImageList::create(tw, th, space));
            }
            else if (strcmp(spacename, "skin") == 0)
            {
                Theme::Border border;
                Properties* innerSpace = space->getNextNamespace();
                if (innerSpace)
                {
                    const char* innerSpacename = innerSpace->getNamespace();
                    if (strcmp(innerSpacename, "border") == 0)
                    {
                        border.top = innerSpace->getFloat("top");
                        border.bottom = innerSpace->getFloat("bottom");
                        border.left = innerSpace->getFloat("left");
                        border.right = innerSpace->getFloat("right");
                    }
                }

                Vector4 regionVector;
                space->getVector4("region", &regionVector);
                const Rectangle region(regionVector.x, regionVector.y, regionVector.z, regionVector.w);

                Vector4 color(1, 1, 1, 1);
                if (space->exists("color"))
                {
                    space->getColor("color", &color);
                }

                Skin* skin = Skin::create(space->getId(), tw, th, region, border, color);
                theme->_skins.push_back(skin);
            }

            space = themeProperties->getNextNamespace();
        }

        themeProperties->rewind();
        space = themeProperties->getNextNamespace();
        while (space != NULL)
        {
            const char* spacename = space->getNamespace();
            if (strcmp(spacename, "style") == 0)
            {
                // Each style contains up to MAX_OVERLAYS overlays,
                // as well as Border and Padding namespaces.
                Theme::Margin margin;
                Theme::Padding padding;
                Theme::Style::Overlay* normal = NULL;
                Theme::Style::Overlay* focus = NULL;
                Theme::Style::Overlay* active = NULL;
                Theme::Style::Overlay* disabled = NULL;

                // Need to load OVERLAY_NORMAL first so that the other overlays can inherit from it.
                Properties* innerSpace = space->getNextNamespace();
                while (innerSpace != NULL)
                {
                    const char* innerSpacename = innerSpace->getNamespace();
                    if (strcmp(innerSpacename, "stateNormal") == 0)
                    {
                        Vector4 textColor(0, 0, 0, 1);
                        if (innerSpace->exists("textColor"))
                        {
                            innerSpace->getColor("textColor", &textColor);
                        }

                        const char* fontPath = innerSpace->getString("font");
                        Font* font = NULL;
                        if (fontPath)
                        {
                            font = Font::create(fontPath);
                        }
                        unsigned int fontSize = innerSpace->getInt("fontSize");
                        const char* alignmentString = innerSpace->getString("alignment");
                        Font::Justify alignment = Font::ALIGN_TOP_LEFT;
                        if (alignmentString)
                        {
                            alignment = Font::getJustifyFromString(alignmentString);
                        }
                        bool rightToLeft = innerSpace->getBool("rightToLeft");

                        float opacity = 1.0f;
                        if (innerSpace->exists("opacity"))
                        {
                            opacity = innerSpace->getFloat("opacity");
                        }

                        ImageList* imageList = NULL;
                        Image* cursor = NULL;
                        Skin* skin = NULL;
                        theme->lookUpSprites(innerSpace, &imageList, &cursor, &skin);

                        normal = Theme::Style::Overlay::create();
                        normal->setSkin(skin);
                        normal->setCursor(cursor);
                        normal->setImageList(imageList);
                        normal->setTextColor(textColor);
                        normal->setFont(font);
                        normal->setFontSize(fontSize);
                        normal->setTextAlignment(alignment);
                        normal->setTextRightToLeft(rightToLeft);
                        normal->setOpacity(opacity);

                        theme->_fonts.insert(font);

                        if (font) font->release();

                        // Done with this pass.
                        break;
                    }

                    innerSpace = space->getNextNamespace();
                }

                // At least the OVERLAY_NORMAL is required.
                assert(normal);

                space->rewind();
                innerSpace = space->getNextNamespace();
                while (innerSpace != NULL)
                {
                    const char* innerSpacename = innerSpace->getNamespace();
                    if (strcmp(innerSpacename, "margin") == 0)
                    {
                        margin.top = innerSpace->getFloat("top");
                        margin.bottom = innerSpace->getFloat("bottom");
                        margin.left = innerSpace->getFloat("left");
                        margin.right = innerSpace->getFloat("right");
                    }
                    else if (strcmp(innerSpacename, "padding") == 0)
                    {
                        padding.top = innerSpace->getFloat("top");
                        padding.bottom = innerSpace->getFloat("bottom");
                        padding.left = innerSpace->getFloat("left");
                        padding.right = innerSpace->getFloat("right");
                    }
                    else if (strcmp(innerSpacename, "stateNormal") != 0)
                    {
                        // Either OVERLAY_FOCUS or OVERLAY_ACTIVE.
                        // If a property isn't specified, it inherits from OVERLAY_NORMAL.
                        Vector4 textColor;
                        if (!innerSpace->getColor("textColor", &textColor))
                        {
                            textColor.set(normal->getTextColor());
                        }

                        const char* fontPath = innerSpace->getString("font");
                        Font* font = NULL;
                        if (fontPath)
                        {
                            font = Font::create(fontPath);
                        }
                        if (!font)
                        {
                            font = normal->getFont();
                        }

                        unsigned int fontSize;
                        if (innerSpace->exists("fontSize"))
                        {
                            fontSize = innerSpace->getInt("fontSize");
                        }
                        else
                        {
                            fontSize = normal->getFontSize();
                        }

                        const char* alignmentString = innerSpace->getString("alignment");
                        Font::Justify alignment;
                        if (alignmentString)
                        {
                            alignment = Font::getJustifyFromString(alignmentString);
                        }
                        else
                        {
                            alignment = normal->getTextAlignment();
                        }

                        bool rightToLeft;
                        if (innerSpace->exists("rightToLeft"))
                        {
                            rightToLeft = innerSpace->getBool("rightToLeft");
                        }
                        else
                        {
                            rightToLeft = normal->getTextRightToLeft();
                        }

                        float opacity;
                        if (innerSpace->exists("opacity"))
                        {
                            opacity = innerSpace->getFloat("opacity");
                        }
                        else
                        {
                            opacity = normal->getOpacity();
                        }

                        ImageList* imageList = NULL;
                        Image* cursor = NULL;
                        Skin* skin = NULL;
                        theme->lookUpSprites(innerSpace, &imageList, &cursor, &skin);

                        if (!imageList)
                        {
                            imageList = normal->getImageList();
                        }

                        if (!cursor)
                        {
                            cursor = normal->getCursor();
                        }
                        
                        if (!skin)
                        {
                            skin = normal->getSkin();
                        }

                        if (strcmp(innerSpacename, "stateFocus") == 0)
                        {
                            focus = Theme::Style::Overlay::create();
                            focus->setSkin(skin);
                            focus->setCursor(cursor);
                            focus->setImageList(imageList);
                            focus->setTextColor(textColor);
                            focus->setFont(font);
                            focus->setFontSize(fontSize);
                            focus->setTextAlignment(alignment);
                            focus->setTextRightToLeft(rightToLeft);
                            focus->setOpacity(opacity);

                            theme->_fonts.insert(font);
                        }
                        else if (strcmp(innerSpacename, "stateActive") == 0)
                        {
                            active = Theme::Style::Overlay::create();
                            active->setSkin(skin);
                            active->setCursor(cursor);
                            active->setImageList(imageList);
                            active->setTextColor(textColor);
                            active->setFont(font);
                            active->setFontSize(fontSize);
                            active->setTextAlignment(alignment);
                            active->setTextRightToLeft(rightToLeft);
                            active->setOpacity(opacity);

                            theme->_fonts.insert(font);
                        }
                        else if (strcmp(innerSpacename, "stateDisabled") == 0)
                        {
                            disabled = Theme::Style::Overlay::create();
                            disabled->setSkin(skin);
                            disabled->setCursor(cursor);
                            disabled->setImageList(imageList);
                            disabled->setTextColor(textColor);
                            disabled->setFont(font);
                            disabled->setFontSize(fontSize);
                            disabled->setTextAlignment(alignment);
                            disabled->setTextRightToLeft(rightToLeft);
                            disabled->setOpacity(opacity);

                            theme->_fonts.insert(font);
                        }
                    }

                    innerSpace = space->getNextNamespace();
                }
                
                if (!focus)
                {
                    focus = normal;
                    focus->addRef();
                }

                if (!active)
                {
                    active = normal;
                    active->addRef();
                }

                if (!disabled)
                {
                    disabled = normal;
                    disabled->addRef();
                }

                Theme::Style* s = new Theme::Style(space->getId(), tw, th, margin, padding, normal, focus, active, disabled);
                theme->_styles.push_back(s);
            }

            space = themeProperties->getNextNamespace();
        }

        // Add this theme to the cache.
        __themeCache.push_back(theme);

        SAFE_DELETE(properties);

        return theme;
    }

    Theme::Style* Theme::getStyle(const char* name) const
    {
        for (unsigned int i = 0, count = _styles.size(); i < count; ++i)
        {
            if (strcmp(name, _styles[i]->getId()) == 0)
            {
                return _styles[i];
            }
        }

        return NULL;
    }

    void Theme::setProjectionMatrix(const Matrix& matrix)
    {
        _spriteBatch->setProjectionMatrix(matrix);

        // Set the matrix on each Font used by the style.
        std::set<Font*>::const_iterator it;
        for (it = _fonts.begin(); it != _fonts.end(); ++it)
        {
            (*it)->getSpriteBatch()->setProjectionMatrix(matrix);
        }
    }

    SpriteBatch* Theme::getSpriteBatch() const
    {
        return _spriteBatch;
    }

    /**************
     * Theme::UVs *
     **************/
    Theme::UVs::UVs()
        : u1(0), v1(0), u2(0), v2(0)
    {
    }

    Theme::UVs::UVs(float u1, float v1, float u2, float v2)
        : u1(u1), v1(v1), u2(u2), v2(v2)
    {
    }

    const Theme::UVs& Theme::UVs::empty()
    {
        static UVs empty(0, 0, 0, 0);
        return empty;
    }

    /**********************
     * Theme::SideRegions *
     **********************/
    const Theme::SideRegions& Theme::SideRegions::empty()
    {
        static SideRegions empty;
        return empty;
    }

    /****************
     * Theme::Image *
     ****************/
    Theme::Image::Image(float tw, float th, const Rectangle& region, const Vector4& color)
        : _region(region), _color(color)
    {
        generateUVs(tw, th, region.x, region.y, region.width, region.height, &_uvs);
    }

    Theme::Image::~Image()
    {
    }

    Theme::Image* Theme::Image::create(float tw, float th, Properties* properties, const Vector4& defaultColor)
    {
        Vector4 regionVector;                
        properties->getVector4("region", &regionVector);
        const Rectangle region(regionVector.x, regionVector.y, regionVector.z, regionVector.w);

        Vector4 color;
        if (properties->exists("color"))
        {
            properties->getColor("color", &color);
        }
        else
        {
            color.set(defaultColor);
        }

        Image* image = new Image(tw, th, region, color);
        const char* id = properties->getId();
        if (id)
        {
            image->_id = id;
        }

        return image;
    }

    const char* Theme::Image::getId() const
    {
        return _id.c_str();
    }

    const Theme::UVs& Theme::Image::getUVs() const
    {
        return _uvs;
    }

    const Rectangle& Theme::Image::getRegion() const
    {
        return _region;
    }

    const Vector4& Theme::Image::getColor() const
    {
        return _color;
    }

    /********************
     * Theme::ImageList *
     ********************/
    Theme::ImageList::ImageList(const Vector4& color) : _color(color)
    {
    }

    Theme::ImageList::ImageList(const ImageList& copy)
    {
        _id = copy._id;
        _color = copy._color;

        std::vector<Image*>::const_iterator it;
        for (it = copy._images.begin(); it != copy._images.end(); it++)
        {
            Image* image = *it;
            _images.push_back(new Image(*image));
        }
    }

    Theme::ImageList::~ImageList()
    {
        std::vector<Image*>::const_iterator it;
        for (it = _images.begin(); it != _images.end(); it++)
        {
            Image* image = *it;
            SAFE_RELEASE(image);
        }
    }

    Theme::ImageList* Theme::ImageList::create(float tw, float th, Properties* properties)
    {
        Vector4 color(1, 1, 1, 1);
        if (properties->exists("color"))
        {
            properties->getColor("color", &color);
        }

        ImageList* imageList = new ImageList(color);

        const char* id = properties->getId();
        if (id)
        {
            imageList->_id = id;
        }

        Properties* space = properties->getNextNamespace();
        while (space != NULL)
        {
            Image* image = Image::create(tw, th, space, color);
            imageList->_images.push_back(image);
            space = properties->getNextNamespace();
        }

        return imageList;
    }

    const char* Theme::ImageList::getId() const
    {
        return _id.c_str();
    }

    Theme::Image* Theme::ImageList::getImage(const char* imageId) const
    {
        std::vector<Image*>::const_iterator it;
        for (it = _images.begin(); it != _images.end(); it++)
        {
            Image* image = *it;
            if (strcmp(image->getId(), imageId) == 0)
            {
                return image;
            }
        }

        return NULL;
    }

    /***************
     * Theme::Skin *
     ***************/
    Theme::Skin* Theme::Skin::create(const char* id, float tw, float th, const Rectangle& region, const Theme::Border& border, const Vector4& color)
    {
        Skin* skin = new Skin(tw, th, region, border, color);

        if (id)
        {
            skin->_id = id;
        }

        return skin;
    }

    Theme::Skin::Skin(float tw, float th, const Rectangle& region, const Theme::Border& border, const Vector4& color)
        : _border(border), _color(color), _region(region)
    {
        setRegion(region, tw, th);
    }

    Theme::Skin::~Skin()
    {
    }

    const char* Theme::Skin::getId() const
    {
        return _id.c_str();
    }

    const Theme::Border& Theme::Skin::getBorder() const
    {
        return _border;
    }

    const Rectangle& Theme::Skin::getRegion() const
    {
        return _region;
    }

    void Theme::Skin::setRegion(const Rectangle& region, float tw, float th)
    {
        // Can calculate all measurements in advance.
        float leftEdge = region.x * tw;
        float rightEdge = (region.x + region.width) * tw;
        float leftBorder = (region.x + _border.left) * tw;
        float rightBorder = (region.x + region.width - _border.right) * tw;

        float topEdge = 1.0f - (region.y * th);
        float bottomEdge = 1.0f - ((region.y + region.height) * th);
        float topBorder = 1.0f - ((region.y + _border.top) * th);
        float bottomBorder = 1.0f - ((region.y + region.height - _border.bottom) * th);

        // There are 9 sets of UVs to set.
        _uvs[TOP_LEFT].u1 = leftEdge;
        _uvs[TOP_LEFT].v1 = topEdge;
        _uvs[TOP_LEFT].u2 = leftBorder;
        _uvs[TOP_LEFT].v2 = topBorder;

        _uvs[TOP].u1 = leftBorder;
        _uvs[TOP].v1 = topEdge;
        _uvs[TOP].u2 = rightBorder;
        _uvs[TOP].v2 = topBorder;

        _uvs[TOP_RIGHT].u1 = rightBorder;
        _uvs[TOP_RIGHT].v1 = topEdge;
        _uvs[TOP_RIGHT].u2 = rightEdge;
        _uvs[TOP_RIGHT].v2 = topBorder;

        _uvs[LEFT].u1 = leftEdge;
        _uvs[LEFT].v1 = topBorder;
        _uvs[LEFT].u2 = leftBorder;
        _uvs[LEFT].v2 = bottomBorder;

        _uvs[CENTER].u1 = leftBorder;
        _uvs[CENTER].v1 = topBorder;
        _uvs[CENTER].u2 = rightBorder;
        _uvs[CENTER].v2 = bottomBorder;

        _uvs[RIGHT].u1 = rightBorder;
        _uvs[RIGHT].v1 = topBorder;
        _uvs[RIGHT].u2 = rightEdge;
        _uvs[RIGHT].v2 = bottomBorder;

        _uvs[BOTTOM_LEFT].u1 = leftEdge;
        _uvs[BOTTOM_LEFT].v1 = bottomBorder;
        _uvs[BOTTOM_LEFT].u2 = leftBorder;
        _uvs[BOTTOM_LEFT].v2 = bottomEdge;

        _uvs[BOTTOM].u1 = leftBorder;
        _uvs[BOTTOM].v1 = bottomBorder;
        _uvs[BOTTOM].u2 = rightBorder;
        _uvs[BOTTOM].v2 = bottomEdge;

        _uvs[BOTTOM_RIGHT].u1 = rightBorder;
        _uvs[BOTTOM_RIGHT].v1 = bottomBorder;
        _uvs[BOTTOM_RIGHT].u2 = rightEdge;
        _uvs[BOTTOM_RIGHT].v2 = bottomEdge;
    }

    const Theme::UVs& Theme::Skin::getUVs(SkinArea area) const
    {
        return _uvs[area];
    }

    const Vector4& Theme::Skin::getColor() const
    {
        return _color;
    }

    /****************
     * Theme::Style *
     ****************/
    Theme::Style::Style(const char* id, float tw, float th,
            const Theme::Margin& margin, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active, Theme::Style::Overlay* disabled)
        : _id(id), _tw(tw), _th(th), _margin(margin), _padding(padding)
    {
        _overlays[OVERLAY_NORMAL] = normal;
        _overlays[OVERLAY_FOCUS] = focus;
        _overlays[OVERLAY_ACTIVE] = active;
        _overlays[OVERLAY_DISABLED] = disabled;
    }

    Theme::Style::Style(const Style& copy)
    {
        _id = copy._id;
        _margin = copy._margin;
        _padding = copy._padding;
        _tw = copy._tw;
        _th = copy._th;

        for (int i = 0; i < MAX_OVERLAYS; i++)
        {
            _overlays[i] = new Theme::Style::Overlay(*copy._overlays[i]);
        }
    }

    Theme::Style::~Style()
    {
        for (int i = 0; i < MAX_OVERLAYS; i++)
        {
            SAFE_RELEASE(_overlays[i]);
        }
    }
    
    const char* Theme::Style::getId() const
    {
        return _id.data();
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

    Theme::Style::Overlay::Overlay() : _skin(NULL), _cursor(NULL), _imageList(NULL), _font(NULL)
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
            _cursor = new Image(*copy._cursor);
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
        assert(_skin);
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
        Image* image = _imageList->getImage(id);
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
        Image* image = _imageList->getImage(id);
        assert(image);
        image->_region.set(region);
        generateUVs(tw, th, region.x, region.y, region.width, region.height, &(image->_uvs));
    }

    const Vector4& Theme::Style::Overlay::getImageColor(const char* id) const
    {
        Image* image = _imageList->getImage(id);
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
        Image* image = _imageList->getImage(id);
        assert(image);
        image->_color.set(color);
    }

    const Theme::UVs& Theme::Style::Overlay::getImageUVs(const char* id) const
    {
        Image* image = _imageList->getImage(id);
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
        assert(_cursor);
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
        assert(_cursor);
        _cursor->_color.set(color);
    }

    const Theme::UVs Theme::Style::Overlay::getCursorUVs() const
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

    void Theme::Style::Overlay::setCursor(Image* cursor)
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

    Theme::Image* Theme::Style::Overlay::getCursor() const
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
        switch(propertyId)
        {
            case ANIMATE_OPACITY:
            {
                float opacity = value->getFloat(0);
                if ((_animationPropertyBitFlag & ANIMATION_OPACITY_BIT) != ANIMATION_OPACITY_BIT)
                {
                    _animationPropertyBitFlag |= ANIMATION_OPACITY_BIT;
                }
                else
                {
                    opacity = Curve::lerp(blendWeight, _opacity, opacity);
                }
                _opacity = opacity;
                break;
            }
            default:
                break;
        }
    }
    
    /**
     * Theme utility methods.
     */
    void Theme::generateUVs(float tw, float th, float x, float y, float width, float height, UVs* uvs)
    {
        uvs->u1 = x * tw;
        uvs->u2 = (x + width) * tw;
        uvs->v1 = 1.0f - (y * th);
        uvs->v2 = 1.0f - ((y + height) * th);
    }

    void Theme::lookUpSprites(const Properties* overlaySpace, ImageList** imageList, Image** cursor, Skin** Skin)
    {
        const char* imageListString = overlaySpace->getString("imageList");
        if (imageListString)
        {
            for (unsigned int i = 0; i < _imageLists.size(); ++i)
            {
                if (strcmp(_imageLists[i]->getId(), imageListString) == 0)
                {
                    *imageList = _imageLists[i];
                    break;
                }
            }
        }

        const char* cursorString = overlaySpace->getString("cursor");
        if (cursorString)
        {
            for (unsigned int i = 0; i < _images.size(); ++i)
            {
                if (strcmp(_images[i]->getId(), cursorString) == 0)
                {
                    *cursor = _images[i];
                    break;
                }
            }
        }

        const char* skinString = overlaySpace->getString("skin");
        if (skinString)
        {
            for (unsigned int i = 0; i < _skins.size(); ++i)
            {
                if (strcmp(_skins[i]->getId(), skinString) == 0)
                {
                    *Skin = _skins[i];
                    break;
                }
            }
        }
    }
}
