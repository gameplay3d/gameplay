/*
 * Theme.h
 */

#ifndef THEME_H_
#define THEME_H_

#include "Base.h"
#include "Ref.h"
#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Properties.h"

namespace gameplay
{

#define MAX_OVERLAYS 4
#define MAX_OVERLAY_REGIONS 9

/**
 * This class represents the appearance of an UI form described in a 
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

        padding() : top(0), bottom(0), left(0), right(0) {}
    } Margin, Border, Padding;

    class Icon : public Ref
    {
    public:
        static Icon* create(const char* id, const Texture& texture, const Vector2& size,
                            const Vector2& offPosition, const Vector2& onPosition, const Vector4& color);

        const char* getId() const;
        const Vector2& getSize() const;
        const Vector4& getColor() const;
        const Theme::UVs& getOffUVs() const;
        const Theme::UVs& getOnUVs() const;

    private:
        Icon(const Texture& texture, const Vector2& size, const Vector2& offPosition, const Vector2& onPosition, const Vector4& color);
        Icon(const Icon& copy);
        ~Icon();

        std::string _id;
        Vector2 _size;
        Vector4 _color;
        UVs _off;
        UVs _on;
    };

    class SliderIcon : public Ref
    {
    public:
        static SliderIcon* create(const char* id, const Texture& texture, const Vector4& minCapRegion, const Vector4& maxCapRegion,
                                  const Vector4& markerRegion, const Vector4& trackRegion, const Vector4& color);

        const char* getId() const;
        const Theme::UVs& getMinCapUVs() const;
        const Theme::UVs& getMaxCapUVs() const;
        const Theme::UVs& getMarkerUVs() const;
        const Theme::UVs& getTrackUVs() const;
        const Vector2& getMinCapSize() const;
        const Vector2& getMaxCapSize() const;
        const Vector2& getMarkerSize() const;
        const Vector2& getTrackSize() const;
        const Vector4& getColor() const;

    private:
        SliderIcon(const Texture& texture, const Vector4& minCapRegion, const Vector4& maxCapRegion,
                   const Vector4& markerRegion, const Vector4& trackRegion, const Vector4& color);
        SliderIcon(const SliderIcon& copy);
        ~SliderIcon();

        std::string _id;
        UVs _minCap;
        UVs _maxCap;
        UVs _marker;
        UVs _track;
        Vector2 _minCapSize;
        Vector2 _maxCapSize;
        Vector2 _markerSize;
        Vector2 _trackSize;
        Vector4 _color;
    };
    
    /**
     * This class represents the appearance of a cursor.
     */
    class Cursor : public Ref
    {
    public:
        static Theme::Cursor* create(const char* id, const Texture& texture, const Rectangle& region, const Vector4& color);

       /**
        * Returns the Id of this Cursor.
        */
        const char* getId() const;

       /**
        * Gets a UV coordinates computed from the texture region.
        */
        const Theme::UVs& getUVs() const;

        const Vector2& getSize() const;

        const Vector4& getColor() const;
    
    private:
        Cursor(const Texture& texture, const Rectangle& region, const Vector4& color);
        Cursor(const Cursor& copy);
        ~Cursor();

        std::string _id;
        UVs _uvs;
        Vector2 _size;
        Vector4 _color;
    };

    class ContainerRegion : public Ref
    {
    public:
        static ContainerRegion* create(const char* id, const Texture& texture, const Rectangle& region, const Theme::Border& border, const Vector4& color);

        enum ContainerArea
        {
            TOP_LEFT, TOP, TOP_RIGHT,
            LEFT, CENTER, RIGHT,
            BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT
        };

        const char* getId() const;
        const Theme::Border& getBorder() const;
        const Theme::UVs& getUVs(ContainerArea area) const;
        const Vector4& getColor() const;
        
        /**
         * Set the size of this ContainerRegion's border.
         *
         * When auto-size is set on width and/or height:
         * Space added to the calculated (tightly bound) width and height.
         *
         */
        //void setBorder(float top, float bottom, float left, float right);

    private:
        ContainerRegion(const Texture& texture, const Rectangle& region, const Theme::Border& border, const Vector4& color);
        ContainerRegion(const ContainerRegion& copy);
        ~ContainerRegion();
    
        std::string _id;
        Theme::Border _border;
        UVs _uvs[MAX_OVERLAY_REGIONS];
        Vector4 _color;
    };

    /**
     * Creates an instance of a Theme from a theme file.
     *
     * @param path Path to a theme file.
     *
     * @return A new Theme.
     */
    static Theme* create(const char* path);

    /**
     * Returns style with the given name.
     *
     * @param id ID of the style (as specified in the Theme file).
     *
     * @return Instance of the Style.
     */
    Theme::Style* getStyle(const char* id) const;

    void setProjectionMatrix(const Matrix& matrix);

    SpriteBatch* getSpriteBatch() const;
    
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
            OVERLAY_ACTIVE,
            OVERLAY_DISABLED
        };

        Style(const char* id, const Theme::Margin& margin, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active, Theme::Style::Overlay* disabled);

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
         * Gets the Padding region of this style.
         */
        const Theme::Padding& getPadding() const;

        /**
         * Gets the Margin region of this style.
         */
        const Theme::Margin& getMargin() const;

        /**
         * Set this size of this Style's padding.
         *
         * Padding is the space between a Control's content (all icons and text) and its border.
         */
        void setPadding(float top, float bottom, float left, float right);

        /**
         * Set the size of this Style's margin.
         *
         * The margin is used by Layouts other than AbsoluteLayout to put space between Controls.
         */
        void setMargin(float top, float bottom, float left, float right);
       
        /**
         * This class represents a control's overlay for one of the 3 modes: normal, focussed or active.
         */
        class Overlay : public Ref
        {
        public:
            static Overlay* create();

           /**
            * Returns the Overlay type.
            */
            OverlayType getType();

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

            const Vector4& getTextColor() const;
            void setTextColor(const Vector4& color);
            
           /**
            * Gets a cursor associated with this overlay.
            */
            void setTextCursor(Cursor* cursor);
            Cursor* getTextCursor() const;
            
            void setMouseCursor(Cursor* cursor);
            Cursor* getMouseCursor() const;
            
            void setCheckBoxIcon(Icon* icon);
            Icon* getCheckBoxIcon() const;

            void setRadioButtonIcon(Icon* icon);
            Icon* getRadioButtonIcon() const;

            void setSliderIcon(SliderIcon* slider);
            SliderIcon* getSliderIcon() const;
            
            void setContainerRegion(ContainerRegion* container);
            ContainerRegion* getContainerRegion() const;
        
        private:
            Overlay();
            Overlay(const Overlay& copy);
            ~Overlay();

            ContainerRegion* _container;
            Cursor* _textCursor;
            Cursor* _mouseCursor;
            Icon* _checkBoxIcon;
            Icon* _radioButtonIcon;
            SliderIcon* _sliderIcon;
            Font* _font;
            unsigned int _fontSize;
            Font::Justify _alignment;
            bool _textRightToLeft;
            Vector4 _textColor;
        };

    private:
        Style(const Style& style);
        
        std::string _id;
        Margin _margin;
        Padding _padding;
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

    static void generateUVs(const Texture& texture, float x, float y, float width, float height, UVs* uvs);
    void lookUpSprites(const Properties* overlaySpace, Icon** checkBoxIcon, Icon** radioButtonIcon, SliderIcon** sliderIcon,
                              Cursor** textCursor, Cursor** mouseCursor, ContainerRegion** containerRegion);

    std::string _path;
    Texture* _texture;
    SpriteBatch* _spriteBatch;
    std::vector<Cursor*> _cursors;
    std::vector<Style*> _styles;
    std::vector<Icon*> _icons;
    std::vector<SliderIcon*> _sliders;
    std::vector<ContainerRegion*> _containers;
    std::set<Font*> _fonts;
};

}

#endif
