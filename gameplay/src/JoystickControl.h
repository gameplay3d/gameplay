#ifndef JOYSTICKCONTROL_H_
#define JOYSTICKCONTROL_H_

#include "Control.h"

namespace gameplay
{

/**
 * Defines a control representing a joystick (axis).
 *
 * This is used in virtual Gamepad instances.
 *
 * @see http://gameplay3d.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class JoystickControl : public Control
{
    friend class Container;
    friend class Gamepad;
	friend class ControlFactory;

public:

    /**
     * Creates a new Joystick control.
     *
     * @param id The joystick ID.
     * @param style The joystick style.
     *
     * @return The new joystick.
     * @script{create}
     */
    static JoystickControl* create(const char* id, Theme::Style* style = NULL);

    /**
     * Extends ScriptTarget::getTypeName() to return the type name of this class.
     *
     * Child controls should override this function to return the correct type name.
     *
     * @return The type name of this class: "JoystickControl"
     * @see ScriptTarget::getTypeName()
     */
    const char* getTypeName() const;

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
    void addListener(Control::Listener* listener, int eventFlags);

    /**
     * Gets the value (2-dimensional direction) of the joystick.
     * 
     * @return The value of the joystick.
     */
    const Vector2& getValue() const;

    /**
     * Sets the image size of the inner region of the joystick. Does not do anything if there is no
     * inner image region defined.
     * 
     * @param size The size of the inner region of the joystick. (x, y) == (width, height)
     * @param isWidthPercentage If the width value should be computed as a percentage of the relative size of this control
     * @param isHeightPercentage If the height value should be computed as a percentage of the relative size of this control
     */
    void setInnerRegionSize(const Vector2& size, bool isWidthPercentage = false, bool isHeightPercentage = false);

    /**
     * Gets the image size of the inner region of the joystick. Returns (0,0) if there is no inner image
     * region defined.
     * 
     * @param isWidthPercentage Set to true if the width value is a percentage value of the relative size of this control
     * @param isHeightPercentage Set to true if the height value is a percentage value of the relative size of this control
     *
     * @return The image size of the inner region of the joystick. (x, y) == (width, height)
     */
    const Vector2& getInnerRegionSize(bool* isWidthPercentage = NULL, bool* isHeightPercentage = NULL) const;

    /**
     * Sets the image size of the outer region of the joystick. Does not do anything if there is no
     * outer image region defined.
     * 
     * @param size The size of the outer region of the joystick. (x, y) == (width, height)
     * @param isWidthPercentage If the width value should be computed as a percentage of the relative size of this control
     * @param isHeightPercentage If the height value should be computed as a percentage of the relative size of this control
     */
    void setOuterRegionSize(const Vector2& size, bool isWidthPercentage = false, bool isHeightPercentage = false);

    /**
     * Gets the image size of the outer region of the joystick. Returns (0,0) if there is no outer image
     * region defined.
     *
     * @param isWidthPercentage Set to true if the width value is a percentage value of the relative size of this control
     * @param isHeightPercentage Set to true if the height value is a percentage value of the relative size of this control
     * 
     * @return The image size of the outer region of the joystick. (x, y) == (width, height)
     */
    const Vector2& getOuterRegionSize(bool* isWidthPercentage = NULL, bool* isHeightPercentage = NULL) const;

    /**
     * Sets whether relative positioning is enabled or not.
     * 
     * Note: The default behavior is absolute positioning, and not relative.
     *
     * @param relative Whether relative positioning should be enabled or not.
     */
    void setRelative(bool relative);

    /**
     * Gets whether absolute positioning is enabled or not.
     * 
     * Note: The default behavior is absolute positioning, and not relative.
     *
     * @return <code>true</code> if relative positioning is enabled; <code>false</code> otherwise.
     */
    bool isRelative() const;

    /**
     * Gets the index of this joystick across all joysticks on a form.
     *
     * @return The index of this joystick on a form.
     */
    unsigned int getIndex() const;

    /**
     * Sets the radius of joystick motion
     *
     * @param radius The radius to be set.
     * @param isPercentage If the radius value is a percentage value of the relative size of this control
     */
    void setRadius(float radius, bool isPercentage = false);

    /**
     * Gets the radius of joystick motion
     *
     * @return The radius of joystick motion
     */
    float getRadius() const;

    /**
      * Determines if the radius of joystick motion is a percentage value of the relative size of this control
      *
     * @return True if the radius of joystick motion is a percentage value of the relative size of this control
     */
    bool isRadiusPercentage() const;

protected:
    
    /**
     * Constructor.
     */
    JoystickControl();

    /**
     * Destructor.
     */
    virtual ~JoystickControl();

    /**
     * Create a joystick control with a given style and properties.
     *
     * @param style The style to apply to this joystick.
     * @param properties A properties object containing a definition of the joystick.
	 *
     * @return The new joystick.
     */
    static Control* create(Theme::Style* style, Properties* properties = NULL);

    /**
     * @see Control::initialize
     */
    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * Touch callback on touch events.  Controls return true if they consume the touch event.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by the control.
     *
     * @see Touch::TouchEvent
     */
    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * @see Control::updateAbsoluteBounds
     */
    void updateAbsoluteBounds(const Vector2& offset);

    /**
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

private:

    JoystickControl(const JoystickControl& copy);

    void setRegion(const Vector2& regionSizeIn, Vector2& regionSizeOut, int& regionBoundsBitsOut, bool isWidthPercentage, bool isHeightPercentage);

    void getRegion(Vector2& regionOut, int& regionBoundsBitsOut, const char* regionPropertyId);

    Vector2 getPixelSize(const Vector2& region, const int regionBoundsBits) const;

    Vector2 getPixelSize(const Theme::ThemeImage* image) const;

    Theme::ThemeImage * getNonEmptyImage(const char* id, Control::State state);

    void updateAbsoluteSizes();

    void setBoundsBit(bool set, int& bitSetOut, int bit);

    float _radiusCoord;
    Vector2* _innerRegionCoord;
    Vector2* _outerRegionCoord;
    int _innerRegionCoordBoundsBits;
    int _outerRegionCoordBoundsBits;
    float _radiusPixels;
    Vector2* _innerSizePixels;
    Vector2* _outerSizePixels;
    Rectangle _screenRegionPixels;
    bool _relative;
    Vector2 _value;
    Vector2 _displacement;
    unsigned int _index;
};

}

#endif
