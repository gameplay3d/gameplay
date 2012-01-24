/*
 * Theme.h
 */

# ifndef THEME_H_
# define THEME_H_

#include "Base.h"
#include "Ref.h"
#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Properties.h"

namespace gameplay
{

#define MAX_OVERLAYS 3
#define MAX_OVERLAY_REGIONS 9
/**
 * This class represents the apperance of an UI form described in a 
 * theme file. A theme file, at its simplest, contains a source texture atlas,
 * the texture coordinates of each control in its different mode. Once loaded,
 * the appearance properties can be retrieved using a style id and set on a UI control.
 */
class Theme: public Ref
{
public:
    class Style;
    class Cursor;

    typedef struct UVs
    {
        float u1;
        float v1;
        float u2;
        float v2;
    } UVs;

    typedef struct padding
    {
        float top;
        float bottom;
        float left;
        float right;
    } Margin, Border, Padding;

    typedef struct icon
    {
        std::string id;
        UVs off;
        UVs on;
        Vector2 size;
    } CheckBoxIcon, RadioButtonIcon;

    /**
     * Creates an instance of a Theme from a theme file.
     *
     * @param path path to a theme file.
     *
     * @return A new Theme.
     */
    static Theme* create(const char* path);

    /**
     * Returns style with the given name.
     *
     * @param name Name of the style (as specified in the Theme file).
     *
     * @return instance of the Style.
     */
    Theme::Style* getStyle(const char* styleName) const;

    SpriteBatch* getSpriteBatch() const;

    /**
     * This class represents the apperance of a cursor.
     */
    class Cursor
    {
    public:
        Cursor(const char* id, const Rectangle& region, const Vector4& color);

       /**
        * Returns the Id of this Cursor.
        */
        const char* getId() const;

       /**
        * Gets a texture region in the texture atlas for a cursor.
        */
        const Rectangle& getRegion() const;

       /**
        * Gets a UV coordinates computed from the texture region.
        */
        const Theme::UVs& getUVs() const;
    
    private:

        std::string _id;
        Rectangle _region;
        UVs _uvs;
        Vector4 _color;
    };
    
    /**
     * This class represents the apperance of a control's style.
     */
    class Style
    {
    public:
        class Overlay;

        enum OverlayType
        {
            OVERLAY_NORMAL,
            OVERLAY_FOCUS,
            OVERLAY_ACTIVE
        };

        Style(const char* id, const Theme::Margin& margin, const Theme::Border& border, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active);

        ~Style();

        /**
         * Returns the Id of this Style.
         */
        const char* getId() const;

        /**
         * Gets an overlay from the overlay type.
         */
        Theme::Style::Overlay* getOverlay(OverlayType overlayType) const;

        /**
         * Gets the Border region of this overlay.
         */
        const Theme::Border& getBorder() const;

        /**
         * Gets the Padding region of this overlay.
         */
        const Theme::Padding& getPadding() const;

        /**
         * Gets the Margin region of this overlay.
         */
        const Theme::Margin& getMargin() const;
       
        /**
         * This class represents a control's overlay for one of the 3 modes: normal, focussed or active.
         */
        class Overlay
        {
        public:
            Overlay();
            Overlay(OverlayType type);

            enum OverlayArea
            {
                TOP_LEFT, TOP, TOP_RIGHT,
                LEFT, CENTER, RIGHT,
                BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT
            };

           /**
            * Destructor.
            */
            ~Overlay();

           /**
            * Returns the Overlay type.
            */
            OverlayType getType();
            
            /**
            * Gets a texture region in the texture atlas that the overlay uses.
            */
            const Rectangle& getRegion() const;

            void setRegion(const Rectangle& region);

            // Calculates and sets UV coordinates based on the current region and given border and texture size.
            void calculateUVs(const Theme::Border& border, unsigned int textureWidth, unsigned int textureHeight);

            const Theme::UVs& getUVs(OverlayArea area);

            void setUVs(OverlayArea area, const Theme::UVs& uvs);

           /**
            * Gets a font associated with this overlay.
            */
            Font* getFont() const;

            void setFont(Font* font);

            // Font size.
            unsigned int getFontSize() const;
            void setFontSize(unsigned int fontSize);

            // Alignment.
            Font::Justify getTextAlignment() const;
            void setTextAlignment(Font::Justify alignment);
            
            // Text direction.
            bool getTextRightToLeft() const;
            void setTextRightToLeft(bool rightToLeft);

           /**
            * Gets a cursor associated with this overlay.
            */
            Cursor* getCursor() const;

            void setCursor(Cursor* cursor);

            const Vector4& getTextColor() const;

            void setTextColor(const Vector4& color);

            const Vector4& getBorderColor() const;

            void setBorderColor(const Vector4& color);
            
            void setCheckBoxIcon(CheckBoxIcon* icon);

            CheckBoxIcon* getCheckBoxIcon();
        
        private:
           
            OverlayType _type;
            Rectangle _region;
            UVs _uvs[MAX_OVERLAY_REGIONS];
            Cursor* _cursor;
            CheckBoxIcon* _checkBoxIcon;
            RadioButtonIcon* _radioButtonIcon;
            Font* _font;
            unsigned int _fontSize;
            Font::Justify _alignment;
            bool _textRightToLeft;
            Vector4 _textColor;
            Vector4 _borderColor;
        };

    private:
        
        std::string _id;
        Border _border;
        Padding _padding;
        Margin _margin;
        Overlay* _overlays[MAX_OVERLAYS];
    };

private:
    /**
     * Constructor.
     */
    Theme();

    /**
     * Copy Constructor.
     */
    Theme(const Theme* theme);

    /**
     * Destructor.
     */
    ~Theme();

    std::string _path;
    Texture* _texture;
    SpriteBatch* _spriteBatch;
    std::vector<Cursor *> _cursors;
    std::vector<Style *> _styles;
    std::vector<Font *> _fonts;
    std::vector<CheckBoxIcon *> _checkBoxIcons;
};

}

#endif