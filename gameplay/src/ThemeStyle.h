#ifndef THEMESTYLE_H_
#define THEMESTYLE_H_

#include "Base.h"
#include "Ref.h"
#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Properties.h"
#include "Theme.h"

namespace gameplay
{

/**
 * This class represents the appearance of a control.  A style can have padding and margin values,
 * as well as overlays for each of the control's states.  Each overlay in turn can reference
 * other theme classes to determine the border, background, cursor, and image settings to use for
 * a particular state, as well as color and font settings, etcetera.
 */
class Theme::Style
{
    friend class Theme;
    friend class Control;
    friend class Container;
    friend class Form;

public:

    /**
     * Get the theme this style belongs to.
     *
     * @return The theme this style belongs to.
     */
    Theme* getTheme() const;

private:

    /**
     * A style has one overlay for each possible control state.
     */
    enum OverlayType
    {
        OVERLAY_NORMAL,
        OVERLAY_FOCUS,
        OVERLAY_ACTIVE,
        OVERLAY_DISABLED,
        OVERLAY_HOVER,
        OVERLAY_MAX
    };

    /**
     * This class represents a control's overlay for one of its states.
     */
    class Overlay : public Ref, public AnimationTarget
    {
        friend class Theme;
        friend class Theme::Style;
        friend class Control;
        friend class Container;
        friend class Form;

    private:

        static const int ANIMATE_OPACITY = 1;

        Overlay();
            
        Overlay(const Overlay& copy);
            
        ~Overlay();

        /**
         * Hidden copy assignment operator.
         */
        Overlay& operator=(const Overlay&);

        static Overlay* create();

        OverlayType getType();

        float getOpacity() const;

        void setOpacity(float opacity);

        void setBorder(float top, float bottom, float left, float right);

        const Theme::Border& getBorder() const;

        void setSkinColor(const Vector4& color);

        const Vector4& getSkinColor() const;

        void setSkinRegion(const Rectangle& region, float tw, float th);

        const Rectangle& getSkinRegion() const;

        const Theme::UVs& getSkinUVs(Theme::Skin::SkinArea area) const;

        Font* getFont() const;

        void setFont(Font* font);

        unsigned int getFontSize() const;

        void setFontSize(unsigned int fontSize);

        Font::Justify getTextAlignment() const;

        void setTextAlignment(Font::Justify alignment);
            
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

        const Theme::UVs& getCursorUVs() const;

        /**
         * @see AnimationTarget::getAnimationPropertyComponentCount
         */
        unsigned int getAnimationPropertyComponentCount(int propertyId) const;

        /**
         * @see AnimationTarget::getAnimationProperty
         */
        void getAnimationPropertyValue(int propertyId, AnimationValue* value);

        /**
         * @see AnimationTarget::setAnimationProperty
         */
        void setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight = 1.0f);
       
        void setSkin(Theme::Skin* Skin);

        Theme::Skin* getSkin() const;

        void setCursor(Theme::ThemeImage* cursor);
            
        Theme::ThemeImage* getCursor() const;
            
        void setImageList(Theme::ImageList* imageList);
            
        Theme::ImageList* getImageList() const;
        
        Skin* _skin;
        Theme::ThemeImage* _cursor;
        Theme::ImageList* _imageList;
        Font* _font;
        unsigned int _fontSize;
        Font::Justify _alignment;
        bool _textRightToLeft;
        Vector4 _textColor;
        float _opacity;
    };

    /**
     * Constructor.
     */
    Style(Theme* theme, const char* id, float tw, float th,
          const Theme::Margin& margin, const Theme::Padding& padding,
          Overlay* normal, Overlay* focus, Overlay* active, Overlay* disabled, Overlay* hover);

    /**
     * Constructor.
     */
    Style(const Style& style);

    /**
     * Destructor.
     */
    ~Style();

    /**
     * Hidden copy assignment operator.
     */
    Style& operator=(const Style&);

    /**
     * Returns the Id of this Style.
     */
    const char* getId() const;

    /**
     * Gets an overlay from the overlay type.
     */
    Overlay* getOverlay(OverlayType overlayType) const;

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
    
    Theme* _theme;
    std::string _id;
    float _tw;
    float _th;
    Theme::Margin _margin;
    Theme::Padding _padding;
    Overlay* _overlays[OVERLAY_MAX];
};

}

#endif