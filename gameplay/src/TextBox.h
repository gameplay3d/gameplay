#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "Base.h"
#include "Label.h"
#include "Theme.h"
#include "Keyboard.h"

namespace gameplay
{

class TextBox : public Label
{
public:
    static TextBox* create(Theme::Style* style, Properties* properties);
    static TextBox* getTextBox(const char* id);

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

    void update(const Rectangle& clip);

    // Draw the cursor.
    void drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip);

private:
    TextBox();
    TextBox(const TextBox& copy);
    ~TextBox();

    void setCursorLocation(int x, int y);

    Vector2 _cursorLocation;
    unsigned int textIndex;
};

}

#endif