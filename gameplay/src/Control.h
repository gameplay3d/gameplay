#ifndef CONTROL_H_
#define CONTROL_H_

#include "Ref.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "SpriteBatch.h"
#include "Theme.h"
#include "Touch.h"
#include "Keyboard.h"

/**
 * Default duration of UI animations.
 */
#define DEFAULT_UI_ANIMATION_DURATION 200L

namespace gameplay
{

/**
 * Base class for UI controls.
 */
class Control : public Ref, public AnimationTarget
{
    friend class Form;
    friend class Container;
    friend class Layout;
    friend class AbsoluteLayout;
    friend class VerticalLayout;

public:
    /**
     * The possible states a control can be in.
     */
    enum State
    {
        NORMAL = 0x01,
        FOCUS = 0x02,
        ACTIVE = 0x04,
        DISABLED = 0x08,
    };

    // Only for setting control states
    static const unsigned char STATE_ALL = NORMAL | FOCUS | ACTIVE | DISABLED;

    class Listener
    {
    public:
        enum EventType
        {
            PRESS           = 0x01,
            RELEASE         = 0x02,
            CLICK           = 0x04,
            VALUE_CHANGED   = 0x08,
            TEXT_CHANGED    = 0x10
        };

        virtual void controlEvent(Control* control, EventType evt) = 0;
    };

    /**
     * Position animation property. Data = x, y
     */
    static const int ANIMATE_POSITION = 1;

    /**
     * Position x animation property. Data = x
     */
    static const int ANIMATE_POSITION_X = 2;

    /**
     * Position y animation property. Data = y
     */
    static const int ANIMATE_POSITION_Y = 3;

    /**
     * Size animation property.  Data = width, height
     */
    static const int ANIMATE_SIZE = 4;

    /**
     * Size width animation property.  Data = width
     */
    static const int ANIMATE_SIZE_WIDTH = 5;

    /**
     * Size height animation property.  Data = height
     */
    static const int ANIMATE_SIZE_HEIGHT = 6;

    /**
     * Opacity property.  Data = opacity
     */
    static const int ANIMATE_OPACITY = 7;

    /**
     * Get this control's ID string.
     *
     * @return This control's ID.
     */
    const char* getID() const;

    /**
     * Set the position of this control relative to its parent container.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setPosition(float x, float y, unsigned long duration = DEFAULT_UI_ANIMATION_DURATION);

    /**
     * Get the position of this control relative to its parent container.
     *
     * @return The position of this control relative to its parent container.
     */
    const Vector2& getPosition() const;

    /**
     * Set the desired size of this control, including its border and padding, before clipping.
     *
     * @param width The width.
     * @param height The height.
     */
    void setSize(float width, float height, unsigned long duration = DEFAULT_UI_ANIMATION_DURATION);

    /**
     * Get the desired size of this control, including its border and padding, before clipping.
     *
     * @return The size of this control.
     */
    const Vector2& getSize() const;

    // Themed properties.
    
    //void setBorder(const Theme::Border& border, unsigned char states = STATE_ALL);
    /**
     * Set the size of this control's border.
     *
     * @param top The height of the border's top side.
     * @param bottom The height of the border's bottom side.
     * @param left The width of the border's left side.
     * @param right The width of the border's right side.
     */
    void setBorder(float top, float bottom, float left, float right, unsigned char states = STATE_ALL);

    /**
     * Get the measurements of this control's border for a given state. 
     *
     * @return This control's border.
     */
    const Theme::Border& getBorder(State state = NORMAL) const;

    /**
     * Set the texture region of this control's skin.
     *
     * @param region The texture region, in pixels.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setSkinRegion(const Rectangle& region, unsigned char states = STATE_ALL);

    /**
     * Get the texture region of this control's skin for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return The texture region of this control's skin.
     */
    const Rectangle& getSkinRegion(State state = NORMAL) const;

    /**
     * Get the texture coordinates of an area of this control's skin for a given state.
     *
     * @param area The area of the skin to get the coordinates of.
     * @param state The state to get this property from.
     *
     * @return The texture coordinates of an area of this control's skin.
     */
    const Theme::UVs& getSkinUVs(Theme::Skin::SkinArea area, State state = NORMAL) const;

    /**
     * Set the blend color of this control's skin.
     *
     * @param color The new blend color.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setSkinColor(const Vector4& color, unsigned char states = STATE_ALL);

    /**
     * Get the blend color of this control's skin for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return The blend color of this control's skin.
     */
    const Vector4& getSkinColor(State state = NORMAL) const;

    /**
     * Set this control's margin.
     *
     * @param top Height of top margin.
     * @param bottom Height of bottom margin.
     * @param left Width of left margin.
     * @param right Width of right margin.
     */
    void setMargin(float top, float bottom, float left, float right);

    /**
     * Get this control's margin.
     *
     * @return This control's margin.
     */
    const Theme::Margin& getMargin() const;

    /**
     * Set this control's padding.
     *
     * @param top Height of top padding.
     * @param bottom Height of bottom padding.
     * @param left Width of left padding.
     * @param right Width of right padding.
     */
    void setPadding(float top, float bottom, float left, float right);

    /**
     * Get this control's padding.
     *
     * @return This control's padding.
     */
    const Theme::Padding& getPadding() const;

    /**
     * Set the texture region of an image used by this control.
     *
     * @param id The ID of the image to modify.
     * @param region The new texture region of the image.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setImageRegion(const char* id, const Rectangle& region, unsigned char states = STATE_ALL);

    /**
     * Get the texture region of an image used by this control for a given state.
     *
     * @param id The ID of the image.
     * @param state The state to get this property from.
     *
     * @return The texture region of the specified image.
     */
    const Rectangle& getImageRegion(const char* id, State state) const;

    /**
     * Set the blend color of an image used by this control.
     *
     * @param id The ID of the image to modify.
     * @param color The new blend color of the image.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setImageColor(const char* id, const Vector4& color, unsigned char states = STATE_ALL);

    /**
     * Get the blend color of an image used by this control for a given state.
     *
     * @param id The ID of the image.
     * @param state The state to get this property from.
     *
     * @return The blend color of the specified image.
     */
    const Vector4& getImageColor(const char* id, State state) const;

    /**
     * Get the texture coordinates of an image used by this control for a given state.
     *
     * @param id The ID of the image.
     * @param state The state to get this property from.
     *
     * @return The texture coordinates of the specified image.
     */
    const Theme::UVs& getImageUVs(const char* id, State state) const;

    /**
     * Set the texture region of this control's cursor.
     *
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setCursorRegion(const Rectangle& region, unsigned char states);

    /**
     * Get the texture region of this control's cursor for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return The texture region of this control's cursor.
     */
    const Rectangle& getCursorRegion(State state) const;

    /**
     * Set the blend color of this control's cursor.
     *
     * @param color The new blend color.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setCursorColor(const Vector4& color, unsigned char states);

    /**
     * Get the blend color of this control's cursor for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return The blend color of this control's cursor.
     */
    const Vector4& getCursorColor(State state);
    
    /**
     * Get the texture coordinates of this control's cursor for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return The texture coordinates of this control's cursor.
     */
    const Theme::UVs& getCursorUVs(State state);

    /**
     * Set the font used by this control.
     *
     * @param font The new font to use.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setFont(Font* font, unsigned char states = STATE_ALL);

    /**
     * Get the font used by this control for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return the font used by this control.
     */
    Font* getFont(State state = NORMAL) const;

    /**
     * Set this control's font size.
     *
     * @param size The new font size.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setFontSize(unsigned int size, unsigned char states = STATE_ALL);

    /**
     * Get this control's font size for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return This control's font size.
     */
    unsigned int getFontSize(State state = NORMAL) const;

    /**
     * Set this control's text color.
     *
     * @param color The new text color.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setTextColor(const Vector4& color, unsigned char states = STATE_ALL);

    /**
     * Get this control's text color for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return This control's text color.
     */
    const Vector4& getTextColor(State state = NORMAL) const;

    /**
     * Set this control's text alignment.
     *
     * @param alignment The new text alignment.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setTextAlignment(Font::Justify alignment, unsigned char states = STATE_ALL);

    /**
     * Get this control's text alignment for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return This control's text alignment for the given state.
     */
    Font::Justify getTextAlignment(State state = NORMAL) const;

    /**
     * Set whether text is drawn from right to left within this control.
     *
     * @param rightToLeft Whether text is drawn from right to left within this control.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     */
    void setTextRightToLeft(bool rightToLeft, unsigned char states = STATE_ALL);

    /**
     * Get whether text is drawn from right to left within this control, for a given state.
     *
     * @param state The state to get this property from.
     *
     * @return Whether text is drawn from right to left within this control, for the given state.
     */
    bool getTextRightToLeft(State state = NORMAL) const;

    /**
     * Set the opacity of this control.
     *
     * @param opacity The new opacity.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
     * @param duration The duration to animate opacity by.
     */
    void setOpacity(float opacity, unsigned char states = STATE_ALL, unsigned long duration = DEFAULT_UI_ANIMATION_DURATION);

    /**
     * Get the opacity of this control for a given state. 
     *
     * @param state The state to get this property from.
     *
     * @return The opacity of this control for a given state.
     */
    float getOpacity(State state = NORMAL) const;

    // TODO
    // Controls must state the names of the images they use, for the purposes of a future UI editor.
    //virtual std::vector<std::string> getImageNames() = 0;

    // Control state.
    /**
     * Get the bounds of this control, relative to its parent container, after clipping.
     *
     * @return The bounds of this control.
     */
    const Rectangle& getBounds() const;

    /**
     * Get the content area of this control, in screen coordinates, after clipping.
     *
     * @return The clipping area of this control.
     */
    const Rectangle& getClip() const;

    /**
     * Set width and/or height to auto-size to size a Control to tightly fit
     * its text and themed visual elements (CheckBox / RadioButton toggle etc.).
     *
     * Similarly set this on the width and/or height of a Container to tightly fit
     * the Container around STATE_ALL its children.
     *
     * @param width Whether to automatically determine this Control's width.
     * @param height Whether to automatically determine this Control's height.
     */
    void setAutoSize(bool width, bool height);

    /**
     * Change this control's state.
     *
     * @param state The state to switch this control to.
     */
    void setState(State state);

    /**
     * Get this control's current state.
     *
     * @return This control's current state.
     */
    State getState() const;

    /**
     * Disable this control.
     */
    void disable();

    /**
     * Enable this control.
     */
    void enable();

    /**
     * Get whether this control is currently enabled.
     *
     * @return Whether this control is currently enabled.
     */
    bool isEnabled();

    /**
     * Set whether this control consumes touch events,
     * preventing them from being passed to the game.
     *
     * @param consume Whether this control consumes touch events.
     */
    void setConsumeTouchEvents(bool consume);

    /**
     * Get whether this control consumes touch events.
     *
     * @return Whether this control consumes touch events.
     */
    bool getConsumeTouchEvents();

    /**
     * Set the style this control will use when rendering.
     *
     * @param style The style this control will use when rendering.
     */
    void setStyle(Theme::Style* style);

    /**
     * Get this control's style.
     *
     * @return This control's style.
     */
    Theme::Style* getStyle() const;

    /**
     * Add a listener to be notified of specific events affecting
     * this control.  Event types can be OR'ed together.
     * E.g. To listen to touch-press and touch-release events,
     * pass <code>Control::Listener::TOUCH | Control::Listener::RELEASE</code>
     * as the second parameter.
     *
     * @param listener The listener to add.
     * @param eventFlags The events to listen for.
     */
    virtual void addListener(Control::Listener* listener, int eventFlags);

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

protected:
    Control();
    virtual ~Control();

    /**
     * Get the overlay type corresponding to this control's current state.
     *
     * @return The overlay type corresponding to this control's current state.
     */
    Theme::Style::OverlayType getOverlayType() const;

    /**
     * Touch callback on touch events.  Controls return true if they consume the touch event.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by this control.
     *
     * @see Touch::TouchEvent
     */
    virtual bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Keyboard callback on key events.
     *
     * @param evt The key event that occured.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Called when a control's properties change.  Updates this control's internal rendering
     * properties, such as its text viewport.
     *
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void update(const Rectangle& clip);

private:
    /**
     * Draws the themed border and background of a control.
     *
     * @param spriteBatch The sprite batch containing this control's border images.
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip);

protected:
    /**
     * Draw the images associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draw this control's text.
     *
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void drawText(const Rectangle& clip);

    /**
     * Initialize properties common to STATE_ALL Controls.
     */
    virtual void init(Theme::Style* style, Properties* properties);

    /**
     * Container and classes that extend it should implement this and return true.
     */
    virtual bool isContainer();

    /**
     * Returns whether this control has been modified and requires an update.
     */
    virtual bool isDirty();

    /**
     * Get a Control::State enum from a matching string.
     */
    static State getStateFromString(const char* state);

    /**
     * Notify STATE_ALL listeners of a specific event.
     */
    void notifyListeners(Listener::EventType eventType);

    void addSpecificListener(Control::Listener* listener, Listener::EventType eventType);

    std::string _id;
    State _state;           // Determines overlay used during draw().
    Vector2 _position;      // Position, relative to parent container's clipping window.
    Vector2 _size;          // Desired size.  Will be clipped.
    Rectangle _bounds;      // The position and size of this control, relative to parent container's bounds, including border and padding, after clipping.
    Rectangle _textBounds;  // The position and size of this control's text area, before clipping.  Used for text alignment.
    Rectangle _clip;        // Clipping window of this control's content, after clipping.
    bool _autoWidth;
    bool _autoHeight;
    bool _dirty;
    bool _consumeTouchEvents;
    Theme::Style* _style;
    typedef std::map<Listener::EventType, std::list<Listener*>*> ListenerMap;
    ListenerMap* _listeners;

private:
    // Animation blending bits.
    static const char ANIMATION_POSITION_X_BIT = 0x01;
    static const char ANIMATION_POSITION_Y_BIT = 0x02;
    static const char ANIMATION_SIZE_WIDTH_BIT = 0x04;
    static const char ANIMATION_SIZE_HEIGHT_BIT = 0x08;
    static const char ANIMATION_OPACITY_BIT = 0x10;

    bool _styleOverridden;

    void applyAnimationValuePositionX(float x, float blendWeight);
    void applyAnimationValuePositionY(float y, float blendWeight);
    void applyAnimationValueSizeWidth(float width, float blendWeight);
    void applyAnimationValueSizeHeight(float height, float blendWeight);
    void applyAnimationValueOpacity();

    // Gets the overlays requested in the overlayTypes bitflag.
    Theme::Style::Overlay** getOverlays(unsigned char overlayTypes, Theme::Style::Overlay** overlays);

    /**
     * Gets an overlay from a control state.
     */
    Theme::Style::Overlay* getOverlay(Control::State state) const;

    // Ensures that this control has a copy of its style so that it can override it without affecting other controls.
    void overrideStyle();

    Control(const Control& copy);
};

}

#endif
