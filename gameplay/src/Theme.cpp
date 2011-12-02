/*
 * Theme.cpp
 */

#include "Theme.h"

namespace gameplay
{
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
        for (unsigned int i = 0, count = _cursors.size(); i < count; ++i)
        {
            Cursor* cursor = _cursors[i];
            if (cursor)
            {
                delete cursor;
            }
        }

        for (unsigned int i = 0, count = _styles.size(); i < count; ++i)
        {
            Style* style = _styles[i];
            if (style)
            {
                delete style;
            }
        }

        for (unsigned int i = 0, count = _fonts.size(); i < count; ++i)
        {
            Font* font = _fonts[i];
            if (font)
            {
                SAFE_RELEASE(font);
            }
        }
    }

    Theme::Style::Overlay::~Overlay()
    {
        SAFE_RELEASE(_font);
    }

    Theme* Theme::create(const char* path)
    {
        assert(path);

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
        return theme;
    }   

    const Theme::Style* Theme::getStyle(const char* name) const
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

    const Theme::Style::Overlay& Theme::Style::getOverlay(OverlayType overlayType) const
    {
        return _overlays[overlayType];
    }

    const Theme::Border& Theme::Style::getBorder() const
    {
        return _border;
    }

    const Theme::Padding& Theme::Style::getPadding() const
    {
        return _padding;
    }

    Theme::Style::OverlayType Theme::Style::Overlay::getId()
    {
        return _id;
    }
    
    const Rectangle& Theme::Style::Overlay::getRegion() const
    {
        return _region;
    }

    const Theme::UVs& Theme::Style::Overlay::getUVs() const
    {
        return _uvs;   
    }

    const Font* Theme::Style::Overlay::getFont() const
    {
        return _font;
    }

    const Theme::Cursor& Theme::Style::Overlay::getCursor() const
    {
        return _cursor;
    }

}
