#include "Base.h"
#include "Game.h"
#include "Control.h"
#include "Form.h"

#define BOUNDS_X_PERCENTAGE_BIT 1
#define BOUNDS_Y_PERCENTAGE_BIT 2
#define BOUNDS_WIDTH_PERCENTAGE_BIT 4
#define BOUNDS_HEIGHT_PERCENTAGE_BIT 8

namespace gameplay
{

static std::string toString(float v)
{
    std::ostringstream s;
    s << v;
    return s.str();
}

static float parseCoord(const char* s, bool* isPercentage)
{
    const char* p;
    if ((p = strchr(s, '%')) != NULL)
    {
        std::string value(s, (std::string::size_type)(p - s));
        *isPercentage = true;
        return (float)(atof(value.c_str()) * 0.01);
    }
    *isPercentage = false;
    return (float)atof(s);
}

static bool parseCoordPair(const char* s, float* v1, float* v2, bool* v1Percentage, bool* v2Percentage)
{
    size_t len = strlen(s);
    const char* s2 = strchr(s, ',');
    if (s2 == NULL)
        return false;
    std::string v1Str(s, (std::string::size_type)(s2 - s));
    std::string v2Str(s2 + 1);
    *v1 = parseCoord(v1Str.c_str(), v1Percentage);
    *v2 = parseCoord(v2Str.c_str(), v2Percentage);
    return true;
}

Control::Control()
    : _id(""), _enabled(true), _boundsBits(0), _dirty(true), _consumeInputEvents(true), _alignment(ALIGN_TOP_LEFT), _isAlignmentSet(false),
    _autoWidth(AUTO_SIZE_NONE), _autoHeight(AUTO_SIZE_NONE), _listeners(NULL), _visible(true), _zIndex(-1),
    _contactIndex(INVALID_CONTACT_INDEX), _focusIndex(-1), _canFocus(false), _parent(NULL), _styleOverridden(false), _skin(NULL)
{
    addScriptEvent("controlEvent", "<Control>[Control::Listener::EventType]");
}

Control::~Control()
{
    Form::verifyRemovedControlState(this);

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

static Control::AutoSize parseAutoSize(const char* str)
{
    if (str == NULL)
        return Control::AUTO_SIZE_NONE;
    if (strcmp(str, "AUTO_SIZE_STRETCH") == 0 || strcmp(str, "true") == 0) // left for backwards compatibility
        return Control::AUTO_SIZE_STRETCH;
    if (strcmp(str, "AUTO_SIZE_FIT") == 0)
        return Control::AUTO_SIZE_FIT;
    return Control::AUTO_SIZE_NONE;
}

void Control::initialize(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);
    _style = style;

    // Properties not defined by the style.
    const char * alignmentString = properties->getString("alignment");

    _isAlignmentSet = alignmentString != NULL;
    _alignment = getAlignment(alignmentString);

    _autoWidth = parseAutoSize(properties->getString("autoWidth"));
    _autoHeight = parseAutoSize(properties->getString("autoHeight"));

    _consumeInputEvents = properties->getBool("consumeInputEvents", true);

    _visible = properties->getBool("visible", true);

    if (properties->exists("zIndex"))
    {
        _zIndex = properties->getInt("zIndex");
    }
    else
    {
        _zIndex = -1;
    }

    if (properties->exists("canFocus"))
        _canFocus = properties->getBool("canFocus", false);

    if (properties->exists("focusIndex"))
    {
        _focusIndex = properties->getInt("focusIndex");
    }
    else
    {
        _focusIndex = -1;
    }

    float bounds[4];
    bool boundsBits[4];
    if (properties->exists("position"))
    {
        parseCoordPair(properties->getString("position", "0,0"), &bounds[0], &bounds[1], &boundsBits[0], &boundsBits[1]);
    }
    else
    {
        bounds[0] = parseCoord(properties->getString("x", "0"), &boundsBits[0]);
        bounds[1] = parseCoord(properties->getString("y", "0"), &boundsBits[1]);
    }

    if (properties->exists("size"))
    {
        parseCoordPair(properties->getString("size", "0,0"), &bounds[2], &bounds[3], &boundsBits[2], &boundsBits[3]);
    }
    else
    {
        bounds[2] = parseCoord(properties->getString("width", "0"), &boundsBits[2]);
        bounds[3] = parseCoord(properties->getString("height", "0"), &boundsBits[3]);
    }
    setX(bounds[0], boundsBits[0]);
    setY(bounds[1], boundsBits[1]);
    setWidth(bounds[2], boundsBits[2]);
    setHeight(bounds[3], boundsBits[3]);

    const char* id = properties->getId();
    if (id)
        _id = id;

    if (properties->exists("enabled"))
    {
        setEnabled(properties->getBool("enabled"));
    }

    // Register script listeners for control events
    if (properties->exists("listener"))
        addScriptCallback("controlEvent", properties->getString("listener"));

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

float Control::getX() const
{
    return _bounds.x;
}

void Control::setX(float x, bool percentage)
{
    if (_relativeBounds.x != x || percentage != ((_boundsBits & BOUNDS_X_PERCENTAGE_BIT) != 0))
    {
        _relativeBounds.x = x;
        if (percentage)
        {
            _boundsBits |= BOUNDS_X_PERCENTAGE_BIT;
        }
        else
        {
            _boundsBits &= ~BOUNDS_X_PERCENTAGE_BIT;
            _bounds.x = x;
        }
        _dirty = true;
    }
}

bool Control::isXPercentage() const
{
    return (_boundsBits & BOUNDS_X_PERCENTAGE_BIT) != 0;
}

float Control::getY() const
{
    return _bounds.y;
}

void Control::setY(float y, bool percentage)
{
    if (_relativeBounds.y != y || percentage != ((_boundsBits & BOUNDS_Y_PERCENTAGE_BIT) != 0))
    {
        _relativeBounds.y = y;
        if (percentage)
        {
            _boundsBits |= BOUNDS_Y_PERCENTAGE_BIT;
        }
        else
        {
            _boundsBits &= ~BOUNDS_Y_PERCENTAGE_BIT;
            _bounds.y = y;
        }
        _dirty = true;
    }
}

bool Control::isYPercentage() const
{
    return (_boundsBits & BOUNDS_Y_PERCENTAGE_BIT) != 0;
}

float Control::getWidth() const
{
    return _bounds.width;
}

void Control::setWidth(float width, bool percentage)
{
    if (_relativeBounds.width != width || percentage != ((_boundsBits & BOUNDS_WIDTH_PERCENTAGE_BIT) != 0))
    {
        _relativeBounds.width = width;
        if (percentage)
        {
            _boundsBits |= BOUNDS_WIDTH_PERCENTAGE_BIT;
        }
        else
        {
            _boundsBits &= ~BOUNDS_WIDTH_PERCENTAGE_BIT;
            _bounds.width = width;
        }
        _dirty = true;
    }
}

bool Control::isWidthPercentage() const
{
    return (_boundsBits & BOUNDS_WIDTH_PERCENTAGE_BIT) != 0;
}

float Control::getHeight() const
{
    return _bounds.height;
}

void Control::setHeight(float height, bool percentage)
{
    if (_relativeBounds.height != height || percentage != ((_boundsBits & BOUNDS_HEIGHT_PERCENTAGE_BIT) != 0))
    {
        _relativeBounds.height = height;
        if (percentage)
        {
            _boundsBits |= BOUNDS_HEIGHT_PERCENTAGE_BIT;
        }
        else
        {
            _boundsBits &= ~BOUNDS_HEIGHT_PERCENTAGE_BIT;
            _bounds.height = height;
        }
        _dirty = true;
    }
}

bool Control::isHeightPercentage() const
{
    return (_boundsBits & BOUNDS_HEIGHT_PERCENTAGE_BIT) != 0;
}

void Control::setPosition(float x, float y)
{
    setX(x);
    setY(y);
}

void Control::setSize(float width, float height)
{
    setWidth(width);
    setHeight(height);
}

const Rectangle& Control::getBounds() const
{
    return _bounds;
}

void Control::setBounds(const Rectangle& bounds)
{
    setX(bounds.x);
    setY(bounds.y);
    setWidth(bounds.width);
    setHeight(bounds.height);
}

const Rectangle& Control::getAbsoluteBounds() const
{
    return _absoluteBounds;
}

const Rectangle& Control::getClipBounds() const
{
    return _clipBounds;
}

const Rectangle& Control::getClip() const
{
    return _viewportClipBounds;
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

Control::AutoSize Control::getAutoWidth() const
{
    return _autoWidth;
}

void Control::setAutoWidth(bool autoWidth)
{
    setAutoWidth(autoWidth ? AUTO_SIZE_STRETCH : AUTO_SIZE_NONE);
}

void Control::setAutoWidth(AutoSize mode)
{
    if (_autoWidth != mode)
    {
        _autoWidth = mode;
        _dirty = true;
    }
}

Control::AutoSize Control::getAutoHeight() const
{
    return _autoHeight;
}

void Control::setAutoHeight(bool autoHeight)
{
    setAutoHeight(autoHeight ? AUTO_SIZE_STRETCH : AUTO_SIZE_NONE);
}

void Control::setAutoHeight(AutoSize mode)
{
    if (_autoHeight != mode)
    {
        _autoHeight = mode;
        _dirty = true;
    }
}

void Control::setVisible(bool visible)
{
    if (_visible != visible)
    {
        _visible = visible;
        _dirty = true;

        if (!_visible)
            Form::controlDisabled(this);
    }
}

bool Control::isVisible() const
{
    return _visible;
}

bool Control::isVisibleInHierarchy() const
{
    if (!_visible)
        return false;

    if (_parent)
        return _parent->isVisibleInHierarchy();

    return true;
}

bool Control::canFocus() const
{
    return _canFocus;
}

void Control::setCanFocus(bool acceptsFocus)
{
    _canFocus = acceptsFocus;
}

bool Control::hasFocus() const
{
    return (Form::_focusControl == this);
}

bool Control::setFocus()
{
    if (Form::_focusControl != this && canFocus())
    {
        Form::setFocusControl(this);
        return true;
    }

    return false;
}

void Control::setOpacity(float opacity, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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
    if (_enabled != enabled)
    {
        _enabled = enabled;
        _dirty = true;

        if (!_enabled)
            Form::controlDisabled(this);
    }
}

bool Control::isEnabled() const
{
    return _enabled;
}

bool Control::isEnabledInHierarchy() const
{
    if (!_enabled)
        return false;

    if (_parent)
        return _parent->isEnabledInHierarchy();

    return true;
}

void Control::setBorder(float top, float bottom, float left, float right, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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

Control::State Control::getState() const
{
    if (!_enabled)
        return DISABLED;

    if (Form::_activeControl == this)
    {
        if (Form::_activeControlState == ACTIVE)
            return ACTIVE;
        if (Form::_focusControl == this)
            return FOCUS;
        return Form::_activeControlState;
    }
    
    if (Form::_focusControl == this)
        return FOCUS;

    return NORMAL;
}

Theme::Style::OverlayType Control::getOverlayType() const
{
    switch (getState())
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

        if (_parent)
        {
			_parent->sortControls();
        }
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
    return _consumeInputEvents;
}

bool Control::keyEvent(Keyboard::KeyEvent evt, int key)
{
    return _consumeInputEvents;
}

bool Control::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    // Return false instead of _consumeInputEvents to allow handling to be 
    // routed to touchEvent before consuming.
    return false;
}

bool Control::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    return _consumeInputEvents;
}

void Control::notifyListeners(Control::Listener::EventType eventType)
{
    // This method runs untrusted code by notifying listeners of events.
    // If the user calls exit() or otherwise releases this control, we
    // need to keep it alive until the method returns.
    addRef();

    controlEvent(eventType);

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

void Control::controlEvent(Control::Listener::EventType evt)
{
}

void Control::update(const Control* container, const Vector2& offset)
{
    Game* game = Game::getInstance();
    const Rectangle parentAbsoluteBounds = container ? container->_viewportBounds : Rectangle(0, 0, game->getViewport().width, game->getViewport().height);
    const Rectangle parentAbsoluteClip = container ? container->getClip() : parentAbsoluteBounds;

    // Store previous absolute clip bounds
    _clearBounds.set(_absoluteClipBounds);

    // Calculate local un-clipped bounds.
    _bounds.set(_relativeBounds);
    if (isXPercentage())
        _bounds.x *= parentAbsoluteBounds.width;
    if (isYPercentage())
        _bounds.y *= parentAbsoluteBounds.height;
    if (_autoWidth == AUTO_SIZE_STRETCH)
        _bounds.width = parentAbsoluteBounds.width;
    else if (isWidthPercentage())
        _bounds.width *= parentAbsoluteBounds.width;
    if (_autoHeight == AUTO_SIZE_STRETCH)
        _bounds.height = parentAbsoluteBounds.height;
    else if (isHeightPercentage())
        _bounds.height *= parentAbsoluteBounds.height;

    float x, y, width, height, clipX2, x2, clipY2, y2;

    // Calculate the local clipped bounds
    width = _bounds.width;
    height = _bounds.height;
    if (container)
    {
        x = _bounds.x + offset.x;
        y = _bounds.y + offset.y;
        x2 = parentAbsoluteClip.x + x + width;
        y2 = parentAbsoluteClip.y + y + height;
    }
    else
    {
        x = 0;
        y = 0;
        x2 = width;
        y2 = height;
    }
    clipX2 = parentAbsoluteClip.x + parentAbsoluteClip.width;
    clipY2 = parentAbsoluteClip.y + parentAbsoluteClip.height;
    if (x2 > clipX2)
        width -= x2 - clipX2;
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

    // Calculate absolute bounds un-clipped bounds
    if (container)
    {
        x = _bounds.x + offset.x + parentAbsoluteBounds.x;
        y = _bounds.y + offset.y + parentAbsoluteBounds.y;
    }
    else
    {
        x = 0;
        y = 0;
    }
    _absoluteBounds.set(x, y, _bounds.width, _bounds.height);

    // Calculate the absolute viewport bounds (content area, which does not include border and padding)
    // Absolute bounds minus border and padding.
    const Theme::Border& border = getBorder(getState());
    const Theme::Padding& padding = getPadding();
    x += border.left + padding.left;
    y += border.top + padding.top;
    width = _bounds.width - border.left - padding.left - border.right - padding.right;
    height = _bounds.height - border.top - padding.top - border.bottom - padding.bottom;
    _viewportBounds.set(x, y, width, height);

    // Calculate the clip area.
    // Absolute bounds, minus border and padding,
    // clipped to the parent container's clip area.
    if (container)
    {
        clipX2 = parentAbsoluteClip.x + parentAbsoluteClip.width;
        clipY2 = parentAbsoluteClip.y + parentAbsoluteClip.height;
    }
    else
    {
        clipX2 = parentAbsoluteClip.width;
        clipY2 = parentAbsoluteClip.height;
    }
    x2 = x + width;
    if (x2 > clipX2)
        width = clipX2 - x;
    y2 = y + height;
    if (y2 > clipY2)
        height = clipY2 - y;

    if (x < parentAbsoluteClip.x)
    {
        float dx = parentAbsoluteClip.x - x;
        width -= dx;
        x = parentAbsoluteClip.x;
    }

    if (y < parentAbsoluteClip.y)
    {
        float dy = parentAbsoluteClip.y - y;
        height -= dy;
        y = parentAbsoluteClip.y;
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
    _skin = getSkin(getState());

    // Current opacity should be multiplied by that of the parent container.
    _opacity = getOpacity(getState());
    if (container)
        _opacity *= container->_opacity;
}

void Control::drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    if (!spriteBatch || !_skin || _absoluteBounds.width <= 0 || _absoluteBounds.height <= 0)
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
    const Theme::Border& border = getBorder(getState());
    const Theme::Padding& padding = getPadding();
    Vector4 skinColor = _skin->getColor();
    skinColor.w *= _opacity;

    float midWidth = _absoluteBounds.width - border.left - border.right;
    float midHeight = _absoluteBounds.height - border.top - border.bottom;
    float midX = _absoluteBounds.x + border.left;
    float midY = _absoluteBounds.y + border.top;
    float rightX = _absoluteBounds.x + _absoluteBounds.width - border.right;
    float bottomY = _absoluteBounds.y + _absoluteBounds.height - border.bottom;

    // Draw themed border sprites.
    if (!border.left && !border.right && !border.top && !border.bottom)
    {
        // No border, just draw the image.
        spriteBatch->draw(_absoluteBounds.x, _absoluteBounds.y, _absoluteBounds.width, _absoluteBounds.height, center.u1, center.v1, center.u2, center.v2, skinColor, clip);
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
        spriteBatch->draw(_absoluteBounds.x + border.left, _absoluteBounds.y + border.top, _absoluteBounds.width - border.left - border.right, _absoluteBounds.height - border.top - border.bottom,
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
    {
        _dirty = false;
        return;
    }

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

Control* Control::getParent() const
{
    return _parent;
}

bool Control::isChild(Control* control) const
{
    if (!control)
        return false;

    Control* parent = _parent;
    while (parent)
    {
        if (parent == control)
            return true;
        parent = parent->_parent;
    }

    return false;
}

Form* Control::getTopLevelForm() const
{
    if (_parent)
        return _parent->getTopLevelForm();

    if (isContainer())
    {
        Container* container = static_cast<Container*>(const_cast<Control*>(this));
        if (container->isForm())
            return static_cast<Form*>(container);
    }

    return NULL;
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

    switch (propertyId)
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
        setX(Curve::lerp(blendWeight, _bounds.x, value->getFloat(0)), isXPercentage());
        setY(Curve::lerp(blendWeight, _bounds.y, value->getFloat(1)), isYPercentage());
        break;
    case ANIMATE_POSITION_X:
        setX(Curve::lerp(blendWeight, _bounds.x, value->getFloat(0)), isXPercentage());
        break;
    case ANIMATE_POSITION_Y:
        setY(Curve::lerp(blendWeight, _bounds.y, value->getFloat(0)), isYPercentage());
        break;
    case ANIMATE_SIZE:
        setWidth(Curve::lerp(blendWeight, _bounds.width, value->getFloat(0)), isWidthPercentage());
        setHeight(Curve::lerp(blendWeight, _bounds.height, value->getFloat(1)), isHeightPercentage());
        break;
    case ANIMATE_SIZE_WIDTH:
        setWidth(Curve::lerp(blendWeight, _bounds.width, value->getFloat(0)), isWidthPercentage());
        break;
    case ANIMATE_SIZE_HEIGHT:
        setHeight(Curve::lerp(blendWeight, _bounds.height, value->getFloat(0)), isHeightPercentage());
        break;
    case ANIMATE_OPACITY:
        setOpacity(Curve::lerp(blendWeight, _opacity, value->getFloat(0)));
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

    Theme::Style::Overlay* overlay = NULL;

    switch (state)
    {
    case Control::NORMAL:
        return _style->getOverlay(Theme::Style::OVERLAY_NORMAL);

    case Control::FOCUS:
        overlay = _style->getOverlay(Theme::Style::OVERLAY_FOCUS);
        break;

    case Control::ACTIVE:
        overlay = _style->getOverlay(Theme::Style::OVERLAY_ACTIVE);
        if (!overlay && hasFocus())
            overlay = _style->getOverlay(Theme::Style::OVERLAY_FOCUS);
        break;

    case Control::DISABLED:
        overlay = _style->getOverlay(Theme::Style::OVERLAY_DISABLED);
        break;

    case Control::HOVER:
        overlay = _style->getOverlay(Theme::Style::OVERLAY_HOVER);
        if (!overlay && hasFocus())
            overlay = _style->getOverlay(Theme::Style::OVERLAY_FOCUS);
        break;
    }

    // Fall back to normal overlay if more specific state overlay not found
    if (!overlay)
        overlay = _style->getOverlay(Theme::Style::OVERLAY_NORMAL);

    return overlay;
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

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
            overlays[i]->setImageList(imageList);
    }

    _dirty = true;
}

void Control::setCursor(Theme::ThemeImage* cursor, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
            overlays[i]->setCursor(cursor);
    }

    _dirty = true;
}

void Control::setSkin(Theme::Skin* skin, unsigned char states)
{
    overrideStyle();
    Theme::Style::Overlay* overlays[Theme::Style::OVERLAY_MAX] = { 0 };
    getOverlays(states, overlays);

    for (int i = 0; i < Theme::Style::OVERLAY_MAX; ++i)
    {
        if( overlays[i] )
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
