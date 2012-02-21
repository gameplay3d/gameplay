/*
 * Theme.cpp
 */

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

        for (unsigned int i = 0, count = _cursors.size(); i < count; ++i)
        {
            Cursor* cursor = _cursors[i];
            SAFE_RELEASE(cursor);
        }

        for (unsigned int i = 0, count = _icons.size(); i < count; ++i)
        {
            Icon* icon = _icons[i];
            SAFE_RELEASE(icon);
        }

        for (unsigned int i = 0, count = _sliders.size(); i < count; ++i)
        {
            SliderIcon* slider = _sliders[i];
            SAFE_RELEASE(slider);
        }

        for (unsigned int i = 0, count = _containers.size(); i < count; ++i)
        {
            ContainerRegion* container = _containers[i];
            SAFE_RELEASE(container);
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
        theme->_texture = Texture::create(textureFile, true);
        theme->_spriteBatch = SpriteBatch::create(theme->_texture);

        Properties* space = themeProperties->getNextNamespace();
        while (space != NULL)
        {
            // First load all cursors, checkboxes etc. that can be referred to by styles.
            const char* spacename = space->getNamespace();
            if (strcmp(spacename, "cursor") == 0)
            {
                Vector4 regionVector;                
                space->getVector4("region", &regionVector);
                const Rectangle region(regionVector.x, regionVector.y, regionVector.z, regionVector.w);

                Vector4 color(1, 1, 1, 1);
                if (space->exists("color"))
                {
                    space->getColor("color", &color);
                }

                Theme::Cursor* c = Theme::Cursor::create(space->getId(), *theme->_texture, region, color);
                theme->_cursors.push_back(c);
            }
            else if (strcmp(spacename, "icon") == 0)
            {
                Vector2 offVec;
                Vector2 onVec;
                Vector2 size;
                space->getVector2("offPosition", &offVec);
                space->getVector2("onPosition", &onVec);
                space->getVector2("size", &size);
                
                Vector4 color(1, 1, 1, 1);
                if (space->exists("color"))
                {
                    space->getColor("color", &color);
                }

                Icon* icon = Icon::create(space->getId(), *theme->_texture, size, offVec, onVec, color);
                theme->_icons.push_back(icon);
            }
            else if (strcmp(spacename, "slider") == 0)
            {
                Vector4 minCapRegion;
                Vector4 maxCapRegion;
                Vector4 trackRegion;
                Vector4 markerRegion;
                space->getVector4("leftCapRegion", &minCapRegion);
                space->getVector4("rightCapRegion", &maxCapRegion);
                space->getVector4("trackRegion", &trackRegion);
                space->getVector4("markerRegion", &markerRegion);
                
                Vector4 color(1, 1, 1, 1);
                if (space->exists("color"))
                {
                    space->getColor("color", &color);
                }

                SliderIcon* sliderIcon = SliderIcon::create(space->getId(), *theme->_texture, minCapRegion, maxCapRegion, markerRegion, trackRegion, color);
                theme->_sliders.push_back(sliderIcon);
            }
            else if (strcmp(spacename, "cursor") == 0)
            {
                Vector4 regionVector;
                space->getVector4("region", &regionVector);
                const Rectangle region(regionVector.x, regionVector.y, regionVector.z, regionVector.w);

                Vector4 color(1, 1, 1, 1);
                if (space->exists("color"))
                {
                    space->getColor("color", &color);
                }

                Cursor* cursor = Cursor::create(space->getId(), *theme->_texture, region, color);
                theme->_cursors.push_back(cursor);
            }
            else if (strcmp(spacename, "container") == 0)
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

                ContainerRegion* container = ContainerRegion::create(space->getId(), *theme->_texture, region, border, color);
                theme->_containers.push_back(container);
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

                // Need to load OVERLAY_NORMAL first so that the other overlays can inherit from it.
                Properties* innerSpace = space->getNextNamespace();
                while (innerSpace != NULL)
                {
                    const char* innerSpacename = innerSpace->getNamespace();
                    if (strcmp(innerSpacename, "normal") == 0)
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

                        Icon* checkBoxIcon = NULL;
                        Icon* radioButtonIcon = NULL;
                        SliderIcon* sliderIcon = NULL;
                        Cursor* textCursor = NULL;
                        Cursor* mouseCursor = NULL;
                        ContainerRegion* containerRegion = NULL;
                        theme->lookUpSprites(innerSpace, &checkBoxIcon, &radioButtonIcon, &sliderIcon, &textCursor, &mouseCursor, &containerRegion);

                        normal = Theme::Style::Overlay::create();
                        normal->setContainerRegion(containerRegion);
                        normal->setTextCursor(textCursor);
                        normal->setMouseCursor(mouseCursor);
                        normal->setCheckBoxIcon(checkBoxIcon);
                        normal->setRadioButtonIcon(radioButtonIcon);
                        normal->setSliderIcon(sliderIcon);
                        normal->setTextColor(textColor);
                        normal->setFont(font);
                        normal->setFontSize(fontSize);
                        normal->setTextAlignment(alignment);
                        normal->setTextRightToLeft(rightToLeft);

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
                    else if (strcmp(innerSpacename, "normal") != 0)
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


                        Icon* checkBoxIcon = NULL;
                        Icon* radioButtonIcon = NULL;
                        SliderIcon* sliderIcon = NULL;
                        Cursor* textCursor = NULL;
                        Cursor* mouseCursor = NULL;
                        ContainerRegion* containerRegion = NULL;
                        theme->lookUpSprites(innerSpace, &checkBoxIcon, &radioButtonIcon, &sliderIcon, &textCursor, &mouseCursor, &containerRegion);

                        if (!checkBoxIcon)
                        {
                            checkBoxIcon = normal->getCheckBoxIcon();
                        }
                        
                        if (!radioButtonIcon)
                        {
                            radioButtonIcon = normal->getRadioButtonIcon();
                        }
                        
                        if (!sliderIcon)
                        {
                            sliderIcon = normal->getSliderIcon();
                        }

                        if (!textCursor)
                        {
                            textCursor = normal->getTextCursor();
                        }

                        if (!mouseCursor)
                        {
                            mouseCursor = normal->getMouseCursor();
                        }
                        
                        if (!containerRegion)
                        {
                            containerRegion = normal->getContainerRegion();
                        }

                        
                        if (strcmp(innerSpacename, "focus") == 0)
                        {
                            focus = Theme::Style::Overlay::create();
                            focus->setContainerRegion(containerRegion);
                            focus->setCheckBoxIcon(checkBoxIcon);
                            focus->setTextCursor(textCursor);
                            focus->setMouseCursor(mouseCursor);
                            focus->setCheckBoxIcon(checkBoxIcon);
                            focus->setRadioButtonIcon(radioButtonIcon);
                            focus->setSliderIcon(sliderIcon);
                            focus->setTextColor(textColor);
                            focus->setFont(font);
                            focus->setFontSize(fontSize);
                            focus->setTextAlignment(alignment);
                            focus->setTextRightToLeft(rightToLeft);

                            theme->_fonts.insert(font);
                        }
                        else if (strcmp(innerSpacename, "active") == 0)
                        {
                            active = Theme::Style::Overlay::create();
                            active->setContainerRegion(containerRegion);
                            active->setTextCursor(textCursor);
                            active->setMouseCursor(mouseCursor);
                            active->setCheckBoxIcon(checkBoxIcon);
                            active->setRadioButtonIcon(radioButtonIcon);
                            active->setSliderIcon(sliderIcon);
                            active->setTextColor(textColor);
                            active->setFont(font);
                            active->setFontSize(fontSize);
                            active->setTextAlignment(alignment);
                            active->setTextRightToLeft(rightToLeft);

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

                Theme::Style* s = new Theme::Style(space->getId(), margin, padding, normal, focus, active);
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

    /***************
     * Theme::Icon *
     ***************/
    Theme::Icon* Theme::Icon::create(const char* id, const Texture& texture, const Vector2& size,
                                     const Vector2& offPosition, const Vector2& onPosition, const Vector4& color)
    {
        Icon* icon = new Icon(texture, size, offPosition, onPosition, color);

        if (id)
        {
            icon->_id = id;
        }

        return icon;
    }

    Theme::Icon::Icon(const Texture& texture, const Vector2& size,
                      const Vector2& offPosition, const Vector2& onPosition, const Vector4& color)
                      : _size(size), _color(color)
    {
        generateUVs(texture, offPosition.x, offPosition.y, size.x, size.y, &_off);
        generateUVs(texture, onPosition.x, onPosition.y, size.x, size.y, &_on);
    }

    Theme::Icon::~Icon()
    {
    }

    const char* Theme::Icon::getId() const
    {
        return _id.c_str();
    }

    const Vector2& Theme::Icon::getSize() const
    {
        return _size;
    }

    const Vector4& Theme::Icon::getColor() const
    {
        return _color;
    }

    const Theme::UVs& Theme::Icon::getOffUVs() const
    {
        return _off;
    }

    const Theme::UVs& Theme::Icon::getOnUVs() const
    {
        return _on;
    }


    /*********************
     * Theme::SliderIcon *
     *********************/
    Theme::SliderIcon* Theme::SliderIcon::create(const char* id, const Texture& texture, const Vector4& minCapRegion,
            const Vector4& maxCapRegion, const Vector4& markerRegion, const Vector4& trackRegion, const Vector4& color)
    {
        SliderIcon* sliderIcon = new SliderIcon(texture, minCapRegion, maxCapRegion, markerRegion, trackRegion, color);

        if (id)
        {
            sliderIcon->_id = id;
        }

        return sliderIcon;
    }

    Theme::SliderIcon::SliderIcon(const Texture& texture, const Vector4& minCapRegion, const Vector4& maxCapRegion,
                                  const Vector4& markerRegion, const Vector4& trackRegion, const Vector4& color)
                                  : _color(color)
    {
        _minCapSize.set(minCapRegion.z, minCapRegion.w);
        _maxCapSize.set(maxCapRegion.z, maxCapRegion.w);
        _markerSize.set(markerRegion.z, markerRegion.w);
        _trackSize.set(trackRegion.z, trackRegion.w);

        generateUVs(texture, minCapRegion.x, minCapRegion.y, minCapRegion.z, minCapRegion.w, &_minCap);
        generateUVs(texture, maxCapRegion.x, maxCapRegion.y, maxCapRegion.z, maxCapRegion.w, &_maxCap);
        generateUVs(texture, markerRegion.x, markerRegion.y, markerRegion.z, markerRegion.w, &_marker);
        generateUVs(texture, trackRegion.x, trackRegion.y, trackRegion.z, trackRegion.w, &_track);
    }

    Theme::SliderIcon::~SliderIcon()
    {
    }

    const char* Theme::SliderIcon::getId() const
    {
        return _id.c_str();
    }

    const Theme::UVs& Theme::SliderIcon::getMinCapUVs() const
    {
        return _minCap;
    }

    const Theme::UVs& Theme::SliderIcon::getMaxCapUVs() const
    {
        return _maxCap;
    }
    
    const Theme::UVs& Theme::SliderIcon::getMarkerUVs() const
    {
        return _marker;
    }
    
    const Theme::UVs& Theme::SliderIcon::getTrackUVs() const
    {
        return _track;
    }

    const Vector2& Theme::SliderIcon::getMinCapSize() const
    {
        return _minCapSize;
    }

    const Vector2& Theme::SliderIcon::getMaxCapSize() const
    {
        return _maxCapSize;
    }

    const Vector2& Theme::SliderIcon::getMarkerSize() const
    {
        return _markerSize;
    }

    const Vector2& Theme::SliderIcon::getTrackSize() const
    {
        return _trackSize;
    }

    const Vector4& Theme::SliderIcon::getColor() const
    {
        return _color;
    }

    /*****************
     * Theme::Cursor *
     *****************/
    Theme::Cursor* Theme::Cursor::create(const char* id, const Texture& texture, const Rectangle& region, const Vector4& color)
    {
        Cursor* cursor = new Cursor(texture, region, color);
        
        if (id)
        {
            cursor->_id = id;
        }

        return cursor;
    }

    Theme::Cursor::Cursor(const Texture& texture, const Rectangle& region, const Vector4& color)
        : _color(color)
    {
        _size.set(region.width, region.height);
        generateUVs(texture, region.x, region.y, region.width, region.height, &_uvs);
    }

    Theme::Cursor::~Cursor()
    {
    }

    const char* Theme::Cursor::getId() const
    {
        return _id.data();
    }

    const Theme::UVs& Theme::Cursor::getUVs() const
    {
        return _uvs;
    }

    const Vector2& Theme::Cursor::getSize() const
    {
        return _size;
    }

    const Vector4& Theme::Cursor::getColor() const
    {
        return _color;
    }

    /**************************
     * Theme::ContainerRegion *
     **************************/
    Theme::ContainerRegion* Theme::ContainerRegion::create(const char* id, const Texture& texture, const Rectangle& region, const Theme::Border& border, const Vector4& color)
    {
        ContainerRegion* containerRegion = new ContainerRegion(texture, region, border, color);

        if (id)
        {
            containerRegion->_id = id;
        }

        return containerRegion;
    }

    Theme::ContainerRegion::ContainerRegion(const Texture& texture, const Rectangle& region, const Theme::Border& border, const Vector4& color)
        : _border(border), _color(color)
    {
        // Need to convert pixel coords to unit space by dividing by texture size.
        float tw = 1.0f / (float)texture.getWidth();
        float th = 1.0f / (float)texture.getHeight();

        // Can calculate all measurements in advance.
        float leftEdge = region.x * tw;
        float rightEdge = (region.x + region.width) * tw;
        float leftBorder = (region.x + border.left) * tw;
        float rightBorder = (region.x + region.width - border.right) * tw;

        float topEdge = 1.0f - (region.y * th);
        float bottomEdge = 1.0f - ((region.y + region.height) * th);
        float topBorder = 1.0f - ((region.y + border.top) * th);
        float bottomBorder = 1.0f - ((region.y + region.height - border.bottom) * th);

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

    Theme::ContainerRegion::~ContainerRegion()
    {
    }

    const char* Theme::ContainerRegion::getId() const
    {
        return _id.c_str();
    }

    const Theme::Border& Theme::ContainerRegion::getBorder() const
    {
        return _border;
    }

    const Theme::UVs& Theme::ContainerRegion::getUVs(ContainerArea area) const
    {
        return _uvs[area];
    }

    const Vector4& Theme::ContainerRegion::getColor() const
    {
        return _color;
    }

    /****************
     * Theme::Style *
     ****************/
    Theme::Style::Style(const char* id, const Theme::Margin& margin, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active)
        : _id(id), _margin(margin), _padding(padding)
    {
        _overlays[OVERLAY_NORMAL] = normal;
        _overlays[OVERLAY_FOCUS] = focus;
        _overlays[OVERLAY_ACTIVE] = active;
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

    const Theme::Margin& Theme::Style::getMargin() const
    {
        return _margin;
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

    Theme::Style::Overlay::Overlay() : _container(NULL), _textCursor(NULL), _mouseCursor(NULL), _checkBoxIcon(NULL), _radioButtonIcon(NULL), _sliderIcon(NULL), _font(NULL)
    {
    }

    Theme::Style::Overlay::~Overlay()
    {
        SAFE_RELEASE(_container);
        SAFE_RELEASE(_checkBoxIcon);
        SAFE_RELEASE(_radioButtonIcon);
        SAFE_RELEASE(_sliderIcon);
        SAFE_RELEASE(_mouseCursor);
        SAFE_RELEASE(_textCursor);
        SAFE_RELEASE(_font);
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

    Theme::Cursor* Theme::Style::Overlay::getTextCursor() const
    {
        return _textCursor;
    }

    void Theme::Style::Overlay::setTextCursor(Theme::Cursor* cursor)
    {
        if (_textCursor != cursor)
        {
            SAFE_RELEASE(_textCursor);

            _textCursor = cursor;
            
            if (cursor)
            {
                cursor->addRef();
            }
        }
    }

    Theme::Cursor* Theme::Style::Overlay::getMouseCursor() const
    {
        return _mouseCursor;
    }

    void Theme::Style::Overlay::setMouseCursor(Theme::Cursor* cursor)
    {
        if (_mouseCursor != cursor)
        {
            SAFE_RELEASE(_mouseCursor);

            _mouseCursor = cursor;
            
            if (cursor)
            {
                cursor->addRef();
            }
        }
    }

    void Theme::Style::Overlay::setCheckBoxIcon(Icon* icon)
    {
        if (_checkBoxIcon != icon)
        {
            SAFE_RELEASE(_checkBoxIcon);

            _checkBoxIcon = icon;
            
            if (icon)
            {
                icon->addRef();
            }
        }
    }

    Theme::Icon* Theme::Style::Overlay::getCheckBoxIcon() const
    {
        return _checkBoxIcon;
    }

    void Theme::Style::Overlay::setRadioButtonIcon(Icon* icon)
    {
        if (_radioButtonIcon != icon)
        {
            SAFE_RELEASE(_radioButtonIcon);

            _radioButtonIcon = icon;

            if (icon)
            {
                icon->addRef();
            }
        }
    }

    Theme::Icon* Theme::Style::Overlay::getRadioButtonIcon() const
    {
        return _radioButtonIcon;
    }

    void Theme::Style::Overlay::setSliderIcon(SliderIcon* slider)
    {
        if (_sliderIcon != slider)
        {
            SAFE_RELEASE(_sliderIcon);

            _sliderIcon = slider;

            if (slider)
            {
                slider->addRef();
            }
        }
    }

    Theme::SliderIcon* Theme::Style::Overlay::getSliderIcon() const
    {
        return _sliderIcon;
    }

    void Theme::Style::Overlay::setContainerRegion(ContainerRegion* container)
    {
        if (_container != container)
        {
            SAFE_RELEASE(_container);

            _container = container;

            if (container)
            {
                container->addRef();
            }
        }
    }

    Theme::ContainerRegion* Theme::Style::Overlay::getContainerRegion() const
    {
        return _container;
    }

    void Theme::generateUVs(const Texture& texture, float x, float y, float width, float height, UVs* uvs)
    {
        float tw = 1.0f / texture.getWidth();
        float th = 1.0f / texture.getHeight();

        uvs->u1 = x * tw;
        uvs->u2 = (x + width) * tw;
        uvs->v1 = 1.0f - (y * th);
        uvs->v2 = 1.0f - ((y + height) * th);
    }

    void Theme::lookUpSprites(const Properties* overlaySpace, Icon** checkBoxIcon, Icon** radioButtonIcon, SliderIcon** sliderIcon,
                              Cursor** textCursor, Cursor** mouseCursor, ContainerRegion** containerRegion)
    {
        const char* checkBoxString = overlaySpace->getString("checkBox");
        if (checkBoxString)
        {
            for (unsigned int i = 0; i < _icons.size(); i++)
            {
                if (strcmp(_icons[i]->getId(), checkBoxString) == 0)
                {
                    *checkBoxIcon = _icons[i];
                    break;
                }
            }
        }

        const char* radioButtonString = overlaySpace->getString("radioButton");
        if (radioButtonString)
        {
            for (unsigned int i = 0; i < _icons.size(); i++)
            {
                if (strcmp(_icons[i]->getId(), radioButtonString) == 0)
                {
                    *radioButtonIcon = _icons[i];
                    break;
                }
            }
        }

        const char* sliderString = overlaySpace->getString("slider");
        if (sliderString)
        {
            for (unsigned int i = 0; i < _sliders.size(); ++i)
            {
                if (strcmp(_sliders[i]->getId(), sliderString) == 0)
                {
                    *sliderIcon = _sliders[i];
                    break;
                }
            }
        }

        const char* textCursorString = overlaySpace->getString("textCursor");
        if (textCursorString)
        {
            for (unsigned int i = 0; i < _cursors.size(); ++i)
            {
                if (strcmp(_cursors[i]->getId(), textCursorString) == 0)
                {
                    *textCursor = _cursors[i];
                    break;
                }
            }
        }

        const char* mouseCursorString = overlaySpace->getString("mouseCursor");
        if (mouseCursorString)
        {
            for (unsigned int i = 0; i < _cursors.size(); ++i)
            {
                if (strcmp(_cursors[i]->getId(), mouseCursorString) == 0)
                {
                    *mouseCursor = _cursors[i];
                    break;
                }
            }
        }

        const char* containerString = overlaySpace->getString("container");
        if (containerString)
        {
            for (unsigned int i = 0; i < _containers.size(); ++i)
            {
                if (strcmp(_containers[i]->getId(), containerString) == 0)
                {
                    *containerRegion = _containers[i];
                    break;
                }
            }
        }
    }
}
