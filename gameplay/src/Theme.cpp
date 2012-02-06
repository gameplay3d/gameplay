/*
 * Theme.cpp
 */

#include "Base.h"
#include "Theme.h"

namespace gameplay
{
    static std::vector<Theme*> __themeCache;

    Theme::Theme()
        //: _texture(NULL)
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
            SAFE_DELETE(cursor);
        }

        for (unsigned int i = 0, count = _fonts.size(); i < count; ++i)
        {
            Font* font = _fonts[i];
            SAFE_RELEASE(font);
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

        SAFE_DELETE(_spriteBatch);

        // Remove ourself from the theme cache.
        std::vector<Theme*>::iterator itr = find(__themeCache.begin(), __themeCache.end(), this);
        if (itr != __themeCache.end())
        {
            __themeCache.erase(itr);
        }
    }

    Theme::Style::Overlay::~Overlay()
    {
        //SAFE_RELEASE(_cursor);
        SAFE_RELEASE(_checkBoxIcon);
        SAFE_RELEASE(_radioButtonIcon);
        SAFE_RELEASE(_font);
    }

    Theme::Style::~Style()
    {
        for (int i = 0; i < MAX_OVERLAYS; i++)
        {
            SAFE_RELEASE(_overlays[i]);
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
            // First load all cursors, checkboxes etc. that are referred to be styles.
            const char* spacename = space->getNamespace();
            if (strcmp(spacename, "cursor") == 0)
            {
                Vector4 regionVector;                
                space->getVector4("region", &regionVector);
                const Rectangle region(regionVector.x, regionVector.y, regionVector.z, regionVector.w);

                Vector4 color;
                space->getColor("color", &color);

                Theme::Cursor* c = new Theme::Cursor(space->getId(), region, color);
                theme->_cursors.push_back(c);
            }
            else if (strcmp(spacename, "icon") == 0)
            {
                Vector2 offVec;
                Vector2 onVec;
                Vector2 size;
                Vector2 activeVec;
                space->getVector2("offPosition", &offVec);
                space->getVector2("onPosition", &onVec);
                space->getVector2("activePosition", &activeVec);
                space->getVector2("size", &size);

                float tw = theme->_texture->getWidth();
                float th = theme->_texture->getHeight();

                UVs on;
                UVs off;
                UVs active;
                generateUVs(onVec.x, onVec.y, size.x, size.y, tw, th, &on);
                generateUVs(offVec.x, offVec.y, size.x, size.y, tw, th, &off);
                generateUVs(activeVec.x, activeVec.y, size.x, size.y, tw, th, &active);

                Icon* icon = new Icon();
                icon->id = space->getId();
                icon->on = on;
                icon->off = off;
                icon->active = active;
                icon->size = size;

                theme->_icons.push_back(icon);
            }
            else if (strcmp(spacename, "slider") == 0)
            {
                Vector4 leftCapRegion;
                Vector4 rightCapRegion;
                Vector4 trackRegion;
                Vector4 markerRegion;
                space->getVector4("leftCapRegion", &leftCapRegion);
                space->getVector4("rightCapRegion", &rightCapRegion);
                space->getVector4("trackRegion", &trackRegion);
                space->getVector4("markerRegion", &markerRegion);

                float tw = theme->_texture->getWidth();
                float th = theme->_texture->getHeight();

                // Vec4f regions:
                // .z == width, .w == height
                UVs leftCap;
                UVs rightCap;
                UVs track;
                UVs marker;
                generateUVs(leftCapRegion.x, leftCapRegion.y, leftCapRegion.z, leftCapRegion.w, tw, th, &leftCap);
                generateUVs(rightCapRegion.x, rightCapRegion.y, rightCapRegion.z, rightCapRegion.w, tw, th, &rightCap);
                generateUVs(trackRegion.x, trackRegion.y, trackRegion.z, trackRegion.w, tw, th, &track);
                generateUVs(markerRegion.x, markerRegion.y, markerRegion.z, markerRegion.w, tw, th, &marker);

                SliderIcon* sliderIcon = new SliderIcon();
                sliderIcon->id = space->getId();
                sliderIcon->leftCap = leftCap;
                sliderIcon->rightCap = rightCap;
                sliderIcon->track = track;
                sliderIcon->marker = marker;
                sliderIcon->leftCapSize.set(leftCapRegion.z, leftCapRegion.w);
                sliderIcon->rightCapSize.set(rightCapRegion.z, rightCapRegion.w);
                sliderIcon->trackSize.set(trackRegion.z, trackRegion.w);
                sliderIcon->markerSize.set(markerRegion.z, markerRegion.w);

                theme->_sliders.push_back(sliderIcon);
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
                Theme::Border border;
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
                        Vector4 regionVector;
                        innerSpace->getVector4("region", &regionVector);
                        const Rectangle region = Rectangle(regionVector.x, regionVector.y, regionVector.z, regionVector.w);

                        Vector4 textColor;
                        Vector4 borderColor;
                        innerSpace->getColor("textColor", &textColor);
                        innerSpace->getColor("borderColor", &borderColor);

                        // Font info.
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

                        const char* checkBoxString = innerSpace->getString("checkBox");
                        Icon* checkBoxIcon = NULL;
                        if (checkBoxString)
                        {
                            for (unsigned int i = 0; i < theme->_icons.size(); i++)
                            {
                                if (strcmp(theme->_icons[i]->id.c_str(), checkBoxString) == 0)
                                {
                                    checkBoxIcon = theme->_icons[i];
                                    break;
                                }
                            }
                        }

                        const char* radioButtonString = innerSpace->getString("radioButton");
                        Icon* radioButtonIcon = NULL;
                        if (radioButtonString)
                        {
                            for (unsigned int i = 0; i < theme->_icons.size(); i++)
                            {
                                if (strcmp(theme->_icons[i]->id.c_str(), radioButtonString) == 0)
                                {
                                    radioButtonIcon = theme->_icons[i];
                                    break;
                                }
                            }
                        }

                        const char* sliderString = innerSpace->getString("slider");
                        SliderIcon* sliderIcon = NULL;
                        if (sliderString)
                        {
                            for (unsigned int i = 0; i < theme->_sliders.size(); ++i)
                            {
                                if (strcmp(theme->_sliders[i]->id.c_str(), sliderString) == 0)
                                {
                                    sliderIcon = theme->_sliders[i];
                                    break;
                                }
                            }
                        }

                        // TODO: Cursor.

                        normal = new Theme::Style::Overlay(Theme::Style::OVERLAY_NORMAL);
                        normal->setRegion(region);
                        normal->setTextColor(textColor);
                        normal->setBorderColor(borderColor);
                        normal->setFont(font);
                        normal->setFontSize(fontSize);
                        normal->setTextAlignment(alignment);
                        normal->setTextRightToLeft(rightToLeft);
                        normal->setCheckBoxIcon(checkBoxIcon);
                        normal->setRadioButtonIcon(radioButtonIcon);
                        normal->setSliderIcon(sliderIcon);

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
                    else if (strcmp(innerSpacename, "border") == 0)
                    {
                        border.top = innerSpace->getFloat("top");
                        border.bottom = innerSpace->getFloat("bottom");
                        border.left = innerSpace->getFloat("left");
                        border.right = innerSpace->getFloat("right");
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
                        Vector4 regionVector;
                        Rectangle region;
                        if (!innerSpace->getVector4("region", &regionVector))
                        {
                            region.set(normal->getRegion());
                        }
                        else
                        {
                            region = Rectangle(regionVector.x, regionVector.y, regionVector.z, regionVector.w);
                        }

                        Vector4 textColor;
                        if (!innerSpace->getColor("textColor", &textColor))
                        {
                            textColor.set(normal->getTextColor());
                        }

                        Vector4 borderColor;
                        if (!innerSpace->getColor("borderColor", &borderColor))
                        {
                            borderColor.set(normal->getBorderColor());
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

                        const char* checkBoxString = innerSpace->getString("checkBox");
                        Icon* checkBoxIcon = NULL;
                        if (checkBoxString)
                        {
                            for (unsigned int i = 0; i < theme->_icons.size(); i++)
                            {
                                if (strcmp(theme->_icons[i]->id.c_str(), checkBoxString) == 0)
                                {
                                    checkBoxIcon = theme->_icons[i];
                                    break;
                                }
                            }
                        }
                        if (!checkBoxIcon)
                        {
                            checkBoxIcon = normal->getCheckBoxIcon();
                        }

                        const char* radioButtonString = innerSpace->getString("radioButton");
                        Icon* radioButtonIcon = NULL;
                        if (radioButtonString)
                        {
                            for (unsigned int i = 0; i < theme->_icons.size(); i++)
                            {
                                if (strcmp(theme->_icons[i]->id.c_str(), radioButtonString) == 0)
                                {
                                    radioButtonIcon = theme->_icons[i];
                                    break;
                                }
                            }
                        }
                        if (!radioButtonIcon)
                        {
                            radioButtonIcon = normal->getRadioButtonIcon();
                        }

                        const char* sliderString = innerSpace->getString("slider");
                        SliderIcon* sliderIcon = NULL;
                        if (sliderString)
                        {
                            for (unsigned int i = 0; i < theme->_sliders.size(); ++i)
                            {
                                if (strcmp(theme->_sliders[i]->id.c_str(), sliderString) == 0)
                                {
                                    sliderIcon = theme->_sliders[i];
                                    break;
                                }
                            }
                        }
                        if (!sliderIcon)
                        {
                            sliderIcon = normal->getSliderIcon();
                        }

                        // TODO: Cursor.
                        
                        if (strcmp(innerSpacename, "focus") == 0)
                        {
                            focus = new Theme::Style::Overlay(Theme::Style::OVERLAY_FOCUS);
                            focus->setRegion(region);
                            focus->setTextColor(textColor);
                            focus->setBorderColor(borderColor);
                            focus->setFont(font);
                            focus->setFontSize(fontSize);
                            focus->setTextAlignment(alignment);
                            focus->setTextRightToLeft(rightToLeft);
                            focus->setCheckBoxIcon(checkBoxIcon);
                            focus->setRadioButtonIcon(radioButtonIcon);
                            focus->setSliderIcon(sliderIcon);
                        }
                        else if (strcmp(innerSpacename, "active") == 0)
                        {
                            active = new Theme::Style::Overlay(Theme::Style::OVERLAY_ACTIVE);
                            active->setRegion(region);
                            active->setTextColor(textColor);
                            active->setBorderColor(borderColor);
                            active->setFont(font);
                            active->setFontSize(fontSize);
                            active->setTextAlignment(alignment);
                            active->setTextRightToLeft(rightToLeft);
                            active->setCheckBoxIcon(checkBoxIcon);
                            active->setRadioButtonIcon(radioButtonIcon);
                            active->setSliderIcon(sliderIcon);
                        }
                    }

                    innerSpace = space->getNextNamespace();
                }

                unsigned int textureWidth = theme->_texture->getWidth();
                unsigned int textureHeight = theme->_texture->getHeight();

                normal->calculateUVs(border, textureWidth, textureHeight);
                
                if (focus)
                {
                    focus->calculateUVs(border, textureWidth, textureHeight);
                }
                else
                {
                    focus = normal;
                    focus->addRef();
                }
                if (active)
                {
                    active->calculateUVs(border, textureWidth, textureHeight);
                }
                else
                {
                    active = normal;
                    active->addRef();
                }

                Theme::Style* s = new Theme::Style(space->getId(), margin, border, padding, normal, focus, active);
                theme->_styles.push_back(s);
            }

            space = themeProperties->getNextNamespace();
        }

        // Add this theme to the cache.
        __themeCache.push_back(theme);

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

    SpriteBatch* Theme::getSpriteBatch() const
    {
        return _spriteBatch;
    }

    Theme::Style::Style(const char* id, const Theme::Margin& margin, const Theme::Border& border, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active)
        : _id(id), _margin(margin), _border(border), _padding(padding)
    {
        _overlays[OVERLAY_NORMAL] = normal;
        _overlays[OVERLAY_FOCUS] = focus;
        _overlays[OVERLAY_ACTIVE] = active;
    }

    Theme::Cursor::Cursor(const char* id, const Rectangle& region, const Vector4& color)
        : _id(id), _region(region), _color(color)
    {
    }

    const char* Theme::Cursor::getId() const
    {
        return _id.data();
    }

    const Rectangle& Theme::Cursor::getRegion() const
    {
        return _region;
    }

    const Theme::UVs& Theme::Cursor::getUVs() const
    {
        return _uvs;
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

    const Theme::Border& Theme::Style::getBorder() const
    {
        return _border;
    }

    const Theme::Padding& Theme::Style::getPadding() const
    {
        return _padding;
    }

    Theme::Style::Overlay::Overlay() : _cursor(NULL), _checkBoxIcon(NULL), _radioButtonIcon(NULL), _font(NULL)
    {
    }

    Theme::Style::Overlay::Overlay(Theme::Style::OverlayType type)
        : _cursor(NULL), _checkBoxIcon(NULL), _radioButtonIcon(NULL), _sliderIcon(NULL), _font(NULL), _type(type)
    {
    }

    Theme::Style::OverlayType Theme::Style::Overlay::getType()
    {
        return _type;
    }
    
    const Rectangle& Theme::Style::Overlay::getRegion() const
    {
        return _region;
    }

    void Theme::Style::Overlay::setRegion(const Rectangle& region)
    {
        _region = region;
    }

    void Theme::Style::Overlay::calculateUVs(const Theme::Border& border, unsigned int textureWidth, unsigned int textureHeight)
    {
        // Need to convert pixel coords to unit space by dividing by texture size.
        float tw = 1.0f / (float)textureWidth;
        float th = 1.0f / (float)textureHeight;

        // Can calculate all measurements in advance.
        float leftEdge = _region.x * tw;
        float rightEdge = (_region.x + _region.width) * tw;
        float leftBorder = (_region.x + border.left) * tw;
        float rightBorder = (_region.x + _region.width - border.right) * tw;

        float topEdge = 1.0f - (_region.y * th);
        float bottomEdge = 1.0f - ((_region.y + _region.height) * th);
        float topBorder = 1.0f - ((_region.y + border.top) * th);
        float bottomBorder = 1.0f - ((_region.y + _region.height - border.bottom) * th);

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

    const Theme::UVs& Theme::Style::Overlay::getUVs(OverlayArea area)
    {
        return _uvs[area];
    }

    void Theme::Style::Overlay::setUVs(OverlayArea area, const Theme::UVs& uvs)
    {
        _uvs[area] = uvs;
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

            if (font)
            {
                font->addRef();
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
            
    // Text direction.
    bool Theme::Style::Overlay::getTextRightToLeft() const
    {
        return _textRightToLeft;
    }

    void Theme::Style::Overlay::setTextRightToLeft(bool rightToLeft)
    {
        _textRightToLeft = rightToLeft;
    }

    Theme::Cursor* Theme::Style::Overlay::getCursor() const
    {
        return _cursor;
    }

    void Theme::Style::Overlay::setCursor(Theme::Cursor* cursor)
    {
        _cursor = cursor;
    }

    const Vector4& Theme::Style::Overlay::getTextColor() const
    {
        return _textColor;
    }

    void Theme::Style::Overlay::setTextColor(const Vector4& color)
    {
        _textColor = color;
    }

    const Vector4& Theme::Style::Overlay::getBorderColor() const
    {
        return _borderColor;
    }

    void Theme::Style::Overlay::setBorderColor(const Vector4& color)
    {
        _borderColor = color;
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

    Theme::Icon* Theme::Style::Overlay::getCheckBoxIcon()
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

    Theme::Icon* Theme::Style::Overlay::getRadioButtonIcon()
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

    Theme::SliderIcon* Theme::Style::Overlay::getSliderIcon()
    {
        return _sliderIcon;
    }

    void Theme::Style::Overlay::addRef()
    {
        Ref::addRef();
        
        if (_font)
        {
            _font->addRef();
        }
        
        if (_radioButtonIcon)
        {
            _radioButtonIcon->addRef();
        }
        
        if (_checkBoxIcon)
        {
            _checkBoxIcon->addRef();
        }

        if (_sliderIcon)
        {
            _sliderIcon->addRef();
        }
    }

    void Theme::generateUVs(float x, float y, float width, float height, float textureWidth, float textureHeight, UVs* uvs)
    {
        float tw = 1.0f / textureWidth;
        float th = 1.0f / textureHeight;

        uvs->u1 = x * tw;
        uvs->u2 = (x + width) * tw;
        uvs->v1 = 1.0f - (y * th);
        uvs->v2 = 1.0f - ((y + height) * th);
    }
}
