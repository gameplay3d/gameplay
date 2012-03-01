#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "Control.h"
#include "Layout.h"

namespace gameplay
{

class Container : public Control
{
public:
    /**
     * A Container's layout type must be specified at creation time.
     */
    static Container* create(Layout::Type type);
    static Container* create(Theme::Style* style, Properties* properties, Theme* theme);
    static Container* getContainer(const char* id);

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
     * Insert a Control at a specific index.
     *
     * @param control The Control to add.
     * @param index The index at which to insert the Control.
     */
    void insertControl(Control* control, unsigned int index);

    void removeControl(unsigned int index);
    void removeControl(const char* id);
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

    std::vector<Control*> getControls() const;

    /**
     * Updates the position of each Control within this Container
     * according to the Container's Layout.
     */
    virtual void update(const Vector2& position);

    //void draw(Theme* theme, const Vector2& position);
    virtual void drawSprites(SpriteBatch* spriteBatch, const Vector2& position);
    virtual void drawText(const Vector2& position);

    virtual bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    virtual void keyEvent(Keyboard::KeyEvent evt, int key);

protected:
    Container();
    Container(const Container& copy);
    virtual ~Container();

    static Layout::Type getLayoutType(const char* layoutString);

    bool isDirty();
    void addControls(Theme* theme, Properties* properties);

    Layout* _layout;
    std::vector<Control*> _controls;
};

}

#endif
