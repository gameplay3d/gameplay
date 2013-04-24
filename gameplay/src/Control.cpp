#include "Base.h"
#include "Game.h"
#include "Control.h"

namespace gameplay
{

Control::Control()
    : _id(""), _state(Control::NORMAL), _bounds(Rectangle::empty()), _clipBounds(Rectangle::empty()), _viewportClipBounds(Rectangle::empty()),
    _clearBounds(Rectangle::empty()), _dirty(true), _consumeInputEvents(false), _alignment(ALIGN_TOP_LEFT), _isAlignmentSet(false), _autoWidth(false), _autoHeight(false), _listeners(NULL), _visible(true),
    _zIndex(-1), _contactIndex(INVALID_CONTACT_INDEX), _focusIndex(-1), _parent(NULL), _styleOverridden(false), _skin(NULL), _previousState(NORMAL)
{
    addScriptEvent("controlEvent", "<Control>[Control::Listener::EventType]");
}

Control::~Control()
{
    if (_listeners)
    {
        for (std::map<Control::Listener::EventType, std::list<Control::Listener*>*>::const_iterator itr = _listeners->begin(); itr != _listeners->end(); ++itr)
        {
            std::list<Control::Listener*>* list = itr->second;
            SAFE_DELETE(list);
        }
        SAFE_DELETE(_listeners);
    }

    if (_styleOverridden)
    {
        SAFE_DELETE(_style);
    }
}

void Control::initialize(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);
    _style = style;

    // Properties not defined by the style.
    _alignment = getAlignment(properties->getString("alignment"));
    _autoWidth = properties->getBool("autoWidth");
    _autoHeight = properties->getBool("autoHeight");

    _consumeInputEvents = properties->getBool("consumeInputEvents", false);

    _visible = properties->getBool("visible", true);

    if (properties->exists("zIndex"))
    {
        _zIndex = properties->getInt("zIndex");
    }
    else
    {
        _zIndex = -1;
    }

    if (properties->exists("focusIndex"))
    {
        _focusIndex = properties->getInt("focusIndex");
    }
    else
    {
        _focusIndex = -1;
    }

    Vector2 position;
    Vector2 size;
    if (properties->exists("position"))
    {
        properties->getVector2("position", &position);
    }
    else
    {
        position.x = properties->getFloat("x");
        position.y = properties->getFloat("y");
    }
        
    if (properties->exists("size"))
    {
        properties->getVector2("size", &size);
    }
    else
    {
        size.x = properties->getFloat("width");
        size.y = properties->getFloat("height");
    }
    setBounds(Rectangle(position.x, position.y, size.x, size.y));

    const char* id = properties->getId();
    if (id)
        _id = id;

    // Potentially override themed properties for all states.
    overrideThemedProperties(properties, STATE_ALL);

    // Override themed properties on specific states.
    Properties* innerSpace = properties->getNextNamespace();
    while (innerSpace != NULL)
    {
        std::string spaceName(innerSpace->getNamespace());
        std::transform(spaceName.begin(), spaceName.end(), spaceName.begin(), (int(*)(int))toupper);
        if (spaceName == "STATENORMAL")
        {
            overrideThemedProperties(innerSpace, NORMAL);
        }
        else if (spaceName == "STATEFOCUS")
        {
            overrideThemedProperties(innerSpace, FOCUS);
        }
        else if (spaceName == "STATEACTIVE")
        {
            overrideThemedProperties(innerSpace, ACTIVE);
        }
        else if (spaceName == "STATEDISABLED")
        {
            overrideThemedProperties(innerSpace, DISABLED);
        }
        else if (spaceName == "STATEHOVER")
        {
            overrideThemedProperties(innerSpace, HOVER);
        }
        else if (spaceName == "MARGIN")
        {
            setMargin(innerSpace->getFloat("top"), innerSpace->getFloat("bottom"),
                innerSpace->getFloat("left"), innerSpace->getFloat("right"));
        }
        else if (spaceName == "PADDING")
        {
            setPadding(innerSpace->getFloat("top"), innerSpace->getFloat("bottom"),
                innerSpace->getFloat("left"), innerSpace->getFloat("right"));
        }

        innerSpace = properties->getNextNamespace();
    }
}

const char* Control::getId() const
{
    return _id.c_str();
}

void Control::setPosition(float x, float y)
{
    if (x != _bounds.x || y != _bounds.y)
    {
        _bounds.x = x;
        _bounds.y = y;
        _dirty = true;
    }
}

void Control::setSize(float width, float height)
{
    if (width != _bounds.width || height != _bounds.height)
    {
        _bounds.width = width;
        _bounds.height = height;
        _dirty = true;
    }
}

void Control::setWidth(float width)
{
    if (width != _bounds.width)
    {
        _bounds.width = width;
        _dirty = true;
    }
}

void Control::setHeight(float height)
{
    if (height != _bounds.height)
    {
        _bounds.height = height;
        _dirty = true;
    }
}

void Control::setBounds(const Rectangle& bounds)
{
    if (bounds != _bounds)
    {
        _bounds.set(bounds);
        _dirty = true;
    }
}

const Rectangle& Control::getBounds() const
{
    return _bounds;
}

const Rectangle& Control::getAbsoluteBounds() const
{
    return _absoluteBounds;
}

float Control::getX() const
{
    return _bounds.x;
}

float Control::getY() const
{
    return _bounds.y;
}

float Control::getWidth() const
{
    return _bounds.width;
}

float Control::getHeight() const
{
    return _bounds.height;
}

void Control::setAlignment(Alignment alignment)
{
    _alignment = alignment;
    _isAlignmentSet = true;
    _dirty = true;
}

Control::Alignment Control::getAlignment() const
{
    return _alignment;
}

void Control::setAutoWidth(bool autoWidth)
{
    if (_autoWidth != autoWidth)
    {
        _autoWidth = autoWidth;
        _dirty = true;
    }
}

bool Control::getAutoWidth() const
{
    return _autoWidth;
}

void Control::setAutoHeight(bool autoHeight)
{
    if (_autoHeight != autoHeight)
    {
        _autoHeight = autoHeight;
        _dirty = true;
    }
}

bool Control::getAutoHeight() const
{
    return _autoHeight;
}

void Control::setVisible(bool visible)
{
    if (visible && !_visible)
    {
        _visible = true;
        _dirty = true;
    }
    else if (!visible && _visible)
    {
        _visible = false;
        _dirty = true;
    }
}

bool Control::isVisible() const
{
    return _visible;
}

bool Control::hasFocus() const
{
    return (_state == FOCUS || (_state == HOVER && _previousState == FOCUS));
}

void Control::setOpacity(float opacity, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setOpacity(opacity);
    }
    _dirty = true;
}

float Control::getOpacity(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getOpacity();
}

void Control::setEnabled(bool enabled)
{
	if (enabled && _state == Control::DISABLED)
	{
		_state = Control::NORMAL;
        _dirty = true;
	}
	else if (!enabled && _state != Control::DISABLED)
	{
		_state = Control::DISABLED;
		_dirty = true;
	}
}

bool Control::isEnabled() const
{
    return _state != DISABLED;
}

void Control::setBorder(float top, float bottom, float left, float right, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setBorder(top, bottom, left, right);
    }

    _dirty = true;
}

const Theme::Border& Control::getBorder(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getBorder();
}

void Control::setSkinRegion(const Rectangle& region, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setSkinRegion(region, _style->_tw, _style->_th);
    }
    _dirty = true;
}

const Rectangle& Control::getSkinRegion(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getSkinRegion();
}

void Control::setSkinColor(const Vector4& color, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setSkinColor(color);
    }

    _dirty = true;
}

const Vector4& Control::getSkinColor(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getSkinColor();
}

void Control::setMargin(float top, float bottom, float left, float right)
{
    GP_ASSERT(_style);
    overrideStyle();
    _style->setMargin(top, bottom, left, right);
    _dirty = true;
}

const Theme::Margin& Control::getMargin() const
{
    GP_ASSERT(_style);
    return _style->getMargin();
}

void Control::setPadding(float top, float bottom, float left, float right)
{
    GP_ASSERT(_style);
    overrideStyle();
    _style->setPadding(top, bottom, left, right);
    _dirty = true;
}
    
const Theme::Padding& Control::getPadding() const
{
    GP_ASSERT(_style);
    return _style->getPadding();
}

void Control::setImageRegion(const char* id, const Rectangle& region, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setImageRegion(id, region, _style->_tw, _style->_th);
    }

    _dirty = true;
}

const Rectangle& Control::getImageRegion(const char* id, State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getImageRegion(id);
}

void Control::setImageColor(const char* id, const Vector4& color, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setImageColor(id, color);
    }

    _dirty = true;
}

const Vector4& Control::getImageColor(const char* id, State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getImageColor(id);
}

const Theme::UVs& Control::getImageUVs(const char* id, State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getImageUVs(id);
}

void Control::setCursorRegion(const Rectangle& region, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setCursorRegion(region, _style->_tw, _style->_th);
    }

    _dirty = true;
}

const Rectangle& Control::getCursorRegion(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getCursorRegion();
}

void Control::setCursorColor(const Vector4& color, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setCursorColor(color);
    }

    _dirty = true;
}

const Vector4& Control::getCursorColor(State state)
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getCursorColor();
}
    
const Theme::UVs& Control::getCursorUVs(State state)
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getCursorUVs();
}

void Control::setFont(Font* font, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setFont(font);
    }

    _dirty = true;
}

Font* Control::getFont(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getFont();
}

void Control::setFontSize(unsigned int fontSize, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setFontSize(fontSize);
    }

    _dirty = true;
}

unsigned int Control::getFontSize(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getFontSize();
}

void Control::setTextColor(const Vector4& color, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setTextColor(color);
    }

    _dirty = true;
}

const Vector4& Control::getTextColor(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getTextColor();
}

void Control::setTextAlignment(Font::Justify alignment, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setTextAlignment(alignment);
    }

    _dirty = true;
}

Font::Justify Control::getTextAlignment(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getTextAlignment();
}

void Control::setTextRightToLeft(bool rightToLeft, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setTextRightToLeft(rightToLeft);
    }

    _dirty = true;
}

bool Control::getTextRightToLeft(State state) const
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getTextRightToLeft();
}

const Rectangle& Control::getClipBounds() const
{
    return _clipBounds;
}

const Rectangle& Control::getClip() const
{
    return _viewportClipBounds;
}

void Control::setStyle(Theme::Style* style)
{
    if (style != _style)
    {
        _dirty = true;
    }

    _style = style;
}

Theme::Style* Control::getStyle() const
{
    return _style;
}

void Control::setState(State state)
{
    if (getOverlay(_state) != getOverlay(state))
        _dirty = true;

    _state = state;
}

Control::State Control::getState() const
{
    return _state;
}

Theme::Style::OverlayType Control::getOverlayType() const
{
    switch (_state)
    {
    case Control::NORMAL:
        return Theme::Style::OVERLAY_NORMAL;
    case Control::FOCUS:
        return Theme::Style::OVERLAY_FOCUS;
    case Control::ACTIVE:
        return Theme::Style::OVERLAY_ACTIVE;
    case Control::DISABLED:
        return Theme::Style::OVERLAY_DISABLED;
    case Control::HOVER:
        return Theme::Style::OVERLAY_HOVER;
    default:
        return Theme::Style::OVERLAY_NORMAL;
    }
}

void Control::setConsumeInputEvents(bool consume)
{
    _consumeInputEvents = consume;
}
    
bool Control::getConsumeInputEvents()
{
    return _consumeInputEvents;
}

int Control::getZIndex() const
{
    return _zIndex;
}

void Control::setZIndex(int zIndex)
{
    if (zIndex != _zIndex)
    {
        _zIndex = zIndex;
        _dirty = true;
    }
}

int Control::getFocusIndex() const
{
    return _focusIndex;
}

void Control::setFocusIndex(int focusIndex)
{
    _focusIndex = focusIndex;
}

void Control::addListener(Control::Listener* listener, int eventFlags)
{
    GP_ASSERT(listener);

    if ((eventFlags & Control::Listener::PRESS) == Control::Listener::PRESS)
    {
        addSpecificListener(listener, Control::Listener::PRESS);
    }

    if ((eventFlags & Control::Listener::RELEASE) == Control::Listener::RELEASE)
    {
        addSpecificListener(listener, Control::Listener::RELEASE);
    }

    if ((eventFlags & Control::Listener::CLICK) == Control::Listener::CLICK)
    {
        addSpecificListener(listener, Control::Listener::CLICK);
    }

    if ((eventFlags & Control::Listener::VALUE_CHANGED) == Control::Listener::VALUE_CHANGED)
    {
        addSpecificListener(listener, Control::Listener::VALUE_CHANGED);
    }

    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        addSpecificListener(listener, Control::Listener::TEXT_CHANGED);
    }
}

void Control::removeListener(Control::Listener* listener)
{
    if (_listeners == NULL || listener == NULL)
        return;

    for (std::map<Control::Listener::EventType, std::list<Control::Listener*>*>::iterator itr = _listeners->begin(); itr != _listeners->end();)
    {
        itr->second->remove(listener);

        if(itr->second->empty())
        {
            std::list<Control::Listener*>* list = itr->second;
            _listeners->erase(itr++);
            SAFE_DELETE(list);
        }
        else
            ++itr;
    }

    if (_listeners->empty())
        SAFE_DELETE(_listeners);
}

void Control::addSpecificListener(Control::Listener* listener, Control::Listener::EventType eventType)
{
    GP_ASSERT(listener);

    if (!_listeners)
    {
        _listeners = new std::map<Control::Listener::EventType, std::list<Control::Listener*>*>();
    }

    std::map<Control::Listener::EventType, std::list<Control::Listener*>*>::const_iterator itr = _listeners->find(eventType);
    if (itr == _listeners->end())
    {
        _listeners->insert(std::make_pair(eventType, new std::list<Control::Listener*>()));
        itr = _listeners->find(eventType);
    }

    std::list<Control::Listener*>* listenerList = itr->second;
    listenerList->push_back(listener);
}

bool Control::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        // Controls that don't have an ACTIVE state go to the FOCUS state when pressed.
        // (Other controls, such as buttons and sliders, become ACTIVE when pressed and go to the FOCUS state on release.)
        // Labels are never any state other than NORMAL.
        if (_contactIndex == INVALID_CONTACT_INDEX && x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
            y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
        {
            _contactIndex = (int) contactIndex;

            notifyListeners(Control::Listener::PRESS);

            return _consumeInputEvents;
        }
        else
        {
            // If this control was in focus, it's not any more.
            _state = NORMAL;
        }
        break;
            
    case Touch::TOUCH_MOVE:
        break;

    case Touch::TOUCH_RELEASE:
        if (_contactIndex == (int)contactIndex)
        {
            _contactIndex = INVALID_CONTACT_INDEX;

            // Always trigger Control::Listener::RELEASE
            notifyListeners(Control::Listener::RELEASE);

            // Only trigger Control::Listener::CLICK if both PRESS and RELEASE took place within the control's bounds.
            if (x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                // Leave this control in the FOCUS state.
                notifyListeners(Control::Listener::CLICK);
            }

            return _consumeInputEvents;
        }
        break;
    }

    return false;
}

bool Control::keyEvent(Keyboard::KeyEvent evt, int key)
{
    return false;
}

bool Control::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    // By default, mouse events are either interpreted as touch events or ignored.
    switch (evt)
    {
    case Mouse::MOUSE_PRESS_LEFT_BUTTON:
        return touchEvent(Touch::TOUCH_PRESS, x, y, 0);

    case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
        return touchEvent(Touch::TOUCH_RELEASE, x, y, 0);

    case Mouse::MOUSE_MOVE:
        if (_state != ACTIVE)
        {
            if (_state != HOVER &&
                x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                _previousState = _state;
                setState(HOVER);
                notifyListeners(Control::Listener::ENTER);
                return _consumeInputEvents;
            }
            else if (_state == HOVER && !(x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                        y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height))
            {
                setState(_previousState);
                notifyListeners(Control::Listener::LEAVE);
                return _consumeInputEvents;
            }
        }
        return touchEvent(Touch::TOUCH_MOVE, x, y, 0);

    default:
        break;
    }

    return false;
}

bool Control::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    // Default behavior for gamepad events.
    switch (evt)
    {
    case Gamepad::BUTTON_EVENT:
        if (_state == Control::FOCUS)
        {
            if (gamepad->isButtonDown(Gamepad::BUTTON_A) ||
                gamepad->isButtonDown(Gamepad::BUTTON_X))
            {
                notifyListeners(Control::Listener::PRESS);
                return _consumeInputEvents;
            }
        }
        else if (_state == Control::ACTIVE)
        {
            if (!gamepad->isButtonDown(Gamepad::BUTTON_A) &&
                !gamepad->isButtonDown(Gamepad::BUTTON_X))
            {
                notifyListeners(Control::Listener::RELEASE);
                notifyListeners(Control::Listener::CLICK);
                return _consumeInputEvents;
            }
        }
        break;
    case Gamepad::JOYSTICK_EVENT:
        break;
    case Gamepad::TRIGGER_EVENT:
        break;
    }

    return false;
}

void Control::notifyListeners(Control::Listener::EventType eventType)
{
    // This method runs untrusted code by notifying listeners of events.
    // If the user calls exit() or otherwise releases this control, we
    // need to keep it alive until the method returns.
    addRef();

    if (_listeners)
    {
        std::map<Control::Listener::EventType, std::list<Control::Listener*>*>::const_iterator itr = _listeners->find(eventType);
        if (itr != _listeners->end())
        {
            std::list<Control::Listener*>* listenerList = itr->second;
            for (std::list<Control::Listener*>::iterator listenerItr = listenerList->begin(); listenerItr != listenerList->end(); ++listenerItr)
            {
                GP_ASSERT(*listenerItr);
                (*listenerItr)->controlEvent(this, eventType);
            }
        }
    }

    fireScriptEvent<void>("controlEvent", this, eventType);

    release();
}

void Control::update(const Control* container, const Vector2& offset)
{
    const Rectangle& clip = container->getClip();
    const Rectangle& absoluteViewport = container->_viewportBounds;

    _clearBounds.set(_absoluteClipBounds);

    // Calculate the clipped bounds.
    float x = _bounds.x + offset.x;
    float y = _bounds.y + offset.y;
    float width = _bounds.width;
    float height = _bounds.height;

    float clipX2 = clip.x + clip.width;
    float x2 = clip.x + x + width;
    if (x2 > clipX2)
        width -= x2 - clipX2;

    float clipY2 = clip.y + clip.height;
    float y2 = clip.y + y + height;
    if (y2 > clipY2)
        height -= y2 - clipY2;

    if (x < 0)
    {
        width += x;
        x = -x;
    }
    else
    {
        x = 0;
    }

    if (y < 0)
    {
        height += y;
        y = -y;
    }
    else
    {
        y = 0;
    }

    _clipBounds.set(x, y, width, height);

    // Calculate the absolute bounds.
    x = _bounds.x + offset.x + absoluteViewport.x;
    y = _bounds.y + offset.y + absoluteViewport.y;
    _absoluteBounds.set(x, y, _bounds.width, _bounds.height);

    // Calculate the absolute viewport bounds.
    // Absolute bounds minus border and padding.
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding& padding = getPadding();

    x += border.left + padding.left;
    y += border.top + padding.top;
    width = _bounds.width - border.left - padding.left - border.right - padding.right;
    height = _bounds.height - border.top - padding.top - border.bottom - padding.bottom;

    _viewportBounds.set(x, y, width, height);

    // Calculate the clip area.
    // Absolute bounds, minus border and padding,
    // clipped to the parent container's clip area.
    clipX2 = clip.x + clip.width;
    x2 = x + width;
    if (x2 > clipX2)
        width = clipX2 - x;

    clipY2 = clip.y + clip.height;
    y2 = y + height;
    if (y2 > clipY2)
        height = clipY2 - y;

    if (x < clip.x)
    {
        float dx = clip.x - x;
        width -= dx;
        x = clip.x;
    }

    if (y < clip.y)
    {
        float dy = clip.y - y;
        height -= dy;
        y = clip.y;
    }
 
    _viewportClipBounds.set(x, y, width, height);

    width += border.left + padding.left + border.right + padding.right;
    height += border.top + padding.top + border.bottom + padding.bottom;
    _absoluteClipBounds.set(x - border.left - padding.left, y - border.top - padding.top, max(width, 0.0f), max(height, 0.0f));
    if (_clearBounds.isEmpty())
    {
        _clearBounds.set(_absoluteClipBounds);
    }

    // Cache themed attributes for performance.
    _skin = getSkin(_state);

    // Current opacity should be multiplied by that of the parent container.
    _opacity = getOpacity(_state) * container->_opacity;
}

void Control::drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    if (!spriteBatch || !_skin || _bounds.width <= 0 || _bounds.height <= 0)
        return;

    // Get the border and background images for this control's current state.
    const Theme::UVs& topLeft = _skin->getUVs(Theme::Skin::TOP_LEFT);
    const Theme::UVs& top = _skin->getUVs(Theme::Skin::TOP);
    const Theme::UVs& topRight = _skin->getUVs(Theme::Skin::TOP_RIGHT);
    const Theme::UVs& left = _skin->getUVs(Theme::Skin::LEFT);
    const Theme::UVs& center = _skin->getUVs(Theme::Skin::CENTER);
    const Theme::UVs& right = _skin->getUVs(Theme::Skin::RIGHT);
    const Theme::UVs& bottomLeft = _skin->getUVs(Theme::Skin::BOTTOM_LEFT);
    const Theme::UVs& bottom = _skin->getUVs(Theme::Skin::BOTTOM);
    const Theme::UVs& bottomRight = _skin->getUVs(Theme::Skin::BOTTOM_RIGHT);

    // Calculate screen-space positions.
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding& padding = getPadding();
    Vector4 skinColor = _skin->getColor();
    skinColor.w *= _opacity;

    float midWidth = _bounds.width - border.left - border.right;
    float midHeight = _bounds.height - border.top - border.bottom;
    float midX = _absoluteBounds.x + border.left;
    float midY = _absoluteBounds.y + border.top;
    float rightX = _absoluteBounds.x + _bounds.width - border.right;
    float bottomY = _absoluteBounds.y + _bounds.height - border.bottom;

    // Draw themed border sprites.
    if (!border.left && !border.right && !border.top && !border.bottom)
    {
        // No border, just draw the image.
        spriteBatch->draw(_absoluteBounds.x, _absoluteBounds.y, _bounds.width, _bounds.height, center.u1, center.v1, center.u2, center.v2, skinColor, clip);
    }
    else
    {
        if (border.left && border.top)
            spriteBatch->draw(_absoluteBounds.x, _absoluteBounds.y, border.left, border.top, topLeft.u1, topLeft.v1, topLeft.u2, topLeft.v2, skinColor, clip);
        if (border.top)
            spriteBatch->draw(_absoluteBounds.x + border.left, _absoluteBounds.y, midWidth, border.top, top.u1, top.v1, top.u2, top.v2, skinColor, clip);
        if (border.right && border.top)
            spriteBatch->draw(rightX, _absoluteBounds.y, border.right, border.top, topRight.u1, topRight.v1, topRight.u2, topRight.v2, skinColor, clip);
        if (border.left)
            spriteBatch->draw(_absoluteBounds.x, midY, border.left, midHeight, left.u1, left.v1, left.u2, left.v2, skinColor, clip);

        // Always draw the background.
        spriteBatch->draw(_absoluteBounds.x + border.left, _absoluteBounds.y + border.top, _bounds.width - border.left - border.right, _bounds.height - border.top - border.bottom,
            center.u1, center.v1, center.u2, center.v2, skinColor, clip);

        if (border.right)
            spriteBatch->draw(rightX, midY, border.right, midHeight, right.u1, right.v1, right.u2, right.v2, skinColor, clip);
        if (border.bottom && border.left)
            spriteBatch->draw(_absoluteBounds.x, bottomY, border.left, border.bottom, bottomLeft.u1, bottomLeft.v1, bottomLeft.u2, bottomLeft.v2, skinColor, clip);
        if (border.bottom)
            spriteBatch->draw(midX, bottomY, midWidth, border.bottom, bottom.u1, bottom.v1, bottom.u2, bottom.v2, skinColor, clip);
        if (border.bottom && border.right)
            spriteBatch->draw(rightX, bottomY, border.right, border.bottom, bottomRight.u1, bottomRight.v1, bottomRight.u2, bottomRight.v2, skinColor, clip);
    }
}

void Control::drawImages(SpriteBatch* spriteBatch, const Rectangle& position)
{
}

void Control::drawText(const Rectangle& position)
{
}

void Control::draw(SpriteBatch* spriteBatch, const Rectangle& clip, bool needsClear, bool cleared, float targetHeight)
{
    if (needsClear)
    {
        GL_ASSERT( glEnable(GL_SCISSOR_TEST) );
        GL_ASSERT( glScissor(_clearBounds.x, targetHeight - _clearBounds.y - _clearBounds.height, _clearBounds.width, _clearBounds.height) );
        Game::getInstance()->clear(Game::CLEAR_COLOR, Vector4::zero(), 1.0f, 0);
        GL_ASSERT( glDisable(GL_SCISSOR_TEST) );
    }

    if (!_visible)
        return;

    spriteBatch->start();
    drawBorder(spriteBatch, clip);
    drawImages(spriteBatch, clip);
    spriteBatch->finish();

    drawText(clip);
    _dirty = false;
}

bool Control::isDirty()
{
    return _dirty;
}

bool Control::isContainer() const
{
    return false;
}

Control::State Control::getState(const char* state)
{
    if (!state)
    {
        return NORMAL;
    }

    if (strcmp(state, "NORMAL") == 0)
    {
        return NORMAL;
    }
    else if (strcmp(state, "ACTIVE") == 0)
    {
        return ACTIVE;
    }
    else if (strcmp(state, "FOCUS") == 0)
    {
        return FOCUS;
    }
    else if (strcmp(state, "DISABLED") == 0)
    {
        return DISABLED;
    }
    else if (strcmp(state, "HOVER") == 0)
    {
        return HOVER;
    }

    return NORMAL;
}

Theme::ThemeImage* Control::getImage(const char* id, State state)
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    
    Theme::ImageList* imageList = overlay->getImageList();
    if (!imageList)
        return NULL;

    return imageList->getImage(id);
}

const char* Control::getType() const
{
    return "control";
}

// Implementation of AnimationHandler
unsigned int Control::getAnimationPropertyComponentCount(int propertyId) const
{
    switch(propertyId)
    {
    case ANIMATE_POSITION:
    case ANIMATE_SIZE:
        return 2;

    case ANIMATE_POSITION_X:
    case ANIMATE_POSITION_Y:
    case ANIMATE_SIZE_WIDTH:
    case ANIMATE_SIZE_HEIGHT:
    case ANIMATE_OPACITY:
        return 1;

    default:
        return -1;
    }
}

void Control::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    GP_ASSERT(value);

    switch(propertyId)
    {
    case ANIMATE_POSITION:
        value->setFloat(0, _bounds.x);
        value->setFloat(1, _bounds.y);
        break;
    case ANIMATE_SIZE:
        value->setFloat(0, _bounds.width);
        value->setFloat(1, _bounds.height);
        break;
    case ANIMATE_POSITION_X:
        value->setFloat(0, _bounds.x);
        break;
    case ANIMATE_POSITION_Y:
        value->setFloat(0, _bounds.y);
        break;
    case ANIMATE_SIZE_WIDTH:
        value->setFloat(0, _bounds.width);
        break;
    case ANIMATE_SIZE_HEIGHT:
        value->setFloat(0, _bounds.height);
        break;
    case ANIMATE_OPACITY:
        value->setFloat(0, _opacity);
        break;
    default:
        break;
    }
}

void Control::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    GP_ASSERT(value);

    switch(propertyId)
    {
    case ANIMATE_POSITION:
        _bounds.x = Curve::lerp(blendWeight, _bounds.x, value->getFloat(0));
        _bounds.y = Curve::lerp(blendWeight, _bounds.y, value->getFloat(1));
        _dirty = true;
        break;
    case ANIMATE_POSITION_X:
        _bounds.x = Curve::lerp(blendWeight, _bounds.x, value->getFloat(0));
        _dirty = true;
        break;
    case ANIMATE_POSITION_Y:
        _bounds.y = Curve::lerp(blendWeight, _bounds.y, value->getFloat(0));
        _dirty = true;
        break;
    case ANIMATE_SIZE:
        _bounds.width = Curve::lerp(blendWeight, _bounds.width, value->getFloat(0));
        _bounds.height = Curve::lerp(blendWeight, _bounds.height, value->getFloat(1));
        _dirty = true;
        break;
    case ANIMATE_SIZE_WIDTH:
        _bounds.width = Curve::lerp(blendWeight, _bounds.width, value->getFloat(0));
        _dirty = true;
        break;
    case ANIMATE_SIZE_HEIGHT:
        _bounds.height = Curve::lerp(blendWeight, _bounds.height, value->getFloat(0));
        _dirty = true;
        break;
    case ANIMATE_OPACITY:
        setOpacity(Curve::lerp(blendWeight, _opacity, value->getFloat(0)));
        _dirty = true;
        break;
    }
}
    

Theme::Style::Overlay** Control::getOverlays(unsigned char overlayTypes, Theme::Style::Overlay** overlays)
{
    GP_ASSERT(overlays);
    GP_ASSERT(_style);

    unsigned int index = 0;
    if ((overlayTypes & NORMAL) == NORMAL)
    {
        overlays[index++] = _style->getOverlay(Theme::Style::OVERLAY_NORMAL);
    }

    if ((overlayTypes & FOCUS) == FOCUS)
    {
        overlays[index++] = _style->getOverlay(Theme::Style::OVERLAY_FOCUS);
    }

    if ((overlayTypes & ACTIVE) == ACTIVE)
    {
        overlays[index++] = _style->getOverlay(Theme::Style::OVERLAY_ACTIVE);
    }

    if ((overlayTypes & DISABLED) == DISABLED)
    {
        overlays[index++] = _style->getOverlay(Theme::Style::OVERLAY_DISABLED);
    }

    if ((overlayTypes & HOVER) == HOVER)
    {
        overlays[index++] = _style->getOverlay(Theme::Style::OVERLAY_HOVER);
    }

    return overlays;
}

Theme::Style::Overlay* Control::getOverlay(State state) const
{
    GP_ASSERT(_style);

    switch(state)
    {
    case Control::NORMAL:
        return _style->getOverlay(Theme::Style::OVERLAY_NORMAL);
    case Control::FOCUS:
        return _style->getOverlay(Theme::Style::OVERLAY_FOCUS);
    case Control::ACTIVE:
    {
        Theme::Style::Overlay* activeOverlay = _style->getOverlay(Theme::Style::OVERLAY_ACTIVE);
        if (activeOverlay)
            return activeOverlay;
        else
            return getOverlay(_previousState);
    }
    case Control::DISABLED:
        return _style->getOverlay(Theme::Style::OVERLAY_DISABLED);
    case Control::HOVER:
    {
        Theme::Style::Overlay* hoverOverlay = _style->getOverlay(Theme::Style::OVERLAY_HOVER);
        if (hoverOverlay)
            return hoverOverlay;
        else
            return getOverlay(_previousState);
    }
    default:
        return NULL;
    }
}

void Control::overrideStyle()
{
    if (_styleOverridden)
    {
        return;
    }

    // Copy the style.
    GP_ASSERT(_style);
    _style = new Theme::Style(*_style);
    _styleOverridden = true;
}

void Control::overrideThemedProperties(Properties* properties, unsigned char states)
{
    GP_ASSERT(properties);
    GP_ASSERT(_style);
    GP_ASSERT(_style->_theme);

    Theme::ImageList* imageList = NULL;
    Theme::ThemeImage* cursor = NULL;
    Theme::Skin* skin = NULL;
    _style->_theme->lookUpSprites(properties, &imageList, &cursor, &skin);

    if (imageList)
    {
        setImageList(imageList, states);
    }

    if (cursor)
    {
        setCursor(cursor, states);
    }

    if (skin)
    {
        setSkin(skin, states);
    }

    if (properties->exists("font"))
    {
        Font* font = Font::create(properties->getString("font"));
        setFont(font, states);
        font->release();
    }

    if (properties->exists("fontSize"))
    {
        setFontSize(properties->getInt("fontSize"), states);
    }

    if (properties->exists("textColor"))
    {
        Vector4 textColor(0, 0, 0, 1);
        properties->getColor("textColor", &textColor);
        setTextColor(textColor, states);
    }

    if (properties->exists("textAlignment"))
    {
        setTextAlignment(Font::getJustify(properties->getString("textAlignment")), states);
    }

    if (properties->exists("rightToLeft"))
    {
        setTextRightToLeft(properties->getBool("rightToLeft"), states);
    }

    if (properties->exists("opacity"))
    {
        setOpacity(properties->getFloat("opacity"), states);
    }
}

void Control::setImageList(Theme::ImageList* imageList, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setImageList(imageList);
    }

    _dirty = true;
}

void Control::setCursor(Theme::ThemeImage* cursor, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setCursor(cursor);
    }

    _dirty = true;
}

void Control::setSkin(Theme::Skin* skin, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX && overlays[i]; ++i)
    {
        overlays[i]->setSkin(skin);
    }

    _dirty = true;
}

Theme::Skin* Control::getSkin(State state)
{
    Theme::Style::Overlay* overlay = getOverlay(state);
    GP_ASSERT(overlay);
    return overlay->getSkin();
}

Control::Alignment Control::getAlignment(const char* alignment)
{
    if (!alignment)
    {
        return Control::ALIGN_TOP_LEFT;
    }

    if (strcmp(alignment, "ALIGN_LEFT") == 0)
    {
        return Control::ALIGN_LEFT;
    }
    else if (strcmp(alignment, "ALIGN_HCENTER") == 0)
    {
        return Control::ALIGN_HCENTER;
    }
    else if (strcmp(alignment, "ALIGN_RIGHT") == 0)
    {
        return Control::ALIGN_RIGHT;
    }
    else if (strcmp(alignment, "ALIGN_TOP") == 0)
    {
        return Control::ALIGN_TOP;
    }
    else if (strcmp(alignment, "ALIGN_VCENTER") == 0)
    {
        return Control::ALIGN_VCENTER;
    }
    else if (strcmp(alignment, "ALIGN_BOTTOM") == 0)
    {
        return Control::ALIGN_BOTTOM;
    }
    else if (strcmp(alignment, "ALIGN_TOP_LEFT") == 0)
    {
        return Control::ALIGN_TOP_LEFT;
    }
    else if (strcmp(alignment, "ALIGN_VCENTER_LEFT") == 0)
    {
        return Control::ALIGN_VCENTER_LEFT;
    }
    else if (strcmp(alignment, "ALIGN_BOTTOM_LEFT") == 0)
    {
        return Control::ALIGN_BOTTOM_LEFT;
    }
    else if (strcmp(alignment, "ALIGN_TOP_HCENTER") == 0)
    {
        return Control::ALIGN_TOP_HCENTER;
    }
    else if (strcmp(alignment, "ALIGN_VCENTER_HCENTER") == 0)
    {
        return Control::ALIGN_VCENTER_HCENTER;
    }
    else if (strcmp(alignment, "ALIGN_BOTTOM_HCENTER") == 0)
    {
        return Control::ALIGN_BOTTOM_HCENTER;
    }
    else if (strcmp(alignment, "ALIGN_TOP_RIGHT") == 0)
    {
        return Control::ALIGN_TOP_RIGHT;
    }
    else if (strcmp(alignment, "ALIGN_VCENTER_RIGHT") == 0)
    {
        return Control::ALIGN_VCENTER_RIGHT;
    }
    else if (strcmp(alignment, "ALIGN_BOTTOM_RIGHT") == 0)
    {
        return Control::ALIGN_BOTTOM_RIGHT;
    }
    else
    {
        GP_ERROR("Failed to get corresponding control alignment for unsupported value '%s'.", alignment);
    }

    // Default.
    return Control::ALIGN_TOP_LEFT;
}

}
