#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "Control.h"
#include "Layout.h"
#include "TimeListener.h"

namespace gameplay
{

/**
 * A container is a UI control that can contain other controls.
 *
 * The following properties are available for containers:

 @verbatim
    container <containerID>
    {
         // Container properties.
         layout   = <Layout::Type>        // A value from the Layout::Type enum.  E.g.: LAYOUT_VERTICAL
         style    = <styleID>           // A style from the form's theme.
         alignment   = <Control::Alignment constant> // Note: 'position' will be ignored.
         position    = <x, y>    // Position of the container on-screen, measured in pixels.
         autoWidth   = <bool>
         autoHeight  = <bool>
         size        = <width, height>   // Size of the container, measured in pixels.
         width       = <width>   // Can be used in place of 'size', e.g. with 'autoHeight = true'
         height      = <height>  // Can be used in place of 'size', e.g. with 'autoWidth = true'
         scroll      = <Container::Scroll constant> // Whether scrolling is allowed and in which directions.
         scrollBarsAutoHide = <bool>        // Whether scrollbars fade out when not in use.
         scrollingFriction = <float>        // Friction applied to inertial scrolling.
         scrollWheelRequiresFocus = <bool>  // Whether focus or hover state handles scroll-wheel events.
         scrollWheelSpeed = <float>         // Speed to scroll at on a scroll-wheel event.
         consumeEvents = <bool>             // Whether the container propagates input events to the Game's input event handler. Default is true.

         // All the nested controls within this container.
         container 
         { 
             ...
         }

         label { }
         textBox { }
         button { }
         checkBox { }
         radioButton { }
         slider { }
    }
 @endverbatim
 */
class Container : public Control, TimeListener
{

public:

    /**
     * Constant used to auto-hide scrollbars.
     */
    static const int ANIMATE_SCROLLBAR_OPACITY = 8;

    /**
     * The definition for container scrolling.
     */
    enum Scroll
    {
        SCROLL_NONE        = 0,
        SCROLL_HORIZONTAL  = 0x01,
        SCROLL_VERTICAL    = 0x02,
        SCROLL_BOTH = SCROLL_HORIZONTAL | SCROLL_VERTICAL
    };

    /**
     * Create a new container.
     *
     * @param id The container's ID.
     * @param style The container's style.
     * @param layoutType The container's layout type.
     *
     * @return The new container.
     * @script{create}
     */
    static Container* create(const char* id, Theme::Style* style, Layout::Type layoutType = Layout::LAYOUT_ABSOLUTE);

    /**
     * Get this container's layout.
     *
     * @return This container's layout object.
     */
    Layout* getLayout();

    /**
     * Add a control to this layout.
     * The control will be assigned the next available index.
     *
     * @param control The Control to add.
     *
     * @return The index assigned to the added Control.
     */
    unsigned int addControl(Control* control);

    /**
     * Insert a control at a specific index.
     *
     * @param control The control to add.
     * @param index The index at which to insert the control.
     */
    void insertControl(Control* control, unsigned int index);

    /**
     * Remove a control at a specific index.
     *
     * @param index The index from which to remove the control.
     */
    void removeControl(unsigned int index);

    /**
     * Remove a control with the given ID.
     *
     * @param id The ID of the control to remove.
     */
    void removeControl(const char* id);

    /**
     * Remove a specific control.
     *
     * @param control The control to remove.
     */
    void removeControl(Control* control);

    /**
     * Get the Control at a specific index.
     *
     * @param index The index at which to retrieve the Control.
     *
     * @return The Control at the given index.
     */
    Control* getControl(unsigned int index) const;

    /**
     * Get a Control with a specific ID that belongs to this Layout.
     *
     * @param id The ID of the Control to search for.
     */
    Control* getControl(const char* id) const;

    /**
     * Get the vector of controls within this container.
     *
     * @return The vector of the controls within this container.
     * @script{ignore}
     */
    const std::vector<Control*>& getControls() const;

    /**
     * Sets the allowed scroll directions for this container.
     *
     * @param scroll The allowed scroll directions for this container.
     */
    void setScroll(Scroll scroll);

    /**
     * Gets the allowed scroll directions for this container.
     *
     * @return The allowed scroll directions for this container.
     */
    Scroll getScroll() const;

    /**
     * Set whether scrollbars auto hidden when they become static.
     *
     * @param autoHide true to auto hide the scrollbars when they become static.
     */
    void setScrollBarsAutoHide(bool autoHide);

    /**
     * Whether scrollbars are always visible, or only visible while scrolling.
     *
     * @return Whether scrollbars are always visible.
     */
    bool isScrollBarsAutoHide() const;

    /**
     * Whether this container is currently being scrolled.
     *
     * @return Whether this container is currently being scrolled.
     */
    bool isScrolling() const;

    /**
     * Get the friction applied to scrolling velocity for this container.
     */
    float getScrollingFriction() const;

    /**
     * Get the friction applied to scrolling velocity for this container.
     * A higher value will bring the viewport to a stop sooner.
     */
    void setScrollingFriction(float friction);

    /**
     * Get the speed added to scrolling velocity on a scroll-wheel event.
     */
    float getScrollWheelSpeed() const;

    /**
     * Set the speed added to scrolling velocity on a scroll-wheel event.
     */
    void setScrollWheelSpeed(float speed);

    /**
     * @see AnimationTarget::getAnimation
     */
    Animation* getAnimation(const char* id = NULL) const;

    /**
     * @see Control::isContainer
     */
    bool isContainer() const;

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Get whether this container requires focus in order to handle scroll-wheel events.
     */
    bool getScrollWheelRequiresFocus() const;

    /**
     * Set whether this container requires focus in order to handle scroll-wheel events.
     * If this property is set to true, scroll-wheel events will only be handled when the container has focus.
     * If this property is set tofalse, scroll-wheel events will only be handled
     * when the container is in the HOVER state.
     *
     * @param required Whether focus is required in order to handle scroll-wheel events.
     */
    void setScrollWheelRequiresFocus(bool required);

    /**
     * @see AnimationTarget::getAnimationPropertyComponentCount
     */
    virtual unsigned int getAnimationPropertyComponentCount(int propertyId) const;

    /**
     * @see AnimationTarget::getAnimationProperty
     */
    virtual void getAnimationPropertyValue(int propertyId, AnimationValue* value);

    /**
     * @see AnimationTarget::setAnimationProperty
     */
    virtual void setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight = 1.0f);

    /**
     * @see TimeListener::timeEvent
     *
     * @script{ignore}
     */
    void timeEvent(long timeDiff, void* cookie);

protected:

    /**
     * Constructor.
     */
    Container();

    /**
     * Destructor.
     */
    virtual ~Container();

    /**
     * Create an empty container.  A container's layout type must be specified at creation time.
     *
     * @param type The container's layout type.
     *
     * @return The new container.
     */
    static Container* create(Layout::Type type);

    /**
     * Create a container with a given style and properties, including a list of controls.
     *
     * @param style The style to apply to this container.
     * @param properties The properties to set on this container, including nested controls.
     * @param theme The theme to search for control styles within.
     *
     * @return The new container.
     */
    static Container* create(Theme::Style* style, Properties* properties, Theme* theme);

    /**
     * Updates each control within this container,
     * and positions them according to the container's layout.
     *
     * @param container This container's parent container.
     * @param offset The offset.
     */
    virtual void update(const Control* container, const Vector2& offset);

    /**
     * Touch callback on touch events.  Controls return true if they consume the touch event.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by a control within this container.
     *
     * @see Touch::TouchEvent
     */
    virtual bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    /**
     * Keyboard callback on key events.  Passes key events on to the currently focused control.
     *
     * @param evt The key event that occurred.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     *
     * @return Whether the key event was consumed by this control.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual bool keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Mouse callback on mouse events.
     *
     * @param evt The mouse event that occurred.
     * @param x The x position of the mouse in pixels. Left edge is zero.
     * @param y The y position of the mouse in pixels. Top edge is zero.
     * @param wheelDelta The number of mouse wheel ticks. Positive is up (forward), negative is down (backward).
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::mouseEvent
     */
    virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Gamepad callback on gamepad events.
     *
     * @see Control::gamepadEvent
     */
    virtual bool gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Gets a Layout::Type enum from a matching string.
     *
     * @param layoutString The layout string to parse
     */
    static Layout::Type getLayoutType(const char* layoutString);
    
    /**
     * Returns whether this container or any of its controls have been modified and require an update.
     * 
     * @return true if this container or any of its controls have been modified and require an update.
     */
    virtual bool isDirty();

    /**
     * Adds controls nested within a properties object to this container,
     * searching for styles within the given theme.
     *
     * @param theme The them to add controls from
     * @param properties The properties to use.
     */
    void addControls(Theme* theme, Properties* properties);

    /**
     * Draws a sprite batch for the specified clipping rect.
     *
     * @param spriteBatch The sprite batch to use.
     * @param clip The clipping rectangle.
     * @param needsClear Whether it needs to be cleared.
     * @param cleared Whether it was previously cleared
     * @param targetHeight The targets height
     */
    virtual void draw(SpriteBatch* spriteBatch, const Rectangle& clip, bool needsClear, bool cleared, float targetHeight);

    /**
     * Update scroll position and velocity.
     */
    void updateScroll();

    /**
     * Sorts controls by Z-Order (for absolute layouts only).
     * This method is used by controls to notify their parent container when
     * their Z-Index changes.
     */
    void sortControls();

    /**
     * Applies touch events to scroll state.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by scrolling within this container.
     *
     * @see Touch::TouchEvent
     */
    bool touchEventScroll(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Mouse scroll event callback.
     *
     * @param evt The mouse scroll event that occurred.
     * @param x The x position of the scroll in pixels. Left edge is zero.
     * @param y The y position of the scroll in pixels. Top edge is zero.
     * @param wheelDelta The value change of the mouse's scroll wheel.
     *
     * @return Whether the scroll event was consumed by scrolling within this container.
     *
     * @see Mouse::MouseEvent
     */
    bool mouseEventScroll(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Mouse pointer event callback.
     *
     * @param mouse Whether to treat the event as a mouse event or a touch event.
     * @param evt The pointer event (either a Mouse::MouseEvent or a Touch::TouchEvent).
     * @param x The x position of the pointer event in pixels. Left edge is zero.
     * @param y The y position of the pointer event in pixels. Top edge is zero.
     * @param data The event's data (depends on whether it is a mouse event or a touch event).
     *
     * @return Whether the pointer event was consumed by this container.
     * 
     * @see Mouse::MouseEvent
     * @see Touch::TouchEvent
     */
    bool pointerEvent(bool mouse, char evt, int x, int y, int data);

    /**
     * Get a Scroll enum from a matching string.
     *
     * @param scroll A string representing a Scroll enum.
     *
     * @return The Scroll enum value that matches the given string.
     */
    static Scroll getScroll(const char* scroll);

    /**
     * The container's layout.
     */
    Layout* _layout;
    /**
     * List of controls within the container.
     */
    std::vector<Control*> _controls;
    /**
     * Scrollbar top cap image.
     */
    Theme::ThemeImage* _scrollBarTopCap;
    /**
     * Scrollbar vertical track image.
     */
    Theme::ThemeImage* _scrollBarVertical;
    /**
     * Scrollbar bottom cap image.
     */
    Theme::ThemeImage* _scrollBarBottomCap;
    /**
     * Scrollbar left cap image.
     */
    Theme::ThemeImage* _scrollBarLeftCap;
    /**
     * Scrollbar horizontal track image.
     */
    Theme::ThemeImage* _scrollBarHorizontal;
    /**
     * Scrollbar horizontal image.
     */
    Theme::ThemeImage* _scrollBarRightCap;
    /** 
     * Flag representing whether scrolling is enabled, and in which directions.
     */
    Scroll _scroll;
    /** 
     * Scroll bar bounds.
     */
    Rectangle _scrollBarBounds;
    /** 
     * How far this layout has been scrolled in each direction.
     */
    Vector2 _scrollPosition;
    /** 
     * Whether the scrollbars should auto-hide. Default is false.
     */
    bool _scrollBarsAutoHide;
    /** 
     * Used to animate scrollbars fading out.
     */
    float _scrollBarOpacity;
    /** 
     * Whether the user is currently touching / holding the mouse down within this layout's container.
     */
    bool _scrolling;
    /** 
     * First scrolling touch x position.
     */
    int _scrollingVeryFirstX;
    /**
     * First scrolling touch y position.
     */
    int _scrollingVeryFirstY;
    /**
     * First scrolling touch x position since last change in direction.
     */ 
    int _scrollingFirstX;
    /** 
     * First scrolling touch y position since last change in direction.
     */ 
    int _scrollingFirstY;
    /** 
     * The last y position when scrolling.
     */ 
    int _scrollingLastX;
    /** 
     * The last x position when scrolling.
     */ 
    int _scrollingLastY;
    /** 
     * Time we started scrolling horizontally.
     */ 
    double _scrollingStartTimeX;
    /** 
     * Time we started scrolling vertically.
     */ 
    double _scrollingStartTimeY;
    /** 
     * The last time we were scrolling.
     */
    double _scrollingLastTime;
    /** 
     * Speed to continue scrolling at after touch release or a scroll-wheel event.
     */ 
    Vector2 _scrollingVelocity;
    /** 
     * Friction dampens velocity.
     */ 
    float _scrollingFriction;
    /**
     * Amount to add to scrolling velocity on a scroll-wheel event;
     */
    float _scrollWheelSpeed;
    /** 
     * Are we scrolling to the right?
     */ 
    bool _scrollingRight;
    /** 
     * Are we scrolling down?
     */ 
    bool _scrollingDown;
    /**
     * Locked to scrolling vertically by grabbing the scrollbar with the mouse.
     */
    bool _scrollingMouseVertically;
    /**
     * Locked to scrolling horizontally by grabbing the scrollbar with the mouse.
     */
    bool _scrollingMouseHorizontally;

private:

    /**
     * Constructor.
     */
    Container(const Container& copy);

    enum Direction
    {
        UP = 0x01,
        DOWN = 0x02,
        LEFT = 0x04,
        RIGHT = 0x08,
        NEXT = 0x10
    };

    // Returns true on success; false if there are no controls to focus on,
    // in which case scrolling can be initiated.
    bool moveFocus(Direction direction, Control* outsideControl = NULL);

    void guaranteeFocus(Control* inFocus);

    // Starts scrolling at the given horizontal and vertical speeds.
    void startScrolling(float x, float y, bool resetTime = true);

    void stopScrolling();

    AnimationClip* _scrollBarOpacityClip;
    int _zIndexDefault;
    int _focusIndexDefault;
    int _focusIndexMax;
    unsigned int _focusPressed;
    bool _selectButtonDown;
    double _lastFrameTime;

    // Timing information for repeating focus changes.
    bool _focusChangeRepeat;
    double _focusChangeStartTime;
    double _focusChangeRepeatDelay;
    unsigned int _focusChangeCount;
    Direction _direction;

    float _totalWidth;
    float _totalHeight;
    int _contactIndices;
    bool _initializedWithScroll;
    bool _scrollWheelRequiresFocus;
};

}

#endif
