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
 * all the Icon, Cursor, Slider, and Skin sprites used by the theme.  Each set of sprites within
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
 *    Skin <SkinID>
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
 *        region  =   <x, y, width, height>   // Total Skin region including entire border.
 *        color   =   <#ffffffff>             // Tint to apply to Skin sprites.
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
 *            Skin   =   <SkinID>               // Skin to use for border and background sprites.
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
    friend class Container;
    friend class Form;
    friend class Skin;

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
    typedef class UVs
    {
    public:
        UVs();
        UVs(float u1, float v1, float u2, float v2);

        static const UVs& empty();

        float u1;
        float v1;
        float u2;
        float v2;
    } UVs;

    /**
     * Struct representing margin, border, and padding areas by
     * the width or height of each side.
     */
    typedef class SideRegions
    {
    public:
        SideRegions() : top(0), bottom(0), left(0), right(0) {}

        static const SideRegions& empty();

        float top;
        float bottom;
        float left;
        float right;
    } Margin, Border, Padding;

    class Image : public Ref
    {
        friend class Theme;
        friend class Control;

    public:
        const char* getId() const;

        const UVs& getUVs() const;

        const Rectangle& getRegion() const;

        const Vector4& getColor() const;

    private:
        Image(float tw, float th, const Rectangle& region, const Vector4& color);
        ~Image();

        static Image* create(float tw, float th, Properties* properties);

        std::string _id;
        UVs _uvs;
        Rectangle _region;
        Vector4 _color;
    };

    class ImageList : public Ref
    {
        friend class Theme;
        friend class Control;

    public:
        const char* getId() const;

        Image* getImage(const char* imageId) const;

    private:
        ImageList(const Vector4& color);
        ImageList(const ImageList& copy);
        ~ImageList();

        static ImageList* create(float tw, float th, Properties* properties);

        std::string _id;
        std::vector<Image*> _images;
        Vector4 _color;
    };

    /**
     * A skin defines the border and background of a control.
     */
    class Skin : public Ref
    {
        friend class Theme;

    public:
        enum SkinArea
        {
            TOP_LEFT, TOP, TOP_RIGHT,
            LEFT, CENTER, RIGHT,
            BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT
        };

        /**
         * Gets this skin's ID.
         *
         * @return This skin's ID.
         */
        const char* getId() const;

        /**
         * Gets this skin's border.
         *
         * @return This skin's border.
         */
        const Theme::Border& getBorder() const;

        const Rectangle& getRegion() const;

        /**
         * Gets this skin's UVs.
         *
         * @return This skin's UVs.
         */
        const Theme::UVs& getUVs(SkinArea area) const;

        /**
         * Gets this skin's color.
         *
         * @return This skin's color.
         */
        const Vector4& getColor() const;

    private:
        Skin(float tw, float th, const Rectangle& region, const Theme::Border& border, const Vector4& color);
        //Skin(const Skin& copy);
        ~Skin();

        static Skin* create(const char* id, float tw, float th, const Rectangle& region, const Theme::Border& border, const Vector4& color);

        void setRegion(const Rectangle& region, float tw, float th);
    
        std::string _id;
        Theme::Border _border;
        UVs _uvs[MAX_OVERLAY_REGIONS];
        Vector4 _color;
        Rectangle _region;
        float _tw, _th;
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
        friend class Control;

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
        class Overlay : public Ref, public AnimationTarget
        {
            friend class Theme;
            friend class Style;

        public:
            /**
             * Animate this overlay's opacity property.  Data = opacity
             */
            static const int ANIMATE_OPACITY = 1;

           /**
            * Returns the Overlay type.
            */
            OverlayType getType();

            float getOpacity() const;
            void setOpacity(float opacity);

            // setBorder(const Theme::Border& border) ??
            void setBorder(float top, float bottom, float left, float right);
            const Border& getBorder() const;

            void setSkinColor(const Vector4& color);
            const Vector4& getSkinColor() const;

            void setSkinRegion(const Rectangle& region, float tw, float th);
            const Rectangle& getSkinRegion() const;

            const Theme::UVs& getSkinUVs(Theme::Skin::SkinArea area) const;

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

            const Rectangle& getImageRegion(const char* id) const;
            void setImageRegion(const char* id, const Rectangle& region, float tw, float th);

            const Vector4& getImageColor(const char* id) const;
            void setImageColor(const char* id, const Vector4& color);

            const Theme::UVs& getImageUVs(const char* id) const;

            const Rectangle& getCursorRegion() const;
            void setCursorRegion(const Rectangle& region, float tw, float th);

            const Vector4& getCursorColor() const;
            void setCursorColor(const Vector4& color);

            const Theme::UVs getCursorUVs() const;

            /**
             * @see AnimationTarget#getAnimationPropertyComponentCount
             */
            unsigned int getAnimationPropertyComponentCount(int propertyId) const;

            /**
             * @see AnimationTarget#getAnimationProperty
             */
            void getAnimationPropertyValue(int propertyId, AnimationValue* value);

            /**
             * @see AnimationTarget#setAnimationProperty
             */
            void setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight = 1.0f);
        
        private:
            Overlay();
            Overlay(const Overlay& copy);
            ~Overlay();

            static Overlay* create();

            void setSkin(Skin* Skin);
            Skin* getSkin() const;

            void setCursor(Image* cursor);
            Image* getCursor() const;
            
            void setImageList(ImageList* imageList);
            ImageList* getImageList() const;

            static const char ANIMATION_OPACITY_BIT = 0x01;
            void applyAnimationValueOpacity(float opacity, float blendWeight);

            Skin* _skin;
            Image* _cursor;
            ImageList* _imageList;
            Font* _font;
            unsigned int _fontSize;
            Font::Justify _alignment;
            bool _textRightToLeft;
            Vector4 _textColor;
            float _opacity;
        };

    private:
        Style(const char* id, float tw, float th,
            const Theme::Margin& margin, const Theme::Padding& padding,
            Theme::Style::Overlay* normal, Theme::Style::Overlay* focus, Theme::Style::Overlay* active, Theme::Style::Overlay* disabled);
        Style(const Style& style);
        ~Style();
        
        std::string _id;
        Margin _margin;
        Padding _padding;
        Overlay* _overlays[MAX_OVERLAYS];
        float _tw;
        float _th;
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

    static void generateUVs(float tw, float th, float x, float y, float width, float height, UVs* uvs);
    void lookUpSprites(const Properties* overlaySpace, ImageList** imageList, Image** mouseCursor, Skin** skin);

    std::string _path;
    Texture* _texture;
    SpriteBatch* _spriteBatch;
    std::vector<Style*> _styles;
    std::vector<Image*> _images;
    std::vector<ImageList*> _imageLists;
    std::vector<Skin*> _skins;
    std::set<Font*> _fonts;
};

}

#endif
