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

static const unsigned int MAX_OVERLAYS = 4;
static const unsigned int MAX_OVERLAY_REGIONS = 9;

/**
 * A theme is created and stored as part of a form and represents its appearance.
 * Once loaded, the appearance properties can be retrieved from their style IDs and set on other
 * UI controls.  A Theme has one property, 'texture', which points to an image containing
 * all the Icon, Cursor, Slider, and Container sprites used by the theme.  Each set of sprites within
 * the texture is described in its own namespace.  The rest of the Theme consists of Style namespaces.
 * A Style describes the border, margin, and padding of a Control, what icons and cursors (if any) are
 * associated with a Control, and Font properties to apply to a Control's text.
 *
 * Below is an explanation of the properties that can be set within themes:
 *
 * theme
 * {
 *    texture = <Path to texture>
 *
 *    // Describes the images used for CheckBox and RadioButton icons.
 *    icon <iconID>
 *    {
 *        size            =   <width, height>     // Size of this icon.
 *        offPosition     =   <x, y>              // Position of the unchecked / unselected image.
 *        onPosition      =   <x, y>              // Position of the checked / selected image.
 *        color           =   <#ffffffff>         // Tint to apply to icon.
 *    }
 *   
 *    cursor <cursorID>
 *    {
 *        region  =   <x, y, width, height>   // Region within the texture of cursor sprite.
 *        color   =   <#ffffffff>             // Tint to apply to cursor.
 *    }
 *   
 *    slider <sliderID>
 *    {
 *        minCapRegion    =   <x, y, width, height>   // Left / bottom slider cap.
 *        maxCapRegion    =   <x, y, width, height>   // Right / top slider cap.
 *        markerRegion    =   <x, y, width, height>   // Shows the slider's current position.
 *        trackRegion     =   <x, y, width, height>   // Track the marker slides along.
 *        color           =   <#ffffffff>             // Tint to apply to slider sprites.
 *    }
 *   
 *    // Defines the border and background of a Control.
 *    container <containerID>
 *    {
 *        // The corners and edges of the given region will be used as border sprites.
 *        border
 *        {
 *            top     =   <int>   // Height of top border, top corners.
 *            bottom  =   <int>   // Height of bottom border, bottom corners.
 *            left    =   <int>   // Width of left border, left corners.
 *            right   =   <int>   // Width of right border, right corners.
 *        }
 *       
 *        region  =   <x, y, width, height>   // Total container region including entire border.
 *        color   =   <#ffffffff>             // Tint to apply to container sprites.
 *    }
 *   
 *    style <styleID>
 *    {
 *        // Layouts may make use of a style's margin to put space between adjacent controls.
 *        margin
 *        {
 *            top     =   <int>
 *            bottom  =   <int>
 *            left    =   <int>
 *            right   =   <int>        
 *        }
 *       
 *        // Padding is the space between a control's border and its content.
 *        padding
 *        {
 *            top     =   <int>
 *            bottom  =   <int>
 *            left    =   <int>
 *            right   =   <int>        
 *        }
 *       
 *        // This overlay is used when a control is enabled but not active or focused.
 *        normal
 *        {
 *            container   =   <containerID>               // Container to use for border and background sprites.
 *            checkBox    =   <iconID>                    // Icon to use for checkbox sprites.
 *            radioButton =   <iconID>                    // Icon to use for radioButton sprites.
 *            slider      =   <sliderID>                  // Slider to use for slider sprites.
 *            mouseCursor =   <cursorID>                  // Cursor to use when the mouse is over this control.
 *            textCursor  =   <cursorID>                  // Cursor to use within a textBox.
 *            font        =   <Path to font>              // Font to use for rendering text.
 *            fontSize    =   <int>                       // Size of text.
 *            textColor   =   <#ffffffff>                 // Color of text.
 *            alignment   =   <Text alignment constant>   // Member of Font::Justify enum.
 *            rightToLeft =   <bool>                      // Whether to draw text from right to left.
 *        }
 *       
 *        // This overlay is used when the control is in focus.
 *        // If not specified, the 'normal' overlay will be used.
 *        focus{}
 *       
 *        // This overlay is used when the control is active.
 *        // (Touch or mouse is down within the control.)
 *        // If not specified, the 'normal' overlay will be used.
 *        active{}
 *    }
 * }
 *
 */
class Theme: public Ref
{
    friend class Control;
    friend class Form;
    friend class Container;

public:
    class Style;
    class Cursor;

private:
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

public:
    /**
     * Struct representing the UV coordinates of a rectangular image.
     */
    typedef struct UVs
    {
        float u1;
        float v1;
        float u2;
        float v2;
    } UVs;

    /**
     * Struct representing margin, border, and padding areas by
     * the width or height of each side.
     */
    typedef struct padding
    {
        float top;
        float bottom;
        float left;
        float right;

        padding() : top(0), bottom(0), left(0), right(0) {}
    } Margin, Border, Padding;

    /**
     * An icon with two images representing "on" and "off" states.
     */
    class Icon : public Ref
    {
        friend class Theme;

    public:
        /**
         * Get this icon's ID.
         *
         * @return This icon's ID.
         */
        const char* getId() const;

        /**
         * Get this icon's size.
         *
         * @return This icon's size.
         */
        const Vector2& getSize() const;
        /**
         * Get this icon's color.
         *
         * @return This icon's color.
         */
        const Vector4& getColor() const;

        /**
         * Get the UVs for this icon's off-state image.
         *
         * @return The UVs for this icon's off-state image.
         */
        const Theme::UVs& getOffUVs() const;

        /**
         * Get the UVs for this icon's on-state image.
         *
         * @return The UVs for this icon's on-state image.
         */
        const Theme::UVs& getOnUVs() const;

    private:
        Icon(const Texture& texture, const Vector2& size, const Vector2& offPosition, const Vector2& onPosition, const Vector4& color);
        Icon(const Icon& copy);
        ~Icon();

        static Icon* create(const char* id, const Texture& texture, const Vector2& size,
                            const Vector2& offPosition, const Vector2& onPosition, const Vector4& color);

        std::string _id;
        Vector2 _size;
        Vector4 _color;
        UVs _off;
        UVs _on;
    };

    /**
     * A set of four icons that make up a slider:
     * two end-caps, a track, and a marker to be placed along the track.
     */
    class SliderIcon : public Ref
    {
        friend class Theme;

    public:
        /**
         * Get this slider icon's ID.
         *
         * @return This slider icon's ID.
         */
        const char* getId() const;

        /**
         * Get the UVs for this slider's min-cap image.
         *
         * @return The UVs for this slider's min-cap image.
         */
        const Theme::UVs& getMinCapUVs() const;

        /**
         * Get the UVs for this slider's max-cap image.
         *
         * @return The UVs for this slider's max-cap image.
         */
        const Theme::UVs& getMaxCapUVs() const;

        /**
         * Get the UVs for this slider's marker image.
         *
         * @return The UVs for this slider's marker image.
         */
        const Theme::UVs& getMarkerUVs() const;

        /**
         * Get the UVs for this slider's track image.
         *
         * @return The UVs for this slider's track image.
         */
        const Theme::UVs& getTrackUVs() const;

        /**
         * Get this slider's min-cap size.
         *
         * @return This slider's min-cap size.
         */
        const Vector2& getMinCapSize() const;

        /**
         * Get this slider's max-cap size.
         *
         * @return This slider's max-cap size.
         */
        const Vector2& getMaxCapSize() const;

        /**
         * Get this slider's marker size.
         *
         * @return This slider's marker size.
         */
        const Vector2& getMarkerSize() const;

        /**
         * Get this slider's track size.
         *
         * @return This slider's track size.
         */
        const Vector2& getTrackSize() const;

        /**
         * Get this slider's color.
         *
         * @return This slider's color.
         */
        const Vector4& getColor() const;

    private:
        SliderIcon(const Texture& texture, const Vector4& minCapRegion, const Vector4& maxCapRegion,
                   const Vector4& markerRegion, const Vector4& trackRegion, const Vector4& color);
        SliderIcon(const SliderIcon& copy);
        ~SliderIcon();

        static SliderIcon* create(const char* id, const Texture& texture, const Vector4& minCapRegion, const Vector4& maxCapRegion,
                                  const Vector4& markerRegion, const Vector4& trackRegion, const Vector4& color);

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
        friend class Theme;

    public:
       /**
        * Returns the Id of this Cursor.
        *
        * @return This cursor's ID.
        */
        const char* getId() const;

       /**
        * Gets a UV coordinates computed from the texture region.
        *
        * @return This cursor's UVs.
        */
        const Theme::UVs& getUVs() const;

        /**
         * Gets this cursor's size.
         *
         * @return This cursor's size.
         */
        const Vector2& getSize() const;

        /**
         * Gets this cursor's color.
         *
         * @return This cursor's color.
         */
        const Vector4& getColor() const;
    
    private:
        Cursor(const Texture& texture, const Rectangle& region, const Vector4& color);
        Cursor(const Cursor& copy);
        ~Cursor();

        static Theme::Cursor* create(const char* id, const Texture& texture, const Rectangle& region, const Vector4& color);

        std::string _id;
        UVs _uvs;
        Vector2 _size;
        Vector4 _color;
    };

    /**
     * A container region defines the border and background of a control.
     */
    class ContainerRegion : public Ref
    {
        friend class Theme;

    public:
        enum ContainerArea
        {
            TOP_LEFT, TOP, TOP_RIGHT,
            LEFT, CENTER, RIGHT,
            BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT
        };

        /**
         * Gets this container area's ID.
         *
         * @return This container area's ID.
         */
        const char* getId() const;

        /**
         * Gets this container area's border.
         *
         * @return This container area's border.
         */
        const Theme::Border& getBorder() const;

        /**
         * Gets this container area's UVs.
         *
         * @return This container area's UVs.
         */
        const Theme::UVs& getUVs(ContainerArea area) const;

        /**
         * Gets this container area's color.
         *
         * @return This container area's color.
         */
        const Vector4& getColor() const;

    private:
        ContainerRegion(const Texture& texture, const Rectangle& region, const Theme::Border& border, const Vector4& color);
        ContainerRegion(const ContainerRegion& copy);
        ~ContainerRegion();

        static ContainerRegion* create(const char* id, const Texture& texture, const Rectangle& region, const Theme::Border& border, const Vector4& color);
    
        std::string _id;
        Theme::Border _border;
        UVs _uvs[MAX_OVERLAY_REGIONS];
        Vector4 _color;
    };
    
    /**
     * This class represents the appearance of a control.  A style can have padding and margin values,
     * as well as overlays for each of the control's states.  Each overlay in turn can reference
     * the above classes to determine the border, background, cursor, and icon settings to use for
     * a particular state.
     */
    class Style
    {
        friend class Theme;

    public:
        class Overlay;

        enum OverlayType
        {
            OVERLAY_NORMAL,
            OVERLAY_FOCUS,
            OVERLAY_ACTIVE,
            OVERLAY_DISABLED
        };

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
            friend class Theme;
            friend class Style;

        public:
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

            static Overlay* create();

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
        Style(const char* id, const Theme::Margin& margin, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active, Theme::Style::Overlay* disabled);
        Style(const Style& style);
        ~Style();
        
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
