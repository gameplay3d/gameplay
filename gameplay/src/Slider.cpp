#include "Slider.h"
#include "Game.h"

namespace gameplay
{

// Fraction of slider to scroll when mouse scrollwheel is used.
static const float SCROLLWHEEL_FRACTION = 0.1f;
// Fraction of slider to scroll for a delta of 1.0f when a gamepad or keyboard is used.
static const float MOVE_FRACTION = 0.005f;
// Distance that a slider must be moved before it starts consuming input events,
// e.g. to prevent its parent container from scrolling at the same time.
static const float SLIDER_THRESHOLD = 5.0f;

Slider::Slider() : _min(0.0f), _max(0.0f), _step(0.0f), _value(0.0f), _delta(0.0f), _minImage(NULL),
    _maxImage(NULL), _trackImage(NULL), _markerImage(NULL), _valueTextVisible(false),
    _valueTextAlignment(Font::ALIGN_BOTTOM_HCENTER), _valueTextPrecision(0), _valueText(""),
    _selectButtonDown(false), _directionButtonDown(false), _gamepadValue(0.0f)
{
    _canFocus = true;
}

Slider::~Slider()
{
}

Slider* Slider::create(const char* id, Theme::Style* style)
{
    GP_ASSERT(style);

    Slider* slider = new Slider();
    if (id)
        slider->_id = id;
    slider->setStyle(style);

    return slider;
}

Control* Slider::create(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);

    Slider* slider = new Slider();
    slider->initialize(style, properties);

    slider->_min = properties->getFloat("min");
    slider->_max = properties->getFloat("max");
    slider->_value = properties->getFloat("value");
    slider->_step = properties->getFloat("step");
    slider->_valueTextVisible = properties->getBool("valueTextVisible");
    slider->_valueTextPrecision = properties->getInt("valueTextPrecision");

    if (properties->exists("valueTextAlignment"))
    {
        slider->_valueTextAlignment = Font::getJustify(properties->getString("valueTextAlignment"));
    }

    return slider;
}

void Slider::setMin(float min)
{
    if (_min != _min)
    {
        _dirty = true;
    }
    _min = min;
}

float Slider::getMin() const
{
    return _min;
}

void Slider::setMax(float max)
{
    if (max != _max)
    {
        _dirty = true;
    }
    _max = max;
}

float Slider::getMax() const
{
    return _max;
}

void Slider::setStep(float step)
{
    _step = step;
}

float Slider::getStep() const
{
    return _step;
}

float Slider::getValue() const
{
    return _value;
}

void Slider::setValue(float value)
{
    float oldValue = _value;
    _value = MATH_CLAMP(value, _min, _max);
    if (_value != value)
    {
        _dirty = true;
    }
}

void Slider::setValueTextVisible(bool valueTextVisible)
{
    if (valueTextVisible != _valueTextVisible)
    {
        _dirty = true;
    }
    _valueTextVisible = valueTextVisible;
}

bool Slider::isValueTextVisible() const
{
    return _valueTextVisible;
}

void Slider::setValueTextAlignment(Font::Justify alignment)
{
    if (alignment != _valueTextAlignment)
    {
        _dirty = true;
    }
    _valueTextAlignment = alignment;
}

Font::Justify Slider::getValueTextAlignment() const
{
    return _valueTextAlignment;
}

void Slider::setValueTextPrecision(unsigned int precision)
{
    if (precision != _valueTextPrecision)
    {
        _dirty = true;
    }
    _valueTextPrecision = precision;
}

unsigned int Slider::getValueTextPrecision() const
{
    return _valueTextPrecision;
}

void Slider::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to Slider.");
    }

    Control::addListener(listener, eventFlags);
}

void Slider::updateValue(int x, int y)
{
    State state = getState();

    // If the point lies within this slider, update the value of the slider accordingly
    if (x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
        y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
    {
        // Horizontal case.
        const Theme::Border& border = getBorder(state);
        const Theme::Padding& padding = getPadding();
        const Rectangle& minCapRegion = _minImage->getRegion();
        const Rectangle& maxCapRegion = _maxImage->getRegion();

        float markerPosition = ((float)x - maxCapRegion.width - border.left - padding.left) /
            (_bounds.width - border.left - border.right - padding.left - padding.right - minCapRegion.width - maxCapRegion.width);
            
        if (markerPosition > 1.0f)
        {
            markerPosition = 1.0f;
        }
        else if (markerPosition < 0.0f)
        {
            markerPosition = 0.0f;
        }

        float oldValue = _value;
        _value = (markerPosition * (_max - _min)) + _min;
        if (_step > 0.0f)
        {            
            int numSteps = round(_value / _step);
            _value = _step * numSteps;
        }

        // Call the callback if our value changed.
        if (_value != oldValue)
        {
            notifyListeners(Control::Listener::VALUE_CHANGED);
        }
        _dirty = true;
    }
}

bool Slider::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    State state = getState();

    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        updateValue(x, y);
        return true;

    case Touch::TOUCH_MOVE:
        if (state == ACTIVE)
        {
            updateValue(x, y);
            return true;
        }
        break;
    }

    return Control::touchEvent(evt, x, y, contactIndex);
}

bool Slider::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
        case Mouse::MOUSE_WHEEL:
        {
            if (hasFocus())
            {
                float total = _max - _min;
                float oldValue = _value;
                _value += (total * SCROLLWHEEL_FRACTION) * wheelDelta;
            
                if (_value > _max)
                    _value = _max;
                else if (_value < _min)
                    _value = _min;

                if (_step > 0.0f)
                {            
                    int numSteps = round(_value / _step);
                    _value = _step * numSteps;
                }

                if (_value != oldValue)
                {
                    notifyListeners(Control::Listener::VALUE_CHANGED);
                }

                _dirty = true;
                return true;
            }
            break;
        }

        default:
            break;
    }

    // Return false to fall through to touch handling
    return false;
}

bool Slider::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    bool eventConsumed = false;

    switch (evt)
    {
        case Gamepad::JOYSTICK_EVENT:
        {
            // The right analog stick can be used to change a slider's value.
            if (analogIndex == 1)
            {
                Vector2 joy;
                gamepad->getJoystickValues(analogIndex, &joy);
                _gamepadValue = _value;
                _delta = joy.x;
                _dirty = true;
                return true;
            }
            break;
        }
    }

    return Label::gamepadEvent(evt, gamepad, analogIndex);
}

bool Slider::keyEvent(Keyboard::KeyEvent evt, int key)
{
    switch (evt)
    {
    case Keyboard::KEY_PRESS:
        switch (key)
        {
        case Keyboard::KEY_LEFT_ARROW:
            if (_step > 0.0f)
            {
                _value = std::max(_value - _step, _min);
            }
            else
            {
                _value = std::max(_value - (_max - _min) * MOVE_FRACTION, _min);
            }
            _dirty = true;
            return true;

        case Keyboard::KEY_RIGHT_ARROW:
            if (_step > 0.0f)
            {
                _value = std::min(_value + _step, _max);
            }
            else
            {
                _value = std::min(_value + (_max - _min) * MOVE_FRACTION, _max);
            }
            _dirty = true;
            return true;
        }
        break;
    }

    return Control::keyEvent(evt, key);
}

void Slider::update(const Control* container, const Vector2& offset)
{
    Label::update(container, offset);

    Control::State state = getState();

    _minImage = getImage("minCap", state);
    _maxImage = getImage("maxCap", state);
    _markerImage = getImage("marker", state);
    _trackImage = getImage("track", state);

    char s[32];
    sprintf(s, "%.*f", _valueTextPrecision, _value);
    _valueText = s;

    if (_delta != 0.0f)
    {
        float oldValue = _value;
        float total = _max - _min;

        if (_step > 0.0f)
        {
            _gamepadValue += (total * MOVE_FRACTION) * _delta;
            int numSteps = round(_gamepadValue / _step);
            _value = _step * numSteps;
        }
        else
        {
            _value += (total * MOVE_FRACTION) * _delta;
        }

        if (_value > _max)
            _value = _max;
        else if (_value < _min)
            _value = _min;

        if (_value != oldValue)
        {
            notifyListeners(Control::Listener::VALUE_CHANGED);
        }
    }

    if (_autoHeight == Control::AUTO_SIZE_FIT)
    {
        float height = _minImage->getRegion().height;
        height = std::max(height, _maxImage->getRegion().height);
        height = std::max(height, _markerImage->getRegion().height);
        height = std::max(height, _trackImage->getRegion().height);
        height += _bounds.height;
        if (_valueTextVisible && _font)
            height += getFontSize(state);
        setHeight(height);
    }
}

void Slider::draw(SpriteBatch* spriteBatch, const Rectangle& clip, bool needsClear, bool cleared, float targetHeight)
{
    if (needsClear)
    {
        GL_ASSERT( glEnable(GL_SCISSOR_TEST) );
        GL_ASSERT( glScissor(_clearBounds.x, targetHeight - _clearBounds.y - _clearBounds.height, _clearBounds.width, _clearBounds.height) );
        Game::getInstance()->clear(Game::CLEAR_COLOR, Vector4::zero(), 1.0f, 0);
        GL_ASSERT( glDisable(GL_SCISSOR_TEST) );
    }

    if (!_visible)
    {
        _dirty = false;
        return;
    }

    spriteBatch->start();
    drawBorder(spriteBatch, clip);
    drawImages(spriteBatch, clip);
    spriteBatch->finish();

    drawText(clip);
    if (_delta == 0.0f)
    {
        _dirty = false;
    }
}

void Slider::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    GP_ASSERT(spriteBatch);
    GP_ASSERT(_minImage);
    GP_ASSERT(_maxImage);
    GP_ASSERT(_markerImage);
    GP_ASSERT(_trackImage);

    // TODO: Vertical slider.

    // The slider is drawn in the center of the control (perpendicular to orientation).
    // The track is stretched according to orientation.
    // Caps and marker are not stretched.
    const Rectangle& minCapRegion = _minImage->getRegion();
    const Rectangle& maxCapRegion = _maxImage->getRegion();
    const Rectangle& markerRegion = _markerImage->getRegion();
    const Rectangle& trackRegion = _trackImage->getRegion();

    const Theme::UVs& minCap = _minImage->getUVs();
    const Theme::UVs& maxCap = _maxImage->getUVs();
    const Theme::UVs& marker = _markerImage->getUVs();
    const Theme::UVs& track = _trackImage->getUVs();

    Vector4 minCapColor = _minImage->getColor();
    Vector4 maxCapColor = _maxImage->getColor();
    Vector4 markerColor = _markerImage->getColor();
    Vector4 trackColor = _trackImage->getColor();

    minCapColor.w *= _opacity;
    maxCapColor.w *= _opacity;
    markerColor.w *= _opacity;
    trackColor.w *= _opacity;

    // Draw order: track, caps, marker.
    float midY = _viewportBounds.y + (_viewportBounds.height) * 0.5f;
    Vector2 pos(_viewportBounds.x, midY - trackRegion.height * 0.5f);
    spriteBatch->draw(pos.x, pos.y, _viewportBounds.width, trackRegion.height, track.u1, track.v1, track.u2, track.v2, trackColor, _viewportClipBounds);

    pos.y = midY - minCapRegion.height * 0.5f;
    pos.x -= minCapRegion.width * 0.5f;
    spriteBatch->draw(pos.x, pos.y, minCapRegion.width, minCapRegion.height, minCap.u1, minCap.v1, minCap.u2, minCap.v2, minCapColor, _viewportClipBounds);

    pos.x = _viewportBounds.x + _viewportBounds.width - maxCapRegion.width * 0.5f;
    spriteBatch->draw(pos.x, pos.y, maxCapRegion.width, maxCapRegion.height, maxCap.u1, maxCap.v1, maxCap.u2, maxCap.v2, maxCapColor, _viewportClipBounds);

    // Percent across.
    float markerPosition = (_value - _min) / (_max - _min);
    markerPosition *= _viewportBounds.width - minCapRegion.width * 0.5f - maxCapRegion.width * 0.5f - markerRegion.width;
    pos.x = _viewportBounds.x + minCapRegion.width * 0.5f + markerPosition;
    pos.y = midY - markerRegion.height / 2.0f;
    spriteBatch->draw(pos.x, pos.y, markerRegion.width, markerRegion.height, marker.u1, marker.v1, marker.u2, marker.v2, markerColor, _viewportClipBounds);
}

void Slider::drawText(const Rectangle& clip)
{
    Label::drawText(clip);

    if (_valueTextVisible && _font)
    {
        Control::State state = getState();
        _font->start();
        _font->drawText(_valueText.c_str(), _textBounds, _textColor, getFontSize(state), _valueTextAlignment, true, getTextRightToLeft(state), &_viewportClipBounds);
        _font->finish();
    }
}

const char* Slider::getType() const
{
    return "slider";
}

}
