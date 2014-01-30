#ifndef CONTROL_H_
#define CONTROL_H_

#include "Ref.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "Theme.h"
#include "ThemeStyle.h"
#include "Touch.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ScriptTarget.h"
#include "Gamepad.h"

namespace gameplay
{

class Container;
class Form;

/**
 * Defines the base class for all controls.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class Control : public Ref, public AnimationTarget, public ScriptTarget
{
    friend class Form;
    friend class Container;

public:

    /**
     * The possible states a control can be in.
     */
    enum State
    {
        /**
         * State of an enabled but inactive control.
         */
        NORMAL = 0x01,

        /**
         * State of a control when it is currently in focus.
         */
        FOCUS = 0x02,

        /**
         * State of a control that is currently being acted on,
         * e.g. through touch or mouse-click events.
         */
        ACTIVE = 0x04,

        /**
         * State of a control that has been disabled.
         */
        DISABLED = 0x08,

        /**
         * When a mouse is in use, the state of a control the cursor is over.
         */
        HOVER = 0x10,
    };

    /**
     * Defines the set of alignments for positioning controls and justifying text.
     */
    enum Alignment
    {
        // Specify horizontal alignment, use default vertical alignment (ALIGN_TOP).
        ALIGN_LEFT = 0x01,
        ALIGN_HCENTER = 0x02,
        ALIGN_RIGHT = 0x04,
    
        // Specify vertical alignment, use default horizontal alignment (ALIGN_LEFT).
        ALIGN_TOP = 0x10,
        ALIGN_VCENTER = 0x20,
        ALIGN_BOTTOM = 0x40,

        // Specify both vertical and horizontal alignment.
        ALIGN_TOP_LEFT = ALIGN_TOP | ALIGN_LEFT,
        ALIGN_VCENTER_LEFT = ALIGN_VCENTER | ALIGN_LEFT,
        ALIGN_BOTTOM_LEFT = ALIGN_BOTTOM | ALIGN_LEFT,
        ALIGN_TOP_HCENTER = ALIGN_TOP | ALIGN_HCENTER,
        ALIGN_VCENTER_HCENTER = ALIGN_VCENTER | ALIGN_HCENTER,
        ALIGN_BOTTOM_HCENTER = ALIGN_BOTTOM | ALIGN_HCENTER,
        ALIGN_TOP_RIGHT = ALIGN_TOP | ALIGN_RIGHT,
        ALIGN_VCENTER_RIGHT = ALIGN_VCENTER | ALIGN_RIGHT,
        ALIGN_BOTTOM_RIGHT = ALIGN_BOTTOM | ALIGN_RIGHT
    };

    /**
     * Defines supported auto sizing modes for controls.
     */
    enum AutoSize
    {
        /**
         * No auto sizing is applied.
         */
        AUTO_SIZE_NONE = 0x00,

        /**
         * The control's width is set to tightly fit its contents.
         */
        AUTO_SIZE_WIDTH = 0x01,

        /**
        * The control's height is set to tightly fit its contents.
        */
        AUTO_SIZE_HEIGHT = 0x02,

        /**
         * The control's width and height are set to tightly fit its contents.
         */
        AUTO_SIZE_BOTH = (AUTO_SIZE_WIDTH | AUTO_SIZE_HEIGHT)
    };

    /**
     * Implement Control::Listener and call Control::addListener()
     * in order to listen for events on controls.
     */
    class Listener
    {
    public:

        /**
         * Defines the Listener's event types.
         */
        enum EventType
        {
            /**
             * Mouse-down or touch-press event.
             */
            PRESS           = 0x01,

            /**
             * Mouse-up or touch-release event.
             */
            RELEASE         = 0x02,

            /**
             * Event triggered after consecutive PRESS and RELEASE events take place
             * within the bounds of a control.
             */
            CLICK           = 0x04,

            /**
             * Event triggered when the value of a slider, check box, or radio button
             * changes.
             */
            VALUE_CHANGED   = 0x08,

            /**
             * Event triggered when the contents of a text box are modified.
             */
            TEXT_CHANGED    = 0x10,

            /**
             * Event triggered when a control is clicked with the middle mouse button.
             */
            MIDDLE_CLICK    = 0x20,

            /**
             * Event triggered when a control is clicked with the right mouse button.
             */
            RIGHT_CLICK     = 0x40,

            /**
             * Event triggered when a control gains focus.
             */
            FOCUS_GAINED    = 0x200,

            /**
             * Event triggered when a control loses focus.
             */
            FOCUS_LOST      = 0x400
        };

        /*
         * Destructor.
         */
        virtual ~Listener() { }

        /**
         * Method called by controls when an event is triggered.
         * 
         * @param control The control triggering the event.
         * @param evt The event triggered.
         */
        virtual void controlEvent(Control* control, EventType evt) = 0;
    };

    /**
     * @script{ignore}
     * A constant used for setting themed attributes on all control states simultaneously.
     */
    static const unsigned char STATE_ALL = NORMAL | ACTIVE | FOCUS | DISABLED | HOVER;

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
    const char* getId() const;

	/**
	 * Sets this control's ID string.
	 *
	 * @param id The new control ID.
	 */
	void setId(const char* id);

    /**
     * Get the x coordinate of this control.
     *
     * @return The x coordinate of this control.
     */
    float getX() const;

    /**
     * Sets the X coordinate for the control.
     *
     * If the value is passed as a percentage of its parent container's clip region, it is interpreted as a value
     * between 0-1, where 1 equals the full size of it's parent.
     *
     * @param x The new X coordinate.
     * @param percentage True if the value should be interpreted as a percentage (0-1), false if it is regular number.
     */
    void setX(float x, bool percentage = false);

    /**
     * Determines if the X coordinate of this control computed as a percentage of its parent container.
     *
     * @return True if the X value is computed as a percentage of its parent container.
     */
    bool isXPercentage() const;

    /**
     * Get the y coordinate of this control.
     *
     * @return The y coordinate of this control.
     */
    float getY() const;

    /**
     * Sets the Y coordinate for the control.
     *
     * If the value is passed as a percentage of its parent container's clip region, it is interpreted as a value
     * between 0-1, where 1 equals the full size of it's parent.
     *
     * @param y The new Y coordinate.
     * @param percentage True if the value should be interpreted as a percentage (0-1), false if it is regular number.
     */
    void setY(float y, bool percentage = false);

    /**
     * Determines if the Y coordinate of this control is computed as a percentage of its parent container.
     *
     * @return True if the Y value is computed as a percentage of its parent container.
     */
    bool isYPercentage() const;

    /**
     * Get the width of this control.
     *
     * @return The width of this control.
     */
    float getWidth() const;

    /**
     * Set the desired width of the control, including it's border and padding, before clipping.
     *
     * If the value is passed as a percentage of its parent container's clip region, it is interpreted as a value
     * between 0-1, where 1 equals the full size of it's parent.
     *
     * Explicitly setting the width of a control clears the AUTO_SIZE_WIDTH bit, if set.
     *
     * @param width The new width.
     * @param percentage True if the value should be interpreted as a percentage (0-1), false if it is regular number.
     */
    void setWidth(float width, bool percentage = false);

    /**
     * Determines if the width of this control is computed as a percentage of its parent container.
     *
     * @return True if the width is computed as a percentage of its parent container.
     */
    bool isWidthPercentage() const;

    /**
     * Get the height of this control.
     *
     * @return The height of this control.
     */
    float getHeight() const;

    /**
     * Set the desired height of the control, including it's border and padding, before clipping.
     *
     * If the value is passed as a percentage of its parent container's clip region, it is interpreted as a value
     * between 0-1, where 1 equals the full size of it's parent.
     *
     * Explicitly setting the height of a control clears the AUTO_SIZE_HEIGHT bit, if set.
     *
     * @param height The new height.
     * @param percentage True if the value should be interpreted as a percentage (0-1), false if it is regular number.
     */
    void setHeight(float height, bool percentage = false);

    /**
     * Determines if the height of this control is computed as a percentage of its parent container.
     *
     * @return True if the height is computed as a percentage of its parent container.
     */
    bool isHeightPercentage() const;

    /**
     * Set the position of this control relative to its parent container.
     *
     * This method sets the local position of the control, relative to its container.
     * Setting percetage values is not supported with this method, use setX
     * and setY instead.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setPosition(float x, float y);

    /**
     * Set the desired size of this control, including its border and padding, before clipping.
     *
     * This method sets the size of the control, relative to its container.
     * Setting percetage values is not supported with this method, use setWidth
     * and setHeight instead.
     *
     * Explicitly setting the size of a control clears the AutoSize bits, if set.
     *
     * @param width The new width.
     * @param height The new height.
     */
    void setSize(float width, float height);

    /**
     * Get the bounds of this control, relative to its parent container and including its
     * border and padding, before clipping.
     *
     * @return The bounds of this control.
     */
    const Rectangle& getBounds() const;

    /**
     * Set the bounds of this control, relative to its parent container and including its
     * border and padding, before clipping.
     *
     * This method sets the local bounds of the control, relative to its container.
     * Setting percetage values is not supported with this method, use setX,
     * setY, setWidth and setHeight instead.
     *
     * Explicitly setting the bounds of a control clears the AutoSize bits, if set.
     *
     * @param bounds The new bounds to set.
     */
    void setBounds(const Rectangle& bounds);

    /**
     * Get the absolute bounds of this control, in pixels, including border and padding,
     * before clipping.
     *
     * The absolute bounds of a control represents its final computed bounds after all 
     * alignment, auto sizing, relative position and sizing has been computed. The
     * returned bounds is in absolute coordinates, relative to the control's top-most
     * parent container (usually its form).
     *
     * @return The absolute bounds of this control.
     */
    const Rectangle& getAbsoluteBounds() const;

    /**
     * Get the bounds of this control, relative to its parent container, after clipping.
     *
     * @return The bounds of this control.
     */
    const Rectangle& getClipBounds() const;

    /**
     * Get the content area of this control, in screen coordinates, after clipping.
     *
     * @return The clipping area of this control.
     */
    const Rectangle& getClip() const;

    /**
     * Returns the auto sizing mode for this control.
     *
     * @return The auto size mode for this control.
     */
    AutoSize getAutoSize() const;

    /**
     * Sets the auto size mode for this control.
     *
     * @param mode The new auto size mode for this control.
     */
    void setAutoSize(AutoSize mode);

    /**
     * Set the alignment of this control within its parent container.
     *
     * @param alignment This control's alignment.
     */
    void setAlignment(Alignment alignment);

    /**
     * Get the alignment of this control within its parent container.
     *
     * @return The alignment of this control within its parent container.
     */
    Alignment getAlignment() const;

    /**
     * Set the size of this control's border.
     *
     * @param top The height of the border's top side.
     * @param bottom The height of the border's bottom side.
     * @param left The width of the border's left side.
     * @param right The width of the border's right side.
     * @param states The states to set this property on.
     *               One or more members of the Control::State enum, ORed together.
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
     * @param region The cursor region.
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
     * Sets the visibility of a control.
     *
     * This is a quick way to hide a control without having to remove it from a form.
     *
     * @param visible true if the control is visible and enabled; false if not-visible and disabled.
     */
    void setVisible(bool visible);

    /**
     * Get the visibility of a control.
     *
     * @return true if the control is visible; false if not visible.
     */
    bool isVisible() const;

    /**
     * Determines if this control is visible in its hierarchy.
     *
     * A control is visible in its hierarchy if it is visible and all of its parents
     * are also visible.
     */
    bool isVisibleInHierarchy() const;

    /**
     * Set the opacity of this control.
     *
     * @param opacity The new opacity.
     * @param states The states to set this property on. One or more members of the Control::State enum, OR'ed together.
     */
    void setOpacity(float opacity, unsigned char states = STATE_ALL);

    /**
     * Get the opacity of this control for a given state. 
     *
     * @param state The state to get this property from.
     *
     * @return The opacity of this control for a given state.
     */
    float getOpacity(State state = NORMAL) const;

	/**
	 * Enables/Disables a control. 
	 *
	 * @param enabled true if the control is enabled; false if disabled.
	 */
	virtual void setEnabled(bool enabled);

    /**
     * Get whether this control is currently enabled.
     *
     * @return Whether this control is currently enabled.
     */
    bool isEnabled() const;

    /**
     * Determines if this control is enabled in its hierarchy.
     *
     * A control is enabled in its hierarchy if it is enabled and all of its parents
     * are also enabled.
     */
    bool isEnabledInHierarchy() const;

    /**
     * Get this control's current state.
     *
     * @return This control's current state.
     */
    State getState() const;

    /**
     * Set whether this control consumes input events,
     * preventing them from being passed to the game.
     *
     * @param consume Whether this control consumes input events.
     */
    void setConsumeInputEvents(bool consume);

    /**
     * Get whether this control consumes touch events.
     *
     * @return Whether this control consumes touch events.
     */
    bool getConsumeInputEvents();

    /**
     * Get this control's style.
     *
     * @return This control's style.
     */
    Theme::Style* getStyle() const;

    /**
     * Set the style this control will use when rendering.
     *
     * @param style The style this control will use when rendering.
     */
    void setStyle(Theme::Style* style);

    /**
     * Returns the theme for this control.
     */
    Theme* getTheme() const;

    /**
     * Get this control's z-index.
     *
     * @return This control's z-index.
     */
    int getZIndex() const;

    /**
     * Set this control's z-index.
     *
     * @param zIndex The new z-index.
     */
    void setZIndex(int zIndex);

    /**
     * Determines if this control accepts focus.
     *
     * @return True if this control accepts focus, false if it does not.
     */
    bool canFocus() const;

    /**
     * Sets whether or not the control accepts input focus.
     *
     * @param acceptsFocus True if the control should accept input focus, false otherwise.
     */
    void setCanFocus(bool acceptsFocus);

    /**
     * Determines if this control is currently in focus.
     *
     * @return True if the control is currently in focus.
     */
    bool hasFocus() const;

    /**
     * Sets input focus to this control.
     *
     * If this control accepts focus (the hasFocus method returns true), input focus
     * is set to this control. If this control is a container, the first focusable
     * control within it gains focus.
     *
     * @return True if this control or one of its children successfully gained focus,
     *      false otherwise.
     */
    virtual bool setFocus();

    /**
     * Get this control's focus index.
     *
     * @return This control's focus index.
     */
    int getFocusIndex() const;

    /**
     * Set this control's focus index.
     *
     * Focus indexes control the order in which input focus changes between controls
     * when using the focus change controls such as the TAB key.
     *
     * Valid focus indexes should be zero or greater, with a negative number indicating
     * an unset focus index.
     *
     * @param focusIndex The new focus index.
     */
    void setFocusIndex(int focusIndex);

    /**
     * Returns whether this Control object is a Container or not.
     *
     * @return true if this object is of class Container, false otherwise.
     */
    virtual bool isContainer() const;

    /**
     * Gets the type of the Control and returns it as a string.
     *
     * @return The string of the Control type, all in lower-case.
     */
    virtual const char* getType() const;

    /**
     * Returns this control's parent, or NULL if this control does not have a parent.
     *
     * @return This control's parent.
     */
    Control* getParent() const;

    /**
     * Determines if this control is a child (at any level of hierarchy) of the 
     * specified control.
     *
     * @param control The control to check.
     * @return True if this control is a direct or indirect child of the specified control.
     */
    bool isChild(Control* control) const;

    /**
     * Returns this control's top level form, or NULL if this control does not belong to a form.
     *
     * @return this control's form.
     */
    Form* getTopLevelForm() const;

    /**
     * Adds a listener to be notified of specific events affecting
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
     * Removes a listener from this control.
     * 
     * @param listener The listener to remove.
     */
    virtual void removeListener(Control::Listener* listener);

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
     *  Constant value representing an unset or invalid contact index.
     */
    static const int INVALID_CONTACT_INDEX = -1;

    /**
     * Indicates that the bounds of the control are dirty.
     */
    static const int DIRTY_BOUNDS = 1;

    /**
     * Indicates that the state of the control is dirty.
     */
    static const int DIRTY_STATE = 2;

    /**
     * Constructor.
     */
    Control();

    /**
     * Destructor.
     */
    virtual ~Control();

    /**
     * Hidden copy assignment operator.
     */
    Control& operator=(const Control&);

    /**
     * Internal method for setting the X position of the control.
     *
     * This method is meant for internal use by the Control or descendent classes
     * who need to modify the position of the control during bounds computation.
     *
     * @see setX(float, bool)
     */
    void setXInternal(float x, bool percentage = false);

    /**
     * Internal method for setting the Y position of the control.
     *
     * This method is meant for internal use by the Control or descendent classes
     * who need to modify the position of the control during bounds computation.
     *
     * @see setY(float, bool)
     */
    void setYInternal(float x, bool percentage = false);

    /**
     * Internal method for setting the width of the control.
     *
     * The width of the control is set without modifying the existing AutoSize
     * rules for the control.
     *
     * This method is meant for internal use by the Control or descendent classes
     * who need to modify the size of the control during bounds computation.
     *
     * @see setWidth(float, bool)
     */
    void setWidthInternal(float width, bool percentage = false);

    /**
     * Internal method for setting the height of the control.
     *
     * The height of the control is set without modifying the existing AutoSize
     * rules for the control.
     *
     * This method is meant for internal use by the Control or descendent classes
     * who need to modify the size of the control during bounds computation.
     *
     * @see setHeight(float, bool)
     */
    void setHeightInternal(float height, bool percentage = false);

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
     * @see Mouse::MouseEvent
     */
    virtual bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Gamepad callback on gamepad events.
     *
     * @param gamepad The gamepad whose state changed.
     * @param evt The gamepad event that occurred.
     * @param analogIndex If evt is JOYSTICK_EVENT or TRIGGER_EVENT, this will be the index of the corresponding control.
     */
    virtual bool gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Called each frame to update this control and its children.
     *
     * Any logic that must be performed every frame should be implemented here,
     * such as custom animation or interpolation that depends on time.
     * Layout logic should not be implemented here, but in updateBounds instead.
     *
     * @param elapsedTime Time since last frame.
     */
    virtual void update(float elapsedTime);

    /**
     * Called when the state of the control has been updated.
     *
     * When the state of a control is updated (for example, a control becomes active, focused,
     * hovered, disabled, etc), this method is called to allow implementations to update
     * internal data related to control state.
     *
     * @param state The new control state.
     */
    virtual void updateState(State state);

    /**
     * Updates the local bounds for this control and its children.
     *
     * Child controls that need to customize their bounds calculation should override this method.
     */
    virtual void updateBounds();

    /**
     * Updates the absolute bounds for this control and its children.
     *
     * @param offset Offset to add to the control's position (most often used for scrolling).
     */
    virtual void updateAbsoluteBounds(const Vector2& offset);

    /**
     * Indicates that a control will begin drawing into the specified batch.
     *
     * When drawing is finshed (before any other batch can be drawn into), the
     * finishBatch method should be called.
     *
     * @param form The form beign drawn.
     * @param batch The sprite batch to be drawn into.
     */
    void startBatch(Form* form, SpriteBatch* batch);

    /**
     * Called after a batch has been drawn into and before any other batch is used.
     *
     * @param form The form being drawn.
     * @param batch The batch that was previously started (via Control::startBatch).
     */
    void finishBatch(Form* form, SpriteBatch* batch);

    /**
     * Draws the control.
     *
     * Implementations of Control are expected to perform all drawing into a SpriteBatch.
     * Batches should not be explicitly started or finished, but instead should be passed
     * to Control::prepare(Form*, SpriteBatch*). This will handle automatically starting
     * and finishing the batch when neccessary.
     *
     * @param form The top level form being drawn.
     * @param clip The clipping rectangle.
     *
     * @return The number of draw calls issued.
     */
    virtual unsigned int draw(Form* form, const Rectangle& clip);

    /**
     * Draws the themed border and background of a control.
     *
     * Implementations of Control are expected to perform all drawing into a SpriteBatch.
     * Batches should not be explicitly started or finished, but instead should be passed
     * to Control::prepare(Form*, SpriteBatch*). This will handle automatically starting
     * and finishing the batch when neccessary.
     *
     * @param form The top level form being drawn.
     * @param clip The clipping rectangle of this control's parent container.
     *
     * @return The number of draw calls issued.
     */
    virtual unsigned int drawBorder(Form* form, const Rectangle& clip);

    /**
     * Draw the images associated with this control.
     *
     * Implementations of Control are expected to perform all drawing into a SpriteBatch.
     * Batches should not be explicitly started or finished, but instead should be passed
     * to Control::prepare(Form*, SpriteBatch*). This will handle automatically starting
     * and finishing the batch when neccessary.
     *
     * @param form The top level form being drawn.
     * @param clip The clipping rectangle of this control's parent container.
     *
     * @return The number of draw calls issued.
     */
    virtual unsigned int drawImages(Form* form, const Rectangle& clip);

    /**
     * Draw this control's text.
     *
     * Implementations of Control are expected to perform all drawing into a SpriteBatch.
     * Batches should not be explicitly started or finished, but instead should be passed
     * to Control::prepare(Form*, SpriteBatch*). This will handle automatically starting
     * and finishing the batch when neccessary.
     *
     * @param form The top level form being drawn.
     * @param clip The clipping rectangle of this control's parent container.
     *
     * @return The number of draw calls issued.
     */
    virtual unsigned int drawText(Form* form, const Rectangle& clip);

    /**
     * Initializes the control.
     *
     * @param typeName The type name of the control being initalized.
     * @param style The style to apply to this control (optional).
     * @param properties The properties to set on this control (optional).
     */
    virtual void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * Get a Control::State enum from a matching string.
     *
     * @param state The string to match.
     *
     * @return The Control::State enum that matches the given string.
     */
    static State getState(const char* state);

    /**
     * Get a Theme::ThemeImage from its ID, for a given state.
     *
     * @param id The ID of the image to retrieve.
     * @param state The state to get this image from.
     *
     * @return The requested Theme::ThemeImage, or NULL if none was found.
     */
    Theme::ThemeImage* getImage(const char* id, State state);

    /**
     * Notify this control's listeners of a specific event.
     *
     * @param eventType The event to trigger.
     */
    void notifyListeners(Control::Listener::EventType eventType);

    /**
     * Called when a control event is fired for this control, before external
     * listeners are notified of the event.
     *
     * @param evt The event type.
     */
    virtual void controlEvent(Control::Listener::EventType evt);

    /**
     * Sets dirty bits for the control.
     *
     * Valid bits are any of the "DIRTY_xxx" constants from the Control class.
     *
     * @param bits Dirty bits to set.
     */
    void setDirty(int bits);

    /**
     * Determines if the specified bit is dirty.
     *
     * @param bit The bit to check.
     */
    bool isDirty(int bit) const;

    /**
     * Gets the Alignment by string.
     *
     * @param alignment The string representation of the Alignment type.
     * @return The Alignment enum value corresponding to the given string.
     */
    static Alignment getAlignment(const char* alignment);

    /** 
     * The Control's ID.
     */ 
    std::string _id;

    /**
     * Bits indicating whether bounds values are absolute values or percentages.
     */
    int _boundsBits;

    /**
     * Local bounds, relative to parent container's clipping window, possibly stored as percentages (see _boundsBits).
     */
    Rectangle _relativeBounds;

    /**
     * Local bounds, relative to parent container's clipping window, and desired size.
     */
    Rectangle _bounds;

    /**
     * Local bounds, relative to parent container's clipping window, including border and padding, after clipping.
     */
    Rectangle _clipBounds;

    /**
     * Absolute bounds, including border and padding, before clipping.
     */
    Rectangle _absoluteBounds;

    /**
     * Absolute bounds, including border and padding, after clipping.
     */
    Rectangle _absoluteClipBounds;

    /**
     * Absolute bounds of content area (i.e. without border and padding), before clipping.
     */
    Rectangle _viewportBounds;

    /**
     * Absolute bounds of content area (i.e. without border and padding), after clipping.
     */
    Rectangle _viewportClipBounds;

    /**
     * Control dirty bits.
     */
    int _dirtyBits;

    /**
     * Flag for whether the Control consumes input events.
     */
    bool _consumeInputEvents;
    
    /**
     * The Control's Alignment
     */
    Alignment _alignment;
    
    /**
     * The Control's auto size mode.
     */
    AutoSize _autoSize;
    
    /**
     * Listeners map of EventType's to a list of Listeners.
     */
    std::map<Control::Listener::EventType, std::list<Control::Listener*>*>* _listeners;
    
    /**
     * The Control's Theme::Style.
     */
    Theme::Style* _style;

    /**
     * The control is not visible and _state become DISABLED if false.
     */
    bool _visible;

    /**
     * The current opacity of the control.
     */
    float _opacity;
    
    /**
     * The z-order of the control.
     */
    int _zIndex;

    /**
     * The contact index assigned to this control.
     */
    int _contactIndex;

    /**
     * The focus order of the control.
     */
    int _focusIndex;

    /**
     * Whether or not the control accepts input focus.
     */
    bool _canFocus;

    /**
     * This control's current state.
     */
    State _state;

    /**
     * The control's parent container.
     */
    Container* _parent;

private:

    /*
     * Constructor.
     */    
    Control(const Control& copy);

    bool updateBoundsInternal(const Vector2& offset);

    AutoSize parseAutoSize(const char* str);

    Theme::Style::Overlay** getOverlays(unsigned char overlayTypes, Theme::Style::Overlay** overlays);

    Theme::Style::Overlay* getOverlay(Control::State state) const;

    void overrideStyle();

    void overrideThemedProperties(Properties* properties, unsigned char states);

    void setImageList(Theme::ImageList* imageList, unsigned char states = STATE_ALL);

    void setCursor(Theme::ThemeImage* cursor, unsigned char states = STATE_ALL);

    void setSkin(Theme::Skin* skin, unsigned char states = STATE_ALL);

    Theme::Skin* getSkin(State state);

    void addSpecificListener(Control::Listener* listener, Control::Listener::EventType eventType);

    bool _styleOverridden;
    Theme::Skin* _skin;

};

}

#endif
