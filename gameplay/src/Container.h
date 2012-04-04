#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "Control.h"
#include "Layout.h"

namespace gameplay
{

/**
 * A container is a UI control that can contain other controls.
 *
 * The following properties are available for containers:
 *
 * container <Container ID>
 * {
 *      // Container properties.
 *      layout   = <Layout Type>        // A value from the Layout::Type enum.  E.g.: LAYOUT_VERTICAL
 *      style    = <Style ID>           // A style from the form's theme.
 *      position = <x, y>               // Position of the container on-screen, measured in pixels.
 *      size     = <width, height>      // Size of the container, measured in pixels.
 *   
 *      // All the controls within this container.
 *      container{}
 *      label{}
 *      textBox{}
 *      button{}
 *      checkBox{}
 *      radioButton{}
 *      slider{}
 * }
 */
class Container : public Control
{
public:
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
     * @param ID The ID of the control to remove.
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
     */
    std::vector<Control*> getControls() const;

    /**
     * Gets the first animation in the control with the specified ID.
     *
     * @param id The ID of the animation to get. Returns the first animation if ID is NULL.
     * @return The first animation with the specified ID.
     */
    Animation* getAnimation(const char* id = NULL) const;

protected:
    Container();
    virtual ~Container();

    /**
     * Create an empty container.  A container's layout type must be specified at creation time.
     *
     * @param type The container's layout type.
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
     * @param clip The clipping rectangle of this container's parent container.
     */
    virtual void update(const Rectangle& clip);

    /**
     * Draws the themed border and background of this container and all its controls.
     *
     * @param spriteBatch The sprite batch containing this container's border images.
     * @param clip The clipping rectangle of this container's parent container.
     */
    void drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draws the icons of all controls within this container.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this container's parent container.
     */
    virtual void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draws the text of all controls within this container.
     *
     * @param clip The clipping rectangle of this container's parent container.
     */
    virtual void drawText(const Rectangle& clip);

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
     * @param evt The key event that occured.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Gets a Layout::Type enum from a matching string.
     */
    static Layout::Type getLayoutType(const char* layoutString);

    /**
     * Returns whether this control is a container.
     * This is true in this case.
     */
    bool isContainer();

    /**
     * Returns whether this container or any of its controls have been modified and require an update.
     */
    virtual bool isDirty();

    /**
     * Adds controls nested within a properties object to this container,
     * searching for styles within the given theme.
     */
    void addControls(Theme* theme, Properties* properties);

    Layout* _layout;                    // This container's layout.
    std::vector<Control*> _controls;    // List of controls within this container.

private:
    Container(const Container& copy);
};

}

#endif
