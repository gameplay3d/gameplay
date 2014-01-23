#include "Slider.h"
#include "Game.h"

namespace gameplay
{

// Fraction of slider to scroll when mouse scrollwheel is used.
static const float SCROLLWHEEL_FRACTION = 0.1f;
// Fraction of slider to scroll for a delta of 1.0f when a gamepad or keyboard is used.
static const float MOVE_FRACTION = 0.005f;

Slider::Slider() : _min(0.0f), _max(0.0f), _step(0.0f), _value(0.0f), _delta(0.0f), _minImage(NULL),
    _maxImage(NULL), _trackImage(NULL), _markerImage(NULL), _valueTextVisible(false),
    _valueTextAlignment(Font::ALIGN_BOTTOM_HCENTER), _valueTextPrecision(0), _valueText(""), 
    _trackHeight(0.0f), _gamepadValue(0.0f)
{
    _canFocus = true;
}

Slider::~Slider()
{
}

Slider* Slider::create(const char* id, Theme::Style* style)
{
    Slider* slider = new Slider();
    slider->_id = id ? id : "";
    slider->initialize("Slider", style, NULL);
    return slider;
}

Control* Slider::create(Theme::Style* style, Properties* properties)
{
    Slider* slider = new Slider();
    slider->initialize("Slider", style, properties);
    return slider;
}

void Slider::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Label::initialize(typeName, style, properties);

    if (properties)
    {
        _min = properties->getFloat("min");
        _max = properties->getFloat("max");
        _value = properties->getFloat("value");
        _step = properties->getFloat("step");
        _valueTextVisible = properties->getBool("valueTextVisible");
        _valueTextPrecision = properties->getInt("valueTextPrecision");

        if (properties->exists("valueTextAlignment"))
        {
            _valueTextAlignment = Font::getJustify(properties->getString("valueTextAlignment"));
        }
    }

    // Force value text to be updated
    setValue(_value);
}

void Slider::setMin(float min)
{
    _min = min;
}

float Slider::getMin() const
{
    return _min;
}

void Slider::setMax(float max)
{
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
    value = MATH_CLAMP(value, _min, _max);

    if (value != _value)
    {
        _value = value;
        notifyListeners(Control::Listener::VALUE_CHANGED);
    }

    // Always update value text if it's visible
    if (_valueTextVisible)
    {
        char s[32];
        sprintf(s, "%.*f", _valueTextPrecision, _value);
        _valueText = s;
    }
}

void Slider::setValueTextVisible(bool valueTextVisible)
{
    if (valueTextVisible != _valueTextVisible)
    {
        _valueTextVisible = valueTextVisible;
        if (_autoSize & AUTO_SIZE_HEIGHT)
            setDirty(DIRTY_BOUNDS);
    }
}

bool Slider::isValueTextVisible() const
{
    return _valueTextVisible;
}

void Slider::setValueTextAlignment(Font::Justify alignment)
{
    _valueTextAlignment = alignment;
}

Font::Justify Slider::getValueTextAlignment() const
{
    return _valueTextAlignment;
}

void Slider::setValueTextPrecision(unsigned int precision)
{
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

    // Horizontal case.
    const Theme::Border& border = getBorder(state);
    const Theme::Padding& padding = getPadding();
    const Rectangle& minCapRegion = _minImage->getRegion();
    const Rectangle& maxCapRegion = _maxImage->getRegion();
    const Rectangle& markerRegion = _markerImage->getRegion();

    float markerPosition = x / (_viewportBounds.width - markerRegion.width);
            
    if (markerPosition > 1.0f)
    {
        markerPosition = 1.0f;
    }
    else if (markerPosition < 0.0f)
    {
        markerPosition = 0.0f;
    }

    float value = (markerPosition * (_max - _min)) + _min;
    if (_step > 0.0f)
    {            
        int numSteps = round(value / _step);
        value = _step * numSteps;
    }

    setValue(value);
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

static bool isScrollable(Container* container)
{
    if (container->getScroll() != Container::SCROLL_NONE)
        return true;

    Container* parent = static_cast<Container*>(container->getParent());
    return parent ? isScrollable(parent) : false;
}

bool Slider::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
        case Mouse::MOUSE_WHEEL:
        {
            if (hasFocus() && !isScrollable(_parent))
            {
                float total = _max - _min;
                float value = _value + (total * SCROLLWHEEL_FRACTION) * wheelDelta;

                if (_step > 0.0f)
                {            
                    int numSteps = round(value / _step);
                    value = _step * numSteps;
                }

                setValue(value);
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
                setValue(std::max(_value - _step, _min));
            }
            else
            {
                setValue(std::max(_value - (_max - _min) * MOVE_FRACTION, _min));
            }
            return true;

        case Keyboard::KEY_RIGHT_ARROW:
            if (_step > 0.0f)
            {
                setValue(std::min(_value + _step, _max));
            }
            else
            {
                setValue(std::min(_value + (_max - _min) * MOVE_FRACTION, _max));
            }
            return true;
        }
        break;
    }

    return Control::keyEvent(evt, key);
}

void Slider::update(float elapsedTime)
{
    Label::update(elapsedTime);

    if (_delta != 0.0f)
    {
        float total = _max - _min;

        if (_step > 0.0f)
        {
            _gamepadValue += (total * MOVE_FRACTION) * _delta;
            int numSteps = round(_gamepadValue / _step);
            setValue(_step * numSteps);
        }
        else
        {
            setValue(_value + (total * MOVE_FRACTION) * _delta);
        }
    }
}

void Slider::updateState(State state)
{
    Label::updateState(state);

    _minImage = getImage("minCap", state);
    _maxImage = getImage("maxCap", state);
    _markerImage = getImage("marker", state);
    _trackImage = getImage("track", state);
}

void Slider::updateBounds()
{
    Label::updateBounds();

    // Compute height of track (max of track, min/max and marker
    _trackHeight = _minImage->getRegion().height;
    _trackHeight = std::max(_trackHeight, _maxImage->getRegion().height);
    _trackHeight = std::max(_trackHeight, _markerImage->getRegion().height);
    _trackHeight = std::max(_trackHeight, _trackImage->getRegion().height);

    if (_autoSize & AUTO_SIZE_HEIGHT)
    {
        float height = _bounds.height + _trackHeight;
        if (_valueTextVisible)
            height += getFontSize(NORMAL);
        setHeightInternal(height);
    }
}

unsigned int Slider::drawImages(Form* form, const Rectangle& clip)
{
    if (!(_minImage && _maxImage && _markerImage && _trackImage))
        return 0;

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

    Control::State state = getState();

    minCapColor.w *= _opacity;
    maxCapColor.w *= _opacity;
    markerColor.w *= _opacity;
    trackColor.w *= _opacity;

    SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
    startBatch(form, batch);

    // Compute area to draw the slider track
    unsigned int fontSize = getFontSize(state);
    float startY, endY;
    if (_text.length() > 0)
    {
        if (_valueTextVisible)
        {
            // Both label and value text are visible.
            // Draw slider in the middle.
            startY = fontSize;
            endY = _viewportBounds.height - fontSize;
        }
        else
        {
            // Only label is visible
            if (getTextAlignment(state) & ALIGN_BOTTOM)
            {
                // Draw slider above label
                startY = 0;
                endY = _viewportBounds.height - fontSize;
            }
            else
            {
                // Draw slider below label
                startY = fontSize;
                endY = _viewportBounds.height;
            }
        }
    }
    else if (_valueTextVisible)
    {
        // Only value text is visible.
        if (_valueTextAlignment & ALIGN_BOTTOM)
        {
            // Draw slider above value text
            startY = 0;
            endY = _viewportBounds.height - fontSize;
        }
        else
        {
            // Draw slider below value text
            startY = fontSize;
            endY = _viewportBounds.height;
        }
    }
    else
    {
        // Only the slider track is visible
        startY = 0;
        endY = _viewportBounds.height;
    }

    // Compute midpoint of track location
    float midY = _viewportBounds.y + startY + (endY - startY) * 0.5f;

    // Draw track below the slider text
    Vector2 pos(_viewportBounds.x + minCapRegion.width, midY - trackRegion.height * 0.5f);
    batch->draw(pos.x, pos.y, _viewportBounds.width - minCapRegion.width - maxCapRegion.width, trackRegion.height, track.u1, track.v1, track.u2, track.v2, trackColor, _viewportClipBounds);

    // Draw min cap to the left of the track
    pos.y = midY - minCapRegion.height * 0.5f;
    pos.x = _viewportBounds.x;
    batch->draw(pos.x, pos.y, minCapRegion.width, minCapRegion.height, minCap.u1, minCap.v1, minCap.u2, minCap.v2, minCapColor, _viewportClipBounds);

    // Draw max cap to the right of the track
    pos.x = _viewportBounds.right() - maxCapRegion.width;
    batch->draw(pos.x, pos.y, maxCapRegion.width, maxCapRegion.height, maxCap.u1, maxCap.v1, maxCap.u2, maxCap.v2, maxCapColor, _viewportClipBounds);

    // Draw the marker at the correct position
    float markerPosition = (_value - _min) / (_max - _min);
    markerPosition *= _viewportBounds.width - markerRegion.width;
    pos.x = _viewportBounds.x + markerPosition;
    pos.y = midY - markerRegion.height * 0.5f;
    batch->draw(pos.x, pos.y, markerRegion.width, markerRegion.height, marker.u1, marker.v1, marker.u2, marker.v2, markerColor, _viewportClipBounds);

    finishBatch(form, batch);

    return 4;
}

unsigned int Slider::drawText(Form* form, const Rectangle& clip)
{
    unsigned int drawCalls = Label::drawText(form, clip);

    if (_valueTextVisible && _font)
    {
        Control::State state = getState();
        unsigned int fontSize = getFontSize(state);

        SpriteBatch* batch = _font->getSpriteBatch(fontSize);
        startBatch(form, batch);
        _font->drawText(_valueText.c_str(), _textBounds, _textColor, fontSize, _valueTextAlignment, true, getTextRightToLeft(state), &_viewportClipBounds);
        finishBatch(form, batch);

        ++drawCalls;
    }

    return drawCalls;
}

const char* Slider::getType() const
{
    return "slider";
}

}
