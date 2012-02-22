#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include "Theme.h"
#include "Properties.h"
#include "Touch.h"
#include "Button.h"

namespace gameplay
{

class CheckBox : public Button
{
public:
    CheckBox();
    ~CheckBox();

    static CheckBox* create(Theme::Style* style, Properties* properties);
    static CheckBox* getCheckBox(const char* id);

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void drawSprites(SpriteBatch* spriteBatch, const Vector2& position);
    void drawText(const Vector2& position);

private:
    CheckBox(const CheckBox& copy);

    bool _checked;
};

}

#endif