#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

#include "Button.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

class RadioButton : public Button
{
public:
    RadioButton();
    virtual ~RadioButton();

    static RadioButton* create(Theme::Style* style, Properties* properties);
    static RadioButton* getRadioButton(const char* id);

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void update(const Vector2& position);

    void drawSprites(SpriteBatch* spriteBatch, const Vector2& position);
    void drawText(const Vector2& position);

private:
    RadioButton(const RadioButton& copy);

    // Clear the _selected flag of all RadioButtons in the given group.
    static void clearSelected(const std::string& groupId);

    std::string _groupId;
    bool _selected;
    Vector2 _iconSize;
};

}

#endif