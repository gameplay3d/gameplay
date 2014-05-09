#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "Control.h"
#include "Layout.h"
#include "TimeListener.h"

namespace gameplay
{

/**
 * Defines a container that contains zero or more controls.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class Container : public Control
{
    friend class Form;
    friend class Control;
    friend class ControlFactory;

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
     * Creates a new container.
     *
     * @param id The container ID.
     * @param style The container style (optional).
     * @param layout The container layout (optional).
     *
     * @return The new container.
     * @script{create}
     */
    static Container* create(const char* id, Theme::Style* style = NULL, Layout::Type layout = Layout::LAYOUT_ABSOLUTE);

    /**
     * Get this container's layout.
     *
     * @return This container's layout object.
     */
    Layout* getLayout();

	/**
	 * Sets the layout type for this container.
	 *
	 * @param type The new layout type for the container.
	 */
	void setLayout(Layout::Type type);

    /**
     * Adds a new control to this container.
     *
	 * @param control The control to add.
     *
     * @return The index assigned to the new Control.
     */
    unsigned int addControl(Control* control);

    /**
     * Inserts a control at a specific index.
     *
     * @param control The control to insert.
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
     * Returns the number of child controls for this container.
     *
     * @return The number of child controls.
     */
    unsigned int getControlCount() const;

    /**
     * Get the vector of controls within this container.
     *
     * @return The vector of the controls within this container.
     * @script{ignore}
     */
    const std::vector<Control*>& getControls() const;

    /**
     * Determines if this container is a top level form.
     *
     * @return True if the container is a top level form, false otherwise.
     */
    virtual bool isForm() const;

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
     * Stops this container from scrolling if it is currently being scrolled.
     */
    void stopScrolling();

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
     * Get an offset of how far this layout has been scrolled in each direction.
     */
    const Vector2& getScrollPosition() const;

    /**
     * Set an offset of how far this layout has been scrolled in each direction.
     */
    void setScrollPosition(const Vector2& scrollPosition);

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
     * @see Control::setFocus
     */
    bool setFocus();

    /**
     * Returns the currently active control for this container.
     *
     * @return This container's active control.
     */
    Control* getActiveControl() const;
    
    /**
     * Sets the active control for this container.
     *
     * A container's active control is the control that will receive focus
     * when the container receives focus.
     *
     * @param control The container's new active control (must be a child of this container).
     */
    void setActiveControl(Control* control);

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
     * Create a container with a given style and properties, including a list of controls.
     *
     * @param style The style to apply to this container.
     * @param properties A properties object containing a definition of the container and its nested controls (optional).
     *
     * @return The new container.
     */
    static Control* create(Theme::Style* style, Properties* properties = NULL);

    /**
     * @see Control::initialize
     */
    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * @see Control::update
     */
    void update(float elapsedTime);

    /**
     * @see Control::updateState
     */
    void updateState(State state);

    /**
     * @see Control::updateBounds
     */
    void updateBounds();

    /**
     * @see Control::updateAbsoluteBounds
     */
    void updateAbsoluteBounds(const Vector2& offset);

    /**
     * Updates the bounds for this container's child controls.
     */
    bool updateChildBounds();

    /**
     * Sets the specified dirty bits for all children within this container.
     *
     * @param bits The bits to set.
     * @param recursive If true, set the bits recursively on all children and their children.
     */
    void setChildrenDirty(int bits, bool recursive);

    /**
     * Gets a Layout::Type enum from a matching string.
     *
     * @param layoutString The layout string to parse
     * @return The parsed layout type.
     */
    static Layout::Type getLayoutType(const char* layoutString);

    /**
     * Creates a layout for the specified layout type.
     *
     * @param type The type of layout to create.
     * @return The new Layout.
     */
    static Layout* createLayout(Layout::Type type);

    /**
     * Adds controls nested within a properties object to this container.
     *
     * @param properties The properties to use.
     */
    void addControls(Properties* properties);

    /**
     * @see Control::draw
     */
    virtual unsigned int draw(Form* form, const Rectangle& clip);

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
     * The active control for the container.
     */
    Control* _activeControl;
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
        NEXT = 0x10,
        PREVIOUS = 0x20
    };

    static const int MAX_CONTACT_INDICES = 10;

    // Returns true on success; false if there are no controls to focus on,
    // in which case scrolling can be initiated.
    bool moveFocus(Direction direction);

	bool moveFocusNextPrevious(Direction direction);
	bool moveFocusDirectional(Direction direction);

    // Starts scrolling at the given horizontal and vertical speeds.
    void startScrolling(float x, float y, bool resetTime = true);

    void clearContacts();
    bool inContact();

    AnimationClip* _scrollBarOpacityClip;
    int _zIndexDefault;
    bool _selectButtonDown;
    double _lastFrameTime;

    float _totalWidth;
    float _totalHeight;
    bool _contactIndices[MAX_CONTACT_INDICES];
    bool _initializedWithScroll;
    bool _scrollWheelRequiresFocus;
};

}

#endif
