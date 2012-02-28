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
    static CheckBox* create(Theme::Style* style, Properties* properties);
    static CheckBox* getCheckBox(const char* id);

    bool isChecked();

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void update(const Vector2& position);

    void drawSprites(SpriteBatch* spriteBatch, const Vector2& position);
    void drawText(const Vector2& position);

protected:
    CheckBox();
    CheckBox(const CheckBox& copy);
    ~CheckBox();

    bool _checked;
    Vector2 _iconSize;
};

}

#endif