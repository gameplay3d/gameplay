#ifndef CONTROL_H_
#define CONTROL_H_

#include "Ref.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "SpriteBatch.h"
#include "Theme.h"
#include "Touch.h"

namespace gameplay
{

class Control : public Ref
{
public:

    enum State
    {
        STATE_NORMAL,
        STATE_FOCUS,
        STATE_ACTIVE
    };

    const char* getID();

    /**
     * Get the actual bounding box of this Control, local to its Container,
     * after any calculations performed due to the Container's Layout or settings of auto-size.
     * Always includes the Control's border.
     * Can optionally include the Control's padding.
     * Query getPosition() and getSize() to learn the bounds without border or padding.
     */
    const Rectangle& getBounds(bool includePadding) const;

    /**
     * Position of this Control relative to its parent Container.
     */
    void setPosition(float x, float y);
    const Vector2& getPosition() const;

    void setSize(float width, float height);
    const Vector2& getSize() const;

    /**
     * Set width and/or height to auto-size to size a Control to tightly fit
     * its text and themed visual elements (CheckBox / RadioButton toggle etc.).
     *
     * Similarly set this on the width and/or height of a Container to tightly fit
     * the Container around all its children.
     */
    void setAutoSize(bool width, bool height);

    /**
     * 
     * When auto-size is set on width and/or height:
     * Space added to the calculated (tightly bound) width and height.
     */
    void setBorder(float horizontal, float vertical);
    const Vector2& getBorder() const;

    /**
     * In a layout of any type other than Absolute,
     * guarantee this much empty space surrounding this Control.
     */
    void setPadding(float horizontal, float vertical);
    const Vector2& getPadding() const;

    void setState(State state);
    State getState();

    Theme::Style::OverlayType getOverlayType();

    /**
     * Defaults to empty stub.
     */
    virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Draws the themed border and background of a control.
     */
    void drawBorder(SpriteBatch* spriteBatch, const Vector2& position);
    virtual void drawSprites(SpriteBatch* spriteBatch, const Vector2& position);
    virtual void drawText(const Vector2& position);

    /**
     * Returns whether this Control has been modified since the last time
     * isDirty() was called, and resets its dirty flag.
     */
    virtual bool isDirty();

    void setStyle(Theme::Style* Style);
    Theme::Style* getStyle() const;

    void themeChanged();

protected:
    Control();
    Control(const Control& copy);
    virtual ~Control();

    std::string _id;
    State _state;           // Determines overlay used during draw().
    Vector2 _size;
    Vector2 _position;
    Vector2 _border;
    Vector2 _padding;
    bool _autoWidth;
    bool _autoHeight;
    bool _dirty;
    Theme::Style* _style;
};

}

#endif