#ifndef SLIDER_H_
#define SLIDER_H_

#include "Base.h"
#include "Theme.h"
#include "Properties.h"
#include "Button.h"
#include "Touch.h"

namespace gameplay
{

class Slider : public Button
{
public:
    static Slider* create(Theme::Style* style, Properties* properties);
    static Slider* create(const char* id, float min, float max, float defaultPosition = 0.0f, float step = 1.0f);
    static Slider* getSlider(const char* id);

    void setMin(float min);
    float getMin();

    void setMax(float max);
    float getMax();

    void setStep(float step);
    float getStep();

    void setValue(float value);
    float getValue();

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void drawSprites(SpriteBatch* spriteBatch, const Vector2& position);

protected:
    Slider();
    ~Slider();

private:
    float _min;
    float _max;
    float _step;
    float _value;
};

}

#endif
