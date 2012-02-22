#ifndef CONTROL_H_
#define CONTROL_H_

#include "Ref.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "SpriteBatch.h"
#include "Theme.h"
#include "Touch.h"
#include "Keyboard.h"

namespace gameplay
{

class Control : public Ref
{
public:

    enum State
    {
        STATE_NORMAL,
        STATE_FOCUS,
        STATE_ACTIVE,
        STATE_DISABLED
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
     * Set the position of this Control relative to its parent Container.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setPosition(float x, float y);

    /**
     * Get the position of this Control relative to its parent Container.
     *
     * @return The position vector.
     */
    const Vector2& getPosition() const;

    /**
     * Set the size of this Control, including its border and padding.
     *
     * @param width The width.
     * @param height The height.
     */
    void setSize(float width, float height);

    /**
     * Get the size of this Control, including its border and padding.
     *
     * @return The size vector.
     */
    const Vector2& getSize() const;

    /**
     * Set width and/or height to auto-size to size a Control to tightly fit
     * its text and themed visual elements (CheckBox / RadioButton toggle etc.).
     *
     * Similarly set this on the width and/or height of a Container to tightly fit
     * the Container around all its children.
     *
     * @param width Whether to automatically determine this Control's width.
     * @param height Whether to automatically determine this Control's height.
     */
    void setAutoSize(bool width, bool height);

    void setState(State state);
    State getState();

    void disable();
    void enable();

    Theme::Style::OverlayType getOverlayType() const;

    /**
     * Defaults to empty stub.
     */
    virtual void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    virtual void keyEvent(Keyboard::KeyEvent evt, int key);

    virtual void update(const Vector2& position);

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