#include "Base.h"
#include "Container.h"
#include "Layout.h"
#include "AbsoluteLayout.h"
#include "FlowLayout.h"
#include "VerticalLayout.h"
#include "Label.h"
#include "Button.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "Slider.h"
#include "TextBox.h"
#include "Joystick.h"
#include "ImageControl.h"
#include "Game.h"

namespace gameplay
{

// If the user stops scrolling for this amount of time (in millis) before touch/click release, don't apply inertia.
static const long SCROLL_INERTIA_DELAY = 100L;
// Factor to multiply friction by before applying to velocity.
static const float SCROLL_FRICTION_FACTOR = 5.0f;
// Distance that must be scrolled before isScrolling() will return true, used e.g. to cancel button-click events.
static const float SCROLL_THRESHOLD = 10.0f;
// Distance a joystick must be pushed in order to trigger focus-change and/or scrolling.
static const float JOYSTICK_THRESHOLD = 0.75f;
// Scroll speed when using a DPad -- max scroll speed when using a joystick.
static const float GAMEPAD_SCROLL_SPEED = 500.0f;
// If the DPad or joystick is held down, this is the initial delay in milliseconds between focus change events.
static const float FOCUS_CHANGE_REPEAT_DELAY = 300.0f;

/**
 * Sort function for use with _controls.sort(), based on Z-Order.
 * 
 * @param c1 The first control
 * @param c2 The second control
 * return true if the first controls z index is less than the second.
 */
static bool sortControlsByZOrder(Control* c1, Control* c2);

Container::Container()
    : _layout(NULL), _scrollBarTopCap(NULL), _scrollBarVertical(NULL), _scrollBarBottomCap(NULL),
      _scrollBarLeftCap(NULL), _scrollBarHorizontal(NULL), _scrollBarRightCap(NULL),
      _scroll(SCROLL_NONE), _scrollBarBounds(Rectangle::empty()), _scrollPosition(Vector2::zero()),
      _scrollBarsAutoHide(false), _scrollBarOpacity(1.0f), _scrolling(false),
      _scrollingVeryFirstX(0), _scrollingVeryFirstY(0), _scrollingFirstX(0), _scrollingFirstY(0), _scrollingLastX(0), _scrollingLastY(0),
      _scrollingStartTimeX(0), _scrollingStartTimeY(0), _scrollingLastTime(0),
      _scrollingVelocity(Vector2::zero()), _scrollingFriction(1.0f), _scrollWheelSpeed(400.0f),
      _scrollingRight(false), _scrollingDown(false),
      _scrollingMouseVertically(false), _scrollingMouseHorizontally(false),
      _scrollBarOpacityClip(NULL), _zIndexDefault(0), _focusIndexDefault(0), _focusIndexMax(0),
      _focusPressed(0), _selectButtonDown(false),
      _lastFrameTime(0), _focusChangeRepeat(false),
      _focusChangeStartTime(0), _focusChangeRepeatDelay(FOCUS_CHANGE_REPEAT_DELAY), _focusChangeCount(0),
      _totalWidth(0), _totalHeight(0),
      _contactIndices(0), _initializedWithScroll(false), _scrollWheelRequiresFocus(false)
{
}

Container::~Container()
{
    std::vector<Control*>::iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        SAFE_RELEASE((*it));
    }
    SAFE_RELEASE(_layout);
}

Container* Container::create(const char* id, Theme::Style* style, Layout::Type layoutType)
{
    GP_ASSERT(style);

    Container* container = Container::create(layoutType);
    if (id)
        container->_id = id;
    container->_style = style;
    return container;
}

Container* Container::create(Layout::Type type)
{
    Layout* layout = NULL;
    switch (type)
    {
    case Layout::LAYOUT_ABSOLUTE:
        layout = AbsoluteLayout::create();
        break;
    case Layout::LAYOUT_FLOW:
        layout = FlowLayout::create();
        break;
    case Layout::LAYOUT_VERTICAL:
        layout = VerticalLayout::create();
        break;
    }

    Container* container = new Container();
    container->_layout = layout;

    return container;
}

Container* Container::create(Theme::Style* style, Properties* properties, Theme* theme)
{
    GP_ASSERT(properties);

    const char* layoutString = properties->getString("layout");
    Container* container = Container::create(getLayoutType(layoutString));
    container->initialize(style, properties);
    container->_scroll = getScroll(properties->getString("scroll"));
    container->_scrollBarsAutoHide = properties->getBool("scrollBarsAutoHide");
    if (container->_scrollBarsAutoHide)
    {
        container->_scrollBarOpacity = 0.0f;
    }
    
    container->_scrollWheelRequiresFocus = properties->getBool("scrollWheelRequiresFocus");
    if (properties->exists("scrollingFriction"))
        container->_scrollingFriction = properties->getFloat("scrollingFriction");
    if (properties->exists("scrollWheelSpeed"))
        container->_scrollWheelSpeed = properties->getFloat("scrollWheelSpeed");

    container->addControls(theme, properties);
    container->_layout->update(container, container->_scrollPosition);

    return container;
}

void Container::addControls(Theme* theme, Properties* properties)
{
    GP_ASSERT(theme);
    GP_ASSERT(properties);

    // Add all the controls to this container.
    Properties* controlSpace = properties->getNextNamespace();
    while (controlSpace != NULL)
    {
        Control* control = NULL;

        const char* controlStyleName = controlSpace->getString("style");
        Theme::Style* controlStyle = NULL;
        if (controlStyleName)
        {
            controlStyle = theme->getStyle(controlStyleName);
        }
        else
        {
            controlStyle = theme->getEmptyStyle();
        }

        std::string controlName(controlSpace->getNamespace());
        std::transform(controlName.begin(), controlName.end(), controlName.begin(), (int(*)(int))toupper);
        if (controlName == "LABEL")
        {
            control = Label::create(controlStyle, controlSpace);
        }
        else if (controlName == "BUTTON")
        {
            control = Button::create(controlStyle, controlSpace);
        }
        else if (controlName == "CHECKBOX")
        {
            control = CheckBox::create(controlStyle, controlSpace);
        }
        else if (controlName == "RADIOBUTTON")
        {
            control = RadioButton::create(controlStyle, controlSpace);
        }
        else if (controlName == "CONTAINER")
        {
            control = Container::create(controlStyle, controlSpace, theme);
        }
        else if (controlName == "SLIDER")
        {
            control = Slider::create(controlStyle, controlSpace);
        }
        else if (controlName == "TEXTBOX")
        {
            control = TextBox::create(controlStyle, controlSpace);
        }
        else if (controlName == "JOYSTICK")
        {
            control = Joystick::create(controlStyle, controlSpace);
        }
        else if (controlName == "IMAGE")
        {
            control = ImageControl::create(controlStyle, controlSpace);
        }
        else
        {
            // Ignore - not a valid control name.
            // This used to fail, but I see no reason to hard fail here (this also fixes not being able
            // to set padding on containers).
        }

        // Add the new control to the form.
        if (control)
        {
            addControl(control);
            control->release();
        }

        // Get the next control.
        controlSpace = properties->getNextNamespace();
    }

    // Sort controls by Z-Order.
    sortControls();
}

Layout* Container::getLayout()
{
    return _layout;
}

unsigned int Container::addControl(Control* control)
{
    GP_ASSERT(control);

    if (control->_parent && control->_parent != this)
    {
        control->_parent->removeControl(control);
    }

    if (control->getZIndex() == -1)
    {
        control->setZIndex(_zIndexDefault++);
    }

    if (control->getFocusIndex() == -1)
    {
        control->setFocusIndex(_focusIndexDefault++);
    }

    int focusIndex = control->getFocusIndex();
    if (focusIndex > _focusIndexMax)
        _focusIndexMax = focusIndex;

    if (control->_parent != this)
    {
        _controls.push_back(control);
        control->addRef();
        control->_parent = this;
        sortControls();
        return (unsigned int)(_controls.size() - 1);
    }
    else
    {
        // Control is already in this container.
        // Do nothing but determine and return control's index.
        const size_t size = _controls.size();
        for (size_t i = 0; i < size; ++i)
        {
            Control* c = _controls[i];
            if (c == control)
            {
                return (unsigned int)i;
            }
        }

        // Should never reach this.
        GP_ASSERT(false);
        return 0;
    }
}

void Container::insertControl(Control* control, unsigned int index)
{
    GP_ASSERT(control);

    if (control->_parent && control->_parent != this)
    {
        control->_parent->removeControl(control);
    }

    if (control->_parent != this)
    {
        std::vector<Control*>::iterator it = _controls.begin() + index;
        _controls.insert(it, control);
        control->addRef();
        control->_parent = this;
    }
}

void Container::removeControl(unsigned int index)
{
    GP_ASSERT(index < _controls.size());

    std::vector<Control*>::iterator it = _controls.begin() + index;
    _controls.erase(it);
    Control* control = *it;
    control->_parent = NULL;
    SAFE_RELEASE(control);
}

void Container::removeControl(const char* id)
{
    GP_ASSERT(id);
    std::vector<Control*>::iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* c = *it;
        if (strcmp(id, c->getId()) == 0)
        {
            c->_parent = NULL;
            SAFE_RELEASE(c);
            _controls.erase(it);
            return;
        }
    }
}

void Container::removeControl(Control* control)
{
    GP_ASSERT(control);
    std::vector<Control*>::iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        if (*it == control)
        {
            control->_parent = NULL;
            SAFE_RELEASE(control);
            _controls.erase(it);
            return;
        }
    }
}

Control* Container::getControl(unsigned int index) const
{
    std::vector<Control*>::const_iterator it = _controls.begin() + index;
    return *it;
}

Control* Container::getControl(const char* id) const
{
    GP_ASSERT(id);
    std::vector<Control*>::const_iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* c = *it;
        GP_ASSERT(c);
        if (strcmp(id, c->getId()) == 0)
        {
            return c;
        }
        else if (c->isContainer())
        {
            Control* cc = ((Container*)c)->getControl(id);
            if (cc)
            {
                return cc;
            }
        }
    }
    return NULL;
}

const std::vector<Control*>& Container::getControls() const
{
    return _controls;
}

void Container::setScroll(Scroll scroll)
{
    if (scroll != _scroll)
    {
        _scroll = scroll;
        _dirty = true;
    }
}

Container::Scroll Container::getScroll() const
{
    return _scroll;
}

void Container::setScrollBarsAutoHide(bool autoHide)
{
    if (autoHide != _scrollBarsAutoHide)
    {
        _scrollBarsAutoHide = autoHide;
        _dirty = true;
    }
}

bool Container::isScrollBarsAutoHide() const
{
    return _scrollBarsAutoHide;
}

bool Container::isScrolling() const
{
    if (_parent && _parent->isScrolling())
        return true;

    return (_scrolling &&
            (abs(_scrollingLastX - _scrollingVeryFirstX) > SCROLL_THRESHOLD ||
             abs(_scrollingLastY - _scrollingVeryFirstY) > SCROLL_THRESHOLD));
}

Animation* Container::getAnimation(const char* id) const
{
    std::vector<Control*>::const_iterator itr = _controls.begin();
    std::vector<Control*>::const_iterator end = _controls.end();
    Control* control = NULL;
    for (; itr != end; itr++)
    {
        control = *itr;
        GP_ASSERT(control);
        Animation* animation = control->getAnimation(id);
        if (animation)
            return animation;

        if (control->isContainer())
        {
            animation = ((Container*)control)->getAnimation(id);
            if (animation)
                return animation;
        }
    }
    return NULL;
}

const char* Container::getType() const
{
    return "container";
}

bool Container::getScrollWheelRequiresFocus() const
{
    return _scrollWheelRequiresFocus;
}

void Container::setScrollWheelRequiresFocus(bool required)
{
    _scrollWheelRequiresFocus = required;
}

void Container::update(const Control* container, const Vector2& offset)
{
    // Update this container's viewport.
    Control::update(container, offset);

    // Get scrollbar images and diminish clipping bounds to make room for scrollbars.
    if ((_scroll & SCROLL_HORIZONTAL) == SCROLL_HORIZONTAL)
    {
        _scrollBarLeftCap = getImage("scrollBarLeftCap", _state);
        _scrollBarHorizontal = getImage("horizontalScrollBar", _state);
        _scrollBarRightCap = getImage("scrollBarRightCap", _state);

        GP_ASSERT(_scrollBarLeftCap && _scrollBarHorizontal && _scrollBarRightCap);

        _viewportClipBounds.height -= _scrollBarHorizontal->getRegion().height;
    }

    if ((_scroll & SCROLL_VERTICAL) == SCROLL_VERTICAL)
    {
        _scrollBarTopCap = getImage("scrollBarTopCap", _state);
        _scrollBarVertical = getImage("verticalScrollBar", _state);
        _scrollBarBottomCap = getImage("scrollBarBottomCap", _state);

        GP_ASSERT(_scrollBarTopCap && _scrollBarVertical && _scrollBarBottomCap);
        
        _viewportClipBounds.width -= _scrollBarVertical->getRegion().width;
    }

    GP_ASSERT(_layout);
    if (_scroll != SCROLL_NONE)
    {
        updateScroll();
    }
    else
    {
        _layout->update(this, Vector2::zero());
    }
}

void Container::draw(SpriteBatch* spriteBatch, const Rectangle& clip, bool needsClear, bool cleared, float targetHeight)
{
    if (needsClear)
    {
        GL_ASSERT( glEnable(GL_SCISSOR_TEST) );
        float clearY = targetHeight - _clearBounds.y - _clearBounds.height;
        GL_ASSERT( glScissor(_clearBounds.x, clearY, _clearBounds.width, _clearBounds.height) );
        Game::getInstance()->clear(Game::CLEAR_COLOR, Vector4::zero(), 1.0f, 0);
        GL_ASSERT( glDisable(GL_SCISSOR_TEST) );
        needsClear = false;
        cleared = true;
    }
    else if (!cleared)
    {
        needsClear = true;
    }

    if (!_visible)
        return;

    spriteBatch->start();
    Control::drawBorder(spriteBatch, clip);
    spriteBatch->finish();

    std::vector<Control*>::const_iterator it;
    Rectangle boundsUnion = Rectangle::empty();
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* control = *it;
        GP_ASSERT(control);
        if (!needsClear || control->isDirty() || control->_clearBounds.intersects(boundsUnion))
        {
            control->draw(spriteBatch, _viewportClipBounds, needsClear, cleared, targetHeight);
            Rectangle::combine(control->_clearBounds, boundsUnion, &boundsUnion);
        }
    }

    if (_scroll != SCROLL_NONE && (_scrollBarOpacity > 0.0f))
    {
        // Draw scroll bars.
        Rectangle clipRegion(_viewportClipBounds);

        spriteBatch->start();

        if (_scrollBarBounds.height > 0 && ((_scroll & SCROLL_VERTICAL) == SCROLL_VERTICAL))
        {
            const Rectangle& topRegion = _scrollBarTopCap->getRegion();
            const Theme::UVs& topUVs = _scrollBarTopCap->getUVs();
            Vector4 topColor = _scrollBarTopCap->getColor();
            topColor.w *= _scrollBarOpacity * _opacity;

            const Rectangle& verticalRegion = _scrollBarVertical->getRegion();
            const Theme::UVs& verticalUVs = _scrollBarVertical->getUVs();
            Vector4 verticalColor = _scrollBarVertical->getColor();
            verticalColor.w *= _scrollBarOpacity * _opacity;

            const Rectangle& bottomRegion = _scrollBarBottomCap->getRegion();
            const Theme::UVs& bottomUVs = _scrollBarBottomCap->getUVs();
            Vector4 bottomColor = _scrollBarBottomCap->getColor();
            bottomColor.w *= _scrollBarOpacity * _opacity;

            clipRegion.width += verticalRegion.width;

            Rectangle bounds(_viewportBounds.x + _viewportBounds.width - verticalRegion.width, _viewportBounds.y + _scrollBarBounds.y, topRegion.width, topRegion.height);
            spriteBatch->draw(bounds.x, bounds.y, bounds.width, bounds.height, topUVs.u1, topUVs.v1, topUVs.u2, topUVs.v2, topColor, clipRegion);

            bounds.y += topRegion.height;
            bounds.height = _scrollBarBounds.height - topRegion.height - bottomRegion.height;
            spriteBatch->draw(bounds.x, bounds.y, bounds.width, bounds.height, verticalUVs.u1, verticalUVs.v1, verticalUVs.u2, verticalUVs.v2, verticalColor, clipRegion);

            bounds.y += bounds.height;
            bounds.height = bottomRegion.height;
            spriteBatch->draw(bounds.x, bounds.y, bounds.width, bounds.height, bottomUVs.u1, bottomUVs.v1, bottomUVs.u2, bottomUVs.v2, bottomColor, clipRegion);
        }

        if (_scrollBarBounds.width > 0 && ((_scroll & SCROLL_HORIZONTAL) == SCROLL_HORIZONTAL))
        {
            const Rectangle& leftRegion = _scrollBarLeftCap->getRegion();
            const Theme::UVs& leftUVs = _scrollBarLeftCap->getUVs();
            Vector4 leftColor = _scrollBarLeftCap->getColor();
            leftColor.w *= _scrollBarOpacity * _opacity;

            const Rectangle& horizontalRegion = _scrollBarHorizontal->getRegion();
            const Theme::UVs& horizontalUVs = _scrollBarHorizontal->getUVs();
            Vector4 horizontalColor = _scrollBarHorizontal->getColor();
            horizontalColor.w *= _scrollBarOpacity * _opacity;

            const Rectangle& rightRegion = _scrollBarRightCap->getRegion();
            const Theme::UVs& rightUVs = _scrollBarRightCap->getUVs();
            Vector4 rightColor = _scrollBarRightCap->getColor();
            rightColor.w *= _scrollBarOpacity * _opacity;

            clipRegion.height += horizontalRegion.height;
        
            Rectangle bounds(_viewportBounds.x + _scrollBarBounds.x, _viewportBounds.y + _viewportBounds.height - horizontalRegion.height, leftRegion.width, leftRegion.height);
            spriteBatch->draw(bounds.x, bounds.y, bounds.width, bounds.height, leftUVs.u1, leftUVs.v1, leftUVs.u2, leftUVs.v2, leftColor, clipRegion);

            bounds.x += leftRegion.width;
            bounds.width = _scrollBarBounds.width - leftRegion.width - rightRegion.width;
            spriteBatch->draw(bounds.x, bounds.y, bounds.width, bounds.height, horizontalUVs.u1, horizontalUVs.v1, horizontalUVs.u2, horizontalUVs.v2, horizontalColor, clipRegion);

            bounds.x += bounds.width;
            bounds.width = rightRegion.width;
            spriteBatch->draw(bounds.x, bounds.y, bounds.width, bounds.height, rightUVs.u1, rightUVs.v1, rightUVs.u2, rightUVs.v2, rightColor, clipRegion);
        }

        spriteBatch->finish();

        if (_scrollingVelocity.isZero())
        {
            _dirty = false;
        }
    }
    else
    {
        _dirty = false;
    }
}

bool Container::isDirty()
{
    if (_dirty)
    {
        return true;
    }
    else
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            GP_ASSERT(*it);
            if ((*it)->isDirty())
            {
                return true;
            }
        }
    }

    return false;
}

bool Container::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    return pointerEvent(false, evt, x, y, (int)contactIndex);
}

bool Container::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return pointerEvent(true, evt, x, y, wheelDelta);
}

bool Container::keyEvent(Keyboard::KeyEvent evt, int key)
{
    // This event may run untrusted code by notifying listeners of events.
    // If the user calls exit() or otherwise releases this container, we
    // need to keep it alive until the method returns.
    addRef();

    bool eventConsumed = false;

    std::vector<Control*>::const_iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* control = *it;
        GP_ASSERT(control);
        if (!control->isEnabled() || !control->isVisible())
        {
            continue;
        }

        if ((control->hasFocus() || control->getState() == ACTIVE) && control->keyEvent(evt, key))
        {
            eventConsumed |= true;
            break;
        }
    }

    switch (evt)
    {
        case Keyboard::KEY_PRESS:
        {
            if (!eventConsumed)
            {
                switch (key)
                {
                case Keyboard::KEY_TAB:
                    _focusPressed |= NEXT;
                    if (moveFocus(NEXT))
                        eventConsumed |= true;
                    break;
                case Keyboard::KEY_UP_ARROW:
                    _focusPressed |= UP;
                    if (moveFocus(UP))
                        eventConsumed |= true;
                    break;
                case Keyboard::KEY_DOWN_ARROW:
                    _focusPressed |= DOWN;
                    if (moveFocus(DOWN))
                        eventConsumed |= true;
                    break;
                case Keyboard::KEY_LEFT_ARROW:
                    _focusPressed |= LEFT;
                    if (moveFocus(LEFT))
                        eventConsumed |= true;
                    break;
                case Keyboard::KEY_RIGHT_ARROW:
                    _focusPressed |= RIGHT;
                    if (moveFocus(RIGHT))
                        eventConsumed |= true;
                    break;
                }
            }
            break;
        }
        case Keyboard::KEY_RELEASE:
        {
            switch (key)
            {
            case Keyboard::KEY_TAB:
                _focusPressed &= ~NEXT;
                eventConsumed |= true;
                break;
            case Keyboard::KEY_UP_ARROW:
                _focusPressed &= ~UP;
                eventConsumed |= true;
                break;
            case Keyboard::KEY_DOWN_ARROW:
                _focusPressed &= ~DOWN;
                eventConsumed |= true;
                break;
            case Keyboard::KEY_LEFT_ARROW:
                _focusPressed &= ~LEFT;
                eventConsumed |= true;
                break;
            case Keyboard::KEY_RIGHT_ARROW:
                _focusPressed &= ~RIGHT;
                eventConsumed |= true;
                break;
            }
            break;
        }
    }

    release();
    return eventConsumed;
}

void Container::guaranteeFocus(Control* inFocus)
{
    std::vector<Control*>::const_iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* control = *it;
        GP_ASSERT(control);
        if (control == inFocus)
            continue;

        if (control->isContainer())
        {
            ((Container*)control)->guaranteeFocus(inFocus);
        }
        else if (control->hasFocus())
        {
            control->setState(NORMAL);
            return;
        }
    }
}

bool Container::moveFocus(Direction direction, Control* outsideControl)
{
    _direction = direction;

    Control* start = outsideControl;
    if (!start)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            GP_ASSERT(control);
            if (control->hasFocus())
            {
                start = control;
                break;
            }
        }
    }

    int focusIndex = 0;
    Control* next = NULL;
    if (start)
    {
        const Rectangle& startBounds = start->getAbsoluteBounds();
        Vector2 vStart, vNext;
        float distance = FLT_MAX;
        start->setState(Control::NORMAL);

        switch(direction)
        {
        case UP:
            vStart.set(startBounds.x + startBounds.width * 0.5f,
                        startBounds.y);
            break;
        case DOWN:
            vStart.set(startBounds.x + startBounds.width * 0.5f,
                        startBounds.bottom());
            break;
        case LEFT:
            vStart.set(startBounds.x,
                        startBounds.y + startBounds.height * 0.5f);
            break;
        case RIGHT:
            vStart.set(startBounds.right(),
                        startBounds.y + startBounds.height * 0.5f);
            break;
        case NEXT:
            break;
        }

        if (direction != NEXT)
        {
            std::vector<Control*>::const_iterator itt;
            for (itt = _controls.begin(); itt < _controls.end(); itt++)
            {
                Control* nextControl = *itt;

                if (nextControl == start || nextControl->getFocusIndex() < 0 ||
                    !nextControl->isEnabled() || !nextControl->isVisible())
                {
                    // Control is not focusable.
                    continue;
                }

                const Rectangle& nextBounds = nextControl->getAbsoluteBounds();
                switch(direction)
                {
                case UP:
                    vNext.set(nextBounds.x + nextBounds.width * 0.5f,
                              nextBounds.bottom());
                    if (vNext.y > vStart.y) continue;
                    break;
                case DOWN:
                    vNext.set(nextBounds.x + nextBounds.width * 0.5f,
                              nextBounds.y);
                    if (vNext.y < vStart.y) continue;
                    break;
                case LEFT:
                    vNext.set(nextBounds.right(),
                              nextBounds.y + nextBounds.height * 0.5f);
                    if (vNext.x > vStart.x) continue;
                    break;
                case RIGHT:
                    vNext.set(nextBounds.x,
                              nextBounds.y + nextBounds.height * 0.5f);
                    if (vNext.x < vStart.x) continue;
                    break;
                }

                float nextDistance = vStart.distance(vNext);
                if (abs(nextDistance) < distance)
                {
                    distance = nextDistance;
                    next = nextControl;
                }
            }
        }

        if (!next)
        {
            // Check for controls in the given direction in our parent container.
            if (direction != NEXT && !outsideControl && _parent && _parent->moveFocus(direction, start))
            {
                setState(NORMAL);
                _focusChangeRepeat = false;
                _focusPressed = 0;
                return true;
            }
            
            // No control is in the given direction.  Move to the next control in the focus order.
            int focusDelta;
            switch(direction)
            {
            case UP:
            case LEFT:
                focusDelta = -1;
                break;
            case DOWN:
            case RIGHT:
            case NEXT:
                focusDelta = 1;
                break;
            }

            // Find the index to search for.
            if (outsideControl)
            {
                focusIndex = outsideControl->_parent->getFocusIndex() + focusDelta;
            }
            else
            {
                focusIndex = start->getFocusIndex() + focusDelta;
            }

            if (focusIndex > _focusIndexMax)
            {
                if (direction == NEXT && !outsideControl && _parent && _parent->moveFocus(direction, start))
                {
                    setState(NORMAL);
                    _focusChangeRepeat = false;
                    _focusPressed = 0;
                    return true;
                }

                focusIndex = 0;
            }
            else if (focusIndex < 0)
            {
                focusIndex = _focusIndexMax;
            }
        }
    }

    if (!next)
    {
        std::vector<Control*>::const_iterator itt;
        for (itt = _controls.begin(); itt < _controls.end(); itt++)
        {
            Control* nextControl = *itt;
            if (nextControl->getFocusIndex() == focusIndex)
            {
                next = nextControl;
                break;
            }
        }
    }

    // If we haven't found next by now, then there are no focusable controls in this container.
    if (next)
    {
        next->setState(Control::FOCUS);
        _dirty = true;

        if (next->isContainer())
        {
            if ((direction == NEXT && ((Container*)next)->moveFocus(direction)) ||
                ((Container*)next)->moveFocus(direction, start))
            {
                _focusChangeRepeat = false;
                _focusPressed = 0;
                return true;
            }
        }

        // If the next control is not fully visible, scroll the container so that it is.
        const Rectangle& bounds = next->getBounds();
        if (bounds.x < _scrollPosition.x)
        {
            // Control is to the left of the scrolled viewport.
            _scrollPosition.x = -bounds.x;
        }
        else if (bounds.x + bounds.width > _scrollPosition.x + _viewportBounds.width)
        {
            // Control is off to the right.
            _scrollPosition.x = -(bounds.x + bounds.width - _viewportBounds.width);
        }

        if (bounds.y < _viewportBounds.y - _scrollPosition.y)
        {
            // Control is above the viewport.
            _scrollPosition.y = -bounds.y;
        }
        else if (bounds.y + bounds.height > _viewportBounds.height - _scrollPosition.y)
        {
            // Control is below the viewport.
            _scrollPosition.y = -(bounds.y + bounds.height - _viewportBounds.height);
        }

        if (outsideControl && outsideControl->_parent)
        {
            _focusPressed = outsideControl->_parent->_focusPressed;
            _focusChangeCount = outsideControl->_parent->_focusChangeCount;
            _focusChangeRepeatDelay = outsideControl->_parent->_focusChangeRepeatDelay;
            outsideControl->_parent->guaranteeFocus(next);
        }

        _focusChangeStartTime = Game::getAbsoluteTime();
        _focusChangeRepeat = true;
        addRef();
        Game::getInstance()->schedule(_focusChangeRepeatDelay, this);

        return true;
    }

    return false;
}

void Container::timeEvent(long timeDiff, void* cookie)
{
    double time = Game::getAbsoluteTime();
    if (_focusChangeRepeat && hasFocus() && _focusPressed &&
        abs(time - timeDiff - _focusChangeRepeatDelay - _focusChangeStartTime) < 50)
    {
        ++_focusChangeCount;
        if (_focusChangeCount == 5)
        {
            _focusChangeRepeatDelay *= 0.5;
        }
        moveFocus(_direction);
    }
    else
    {
        _focusChangeCount = 0;
        _focusChangeRepeatDelay = FOCUS_CHANGE_REPEAT_DELAY;
    }

    release();
}

void Container::startScrolling(float x, float y, bool resetTime)
{
    _scrollingVelocity.set(-x, y);
    _scrolling = true;
    _scrollBarOpacity = 1.0f;
    _dirty = true;

    if (_scrollBarOpacityClip && _scrollBarOpacityClip->isPlaying())
    {
        _scrollBarOpacityClip->stop();
        _scrollBarOpacityClip = NULL;
    }

    if (resetTime)
    {
        _lastFrameTime = Game::getGameTime();
    }
}

void Container::stopScrolling()
{
    _scrollingVelocity.set(0, 0);
    _scrolling = false;
    _dirty = true;
}

bool Container::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    addRef();

    bool eventConsumed = false;

    // Pass the event to any control that is active or in focus.
    std::vector<Control*>::const_iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* control = *it;
        GP_ASSERT(control);
        if (control->hasFocus() || control->getState() == Control::ACTIVE)
        {
            eventConsumed |= control->gamepadEvent(evt, gamepad, analogIndex);
            break;
        }
    }

    // First check if a selection button is down.
    if (!_selectButtonDown)
    {
        if (gamepad->isButtonDown(Gamepad::BUTTON_A) ||
            gamepad->isButtonDown(Gamepad::BUTTON_X))
        {
            _selectButtonDown = true;
            _focusChangeRepeat = false;
            eventConsumed |= _consumeInputEvents;
        }
    }
    else
    {
        if (!gamepad->isButtonDown(Gamepad::BUTTON_A) &&
            !gamepad->isButtonDown(Gamepad::BUTTON_X))
        {
            _selectButtonDown = false;
        }
    }

    Vector2 joystick;
    gamepad->getJoystickValues(analogIndex, &joystick);

    // Don't allow focus changes or scrolling while a selection button is down.
    if (!_selectButtonDown && !eventConsumed)
    {
        switch (evt)
        {
            case Gamepad::BUTTON_EVENT:
            {
                // Shift focus forward or backward when the DPad is used.
                if (!(_focusPressed & DOWN) &&
                    gamepad->isButtonDown(Gamepad::BUTTON_DOWN))
                {
                    _focusPressed |= DOWN;
                    eventConsumed = true;
                    if (moveFocus(DOWN))
                        break;
                    else
                        startScrolling(0, -GAMEPAD_SCROLL_SPEED);
                }
                    
                if (!(_focusPressed & RIGHT) &&
                    gamepad->isButtonDown(Gamepad::BUTTON_RIGHT))
                {
                    _focusPressed |= RIGHT;
                    eventConsumed = true;
                    if (moveFocus(RIGHT))
                        break;
                    else
                        startScrolling(GAMEPAD_SCROLL_SPEED, 0);
                }

                if (!(_focusPressed & UP) &&
                    gamepad->isButtonDown(Gamepad::BUTTON_UP))
                {
                    _focusPressed |= UP;
                    eventConsumed = true;
                    if (moveFocus(UP))
                        break;
                    else
                        startScrolling(0, GAMEPAD_SCROLL_SPEED);
                }

                if (!(_focusPressed & LEFT) &&
                    gamepad->isButtonDown(Gamepad::BUTTON_LEFT))
                {
                    _focusPressed |= LEFT;
                    eventConsumed = true;
                    if (moveFocus(LEFT))
                        break;
                    else
                        startScrolling(-GAMEPAD_SCROLL_SPEED, 0);
                }
                break;
            }
            case Gamepad::JOYSTICK_EVENT:
            {
                switch (analogIndex)
                {
                case 0:
                    // The left analog stick can be used in the same way as the DPad.
                    eventConsumed = true;
                    if (!(_focusPressed & RIGHT) &&
                        joystick.x > JOYSTICK_THRESHOLD)
                    {
                        _focusPressed |= RIGHT;
                        if (moveFocus(RIGHT))
                            break;
                        else
                            startScrolling(GAMEPAD_SCROLL_SPEED * joystick.x, 0);
                    }

                    if (!(_focusPressed & DOWN) &&
                        joystick.y < -JOYSTICK_THRESHOLD)
                    {
                        _focusPressed |= DOWN;
                        if (moveFocus(DOWN))
                            break;
                        else
                            startScrolling(0, GAMEPAD_SCROLL_SPEED * joystick.y);
                    }

                    if (!(_focusPressed & LEFT) &&
                        joystick.x < -JOYSTICK_THRESHOLD)
                    {
                        _focusPressed |= LEFT;
                        if (moveFocus(LEFT))
                            break;
                        else
                            startScrolling(GAMEPAD_SCROLL_SPEED * joystick.x, 0);
                    }
                        
                    if (!(_focusPressed & UP) &&
                        joystick.y > JOYSTICK_THRESHOLD)
                    {
                        _focusPressed |= UP;
                        if (moveFocus(UP))
                            break;
                        else
                            startScrolling(0, GAMEPAD_SCROLL_SPEED * joystick.y);
                    }
                    break;

                case 1:
                    // The right analog stick can be used to scroll.
                    if (_scroll != SCROLL_NONE)
                    {
                        if (_scrolling)
                        {
                            if (joystick.isZero())
                            {
                                stopScrolling();
                            }
                            else
                            {
                                startScrolling(GAMEPAD_SCROLL_SPEED * joystick.x, GAMEPAD_SCROLL_SPEED * joystick.y, false);
                            }
                        }
                        else
                        {
                            startScrolling(GAMEPAD_SCROLL_SPEED * joystick.x, GAMEPAD_SCROLL_SPEED * joystick.y);
                        }
                        release();
                        return _consumeInputEvents;
                    }
                    break;
                }
            }
        }
    }

    if ((evt == Gamepad::BUTTON_EVENT || evt == Gamepad::JOYSTICK_EVENT) &&
        analogIndex == 0)
    {
        if ((_focusPressed & DOWN) &&
            !gamepad->isButtonDown(Gamepad::BUTTON_DOWN) &&
            joystick.y > -JOYSTICK_THRESHOLD)
        {
            _focusPressed &= ~DOWN;
            eventConsumed = true;
        }

        if ((_focusPressed & RIGHT) &&
            !gamepad->isButtonDown(Gamepad::BUTTON_RIGHT) &&
            joystick.x < JOYSTICK_THRESHOLD)
        {
            _focusPressed &= ~RIGHT;
            eventConsumed = true;
        }
    
        if ((_focusPressed & UP) &&
            !gamepad->isButtonDown(Gamepad::BUTTON_UP) &&
            joystick.y < JOYSTICK_THRESHOLD)
        {
            _focusPressed &= ~UP;
            eventConsumed = true;
        }

        if ((_focusPressed & LEFT) &&
            !gamepad->isButtonDown(Gamepad::BUTTON_LEFT) &&
            joystick.x > -JOYSTICK_THRESHOLD)
        {
            _focusPressed &= ~LEFT;
            eventConsumed = true;
        }
    }

    if (!_focusPressed && _scrolling)
    {
        stopScrolling();
    }

    release();
    return eventConsumed;
}

bool Container::isContainer() const
{
    return true;
}

Layout::Type Container::getLayoutType(const char* layoutString)
{
    if (!layoutString)
    {
        return Layout::LAYOUT_ABSOLUTE;
    }

    std::string layoutName(layoutString);
    std::transform(layoutName.begin(), layoutName.end(), layoutName.begin(), (int(*)(int))toupper);
    if (layoutName == "LAYOUT_ABSOLUTE")
    {
        return Layout::LAYOUT_ABSOLUTE;
    }
    else if (layoutName == "LAYOUT_VERTICAL")
    {
        return Layout::LAYOUT_VERTICAL;
    }
    else if (layoutName == "LAYOUT_FLOW")
    {
        return Layout::LAYOUT_FLOW;
    }
    else
    {
        // Default.
        return Layout::LAYOUT_ABSOLUTE;
    }
}

void Container::updateScroll()
{
    if (!_initializedWithScroll)
    {
        _initializedWithScroll = true;
        _layout->update(this, _scrollPosition);
    }

    // Update time.
    if (!_lastFrameTime)
    {
        _lastFrameTime = Game::getGameTime();
    }
    double frameTime = Game::getGameTime();
    float elapsedTime = (float)(frameTime - _lastFrameTime);
    _lastFrameTime = frameTime;

    const Theme::Border& containerBorder = getBorder(_state);
    const Theme::Padding& containerPadding = getPadding();

    // Calculate total width and height.
    std::vector<Control*> controls = getControls();
    for (size_t i = 0, controlsCount = controls.size(); i < controlsCount; i++)
    {
        Control* control = controls.at(i);

        const Rectangle& bounds = control->getBounds();
        const Theme::Margin& margin = control->getMargin();

        float newWidth = bounds.x + bounds.width;
        if (newWidth > _totalWidth)
        {
            _totalWidth = newWidth;
        }

        float newHeight = bounds.y + bounds.height;
        if (newHeight > _totalHeight)
        {
            _totalHeight = newHeight;
        }
    }

    float vWidth = getImageRegion("verticalScrollBar", _state).width;
    float hHeight = getImageRegion("horizontalScrollBar", _state).height;
    float clipWidth = _bounds.width - containerBorder.left - containerBorder.right - containerPadding.left - containerPadding.right - vWidth;
    float clipHeight = _bounds.height - containerBorder.top - containerBorder.bottom - containerPadding.top - containerPadding.bottom - hHeight;

    // Apply and dampen inertia.
    if (!_scrollingVelocity.isZero())
    {
        // Calculate the time passed since last update.
        float elapsedSecs = (float)elapsedTime * 0.001f;

        _scrollPosition.x += _scrollingVelocity.x * elapsedSecs;
        _scrollPosition.y += _scrollingVelocity.y * elapsedSecs;

        if (!_scrolling)
        {
            float dampening = 1.0f - _scrollingFriction * SCROLL_FRICTION_FACTOR * elapsedSecs;
            _scrollingVelocity.x *= dampening;
            _scrollingVelocity.y *= dampening;

            if (fabs(_scrollingVelocity.x) < 100.0f)
                _scrollingVelocity.x = 0.0f;
            if (fabs(_scrollingVelocity.y) < 100.0f)
                _scrollingVelocity.y = 0.0f;
        }
    }

    // Stop scrolling when the far edge is reached.
    if (-_scrollPosition.x > _totalWidth - clipWidth)
    {
        _scrollPosition.x = -(_totalWidth - clipWidth);
        _scrollingVelocity.x = 0;
    }
    
    if (-_scrollPosition.y > _totalHeight - clipHeight)
    {
        _scrollPosition.y = -(_totalHeight - clipHeight);
        _scrollingVelocity.y = 0;
    }

    if (_scrollPosition.x > 0)
    {
        _scrollPosition.x = 0;
        _scrollingVelocity.x = 0;
    }

    if (_scrollPosition.y > 0)
    {
        _scrollPosition.y = 0;
        _scrollingVelocity.y = 0;
    }

    float scrollWidth = 0;
    if (clipWidth < _totalWidth)
        scrollWidth = (clipWidth / _totalWidth) * clipWidth;

    float scrollHeight = 0;
    if (clipHeight < _totalHeight)
        scrollHeight = (clipHeight / _totalHeight) * clipHeight;

    _scrollBarBounds.set(((-_scrollPosition.x) / _totalWidth) * clipWidth,
                         ((-_scrollPosition.y) / _totalHeight) * clipHeight,
                         scrollWidth, scrollHeight);

    // If scroll velocity is 0 and scrollbars are not always visible, trigger fade-out animation.
    if (!_scrolling && _scrollingVelocity.isZero() && _scrollBarsAutoHide && _scrollBarOpacity == 1.0f)
    {
        float to = 0;
        _scrollBarOpacity = 0.99f;
        if (!_scrollBarOpacityClip)
        {
            Animation* animation = createAnimationFromTo("scrollbar-fade-out", ANIMATE_SCROLLBAR_OPACITY, &_scrollBarOpacity, &to, Curve::QUADRATIC_IN_OUT, 500L);
            _scrollBarOpacityClip = animation->getClip();
        }
        _scrollBarOpacityClip->play();
    }

    // Position controls within scroll area.
    _layout->update(this, _scrollPosition);
}

void Container::sortControls()
{
    if (_layout->getType() == Layout::LAYOUT_ABSOLUTE)
    {
        std::sort(_controls.begin(), _controls.end(), &sortControlsByZOrder);
    }
}

bool Container::touchEventScroll(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch(evt)
    {
    case Touch::TOUCH_PRESS:
        if (_contactIndex == INVALID_CONTACT_INDEX)
        {
            _contactIndex = (int) contactIndex;
            _contactIndices++;
            _scrollingLastX = _scrollingFirstX = _scrollingVeryFirstX = x;
            _scrollingLastY = _scrollingFirstY = _scrollingVeryFirstY = y;
            _scrollingVelocity.set(0, 0);
            _scrolling = true;
            _scrollingStartTimeX = _scrollingStartTimeY = 0;

            if (_scrollBarOpacityClip && _scrollBarOpacityClip->isPlaying())
            {
                _scrollBarOpacityClip->stop();
                _scrollBarOpacityClip = NULL;
            }
            _scrollBarOpacity = 1.0f;
            _dirty = true;
            return _consumeInputEvents;
        }
        break;
    case Touch::TOUCH_MOVE:
        if (_scrolling && _contactIndex == (int) contactIndex)
        {
            double gameTime = Game::getAbsoluteTime();

            // Calculate the latest movement delta for the next update to use.
            int vx = x - _scrollingLastX;
            int vy = y - _scrollingLastY;
            if (_scrollingMouseVertically)
            {
                float yRatio = _totalHeight / _absoluteBounds.height;
                vy *= yRatio;

                _scrollingVelocity.set(0, -vy);
                _scrollPosition.y -= vy;
            }
            else if (_scrollingMouseHorizontally)
            {
                float xRatio = _totalWidth / _absoluteBounds.width;
                vx *= xRatio;

                _scrollingVelocity.set(-vx, 0);
                _scrollPosition.x -= vx;
            }
            else
            {
                _scrollingVelocity.set(vx, vy);
                _scrollPosition.x += vx;
                _scrollPosition.y += vy;
            }

            _scrollingLastX = x;
            _scrollingLastY = y;

            // If the user changes direction, reset the start time and position.
            bool goingRight = (vx > 0);
            if (goingRight != _scrollingRight)
            {
                _scrollingFirstX = x;
                _scrollingRight = goingRight;
                _scrollingStartTimeX = gameTime;
            }

            bool goingDown = (vy > 0);
            if (goingDown != _scrollingDown)
            {
                _scrollingFirstY = y;
                _scrollingDown = goingDown;
                _scrollingStartTimeY = gameTime;
            }

            if (!_scrollingStartTimeX)
                _scrollingStartTimeX = gameTime;

            if (!_scrollingStartTimeY)
                _scrollingStartTimeY = gameTime;

            _scrollingLastTime = gameTime;
            _dirty = true;
            return _consumeInputEvents;
        }
        break;

    case Touch::TOUCH_RELEASE:
        if (_contactIndex == (int) contactIndex)
        {
            _contactIndex = INVALID_CONTACT_INDEX;
            _contactIndices--;
            _scrolling = false;
            double gameTime = Game::getAbsoluteTime();
            float timeSinceLastMove = (float)(gameTime - _scrollingLastTime);
            if (timeSinceLastMove > SCROLL_INERTIA_DELAY)
            {
                _scrollingVelocity.set(0, 0);
                _scrollingMouseVertically = _scrollingMouseHorizontally = false;
                _dirty = true;
                return _consumeInputEvents;
            }

            int dx = _scrollingLastX - _scrollingFirstX;
            int dy = _scrollingLastY - _scrollingFirstY;

            float timeTakenX = (float)(gameTime - _scrollingStartTimeX);
            float elapsedSecsX = timeTakenX * 0.001f;
            float timeTakenY = (float)(gameTime - _scrollingStartTimeY);
            float elapsedSecsY = timeTakenY * 0.001f;

            float vx = dx;
            float vy = dy;
            if (elapsedSecsX > 0)
                vx = (float)dx / elapsedSecsX;
            if (elapsedSecsY > 0)
                vy = (float)dy / elapsedSecsY;

            if (_scrollingMouseVertically)
            {
                float yRatio = _totalHeight / _absoluteBounds.height;
                vy *= yRatio;
                _scrollingVelocity.set(0, -vy);
            }
            else if (_scrollingMouseHorizontally)
            {
                float xRatio = _totalWidth / _absoluteBounds.width;
                vx *= xRatio;
                _scrollingVelocity.set(-vx, 0);
            }
            else
            {
                _scrollingVelocity.set(vx, vy);
            }

            _scrollingMouseVertically = _scrollingMouseHorizontally = false;
            _dirty = true;
            return _consumeInputEvents;
        }
        break;
    }

    return false;
}

bool Container::mouseEventScroll(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch(evt)
    {
        case Mouse::MOUSE_PRESS_LEFT_BUTTON:
        {
            if (_scrollBarVertical)
            {
                float vWidth = _scrollBarVertical->getRegion().width;
                Rectangle vBounds(_viewportBounds.x + _viewportBounds.width - vWidth,
                                 _scrollBarBounds.y,
                                 vWidth, _scrollBarBounds.height);

                if (x + _viewportBounds.x >= vBounds.x &&
                    x + _viewportBounds.x <= vBounds.x + vBounds.width)
                {
                    // Then we're within the horizontal bounds of the vertical scrollbar.
                    // We want to either jump up or down, or drag the scrollbar itself.
                    if (y < vBounds.y)
                    {
                        _scrollPosition.y += _totalHeight / 5.0f;
                    }
                    else if (y > vBounds.y + vBounds.height)
                    {
                        _scrollPosition.y -= _totalHeight / 5.0f;
                    }
                    else
                    {
                        _scrollingMouseVertically = true;
                    }
                }
            }
            
            if (_scrollBarHorizontal)
            {
                float hHeight = _scrollBarHorizontal->getRegion().height;
                Rectangle hBounds(_scrollBarBounds.x,
                                  _viewportBounds.y + _viewportBounds.height - hHeight,
                                  _scrollBarBounds.width, hHeight);
            
                if (y + _viewportBounds.y >= hBounds.y &&
                         y + _viewportBounds.y <= hBounds.y + hBounds.height)
                {
                    // We're within the vertical bounds of the horizontal scrollbar.
                    if (x < hBounds.x)
                        _scrollPosition.x += _totalWidth / 5.0f;
                    else if (x > hBounds.x + hBounds.width)
                        _scrollPosition.x -= _totalWidth / 5.0f;
                    else
                        _scrollingMouseHorizontally = true;
                }
            }

            return touchEventScroll(Touch::TOUCH_PRESS, x, y, 0);
        }

        case Mouse::MOUSE_MOVE:
            return touchEventScroll(Touch::TOUCH_MOVE, x, y, 0);

        case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
            return touchEventScroll(Touch::TOUCH_RELEASE, x, y, 0);

        case Mouse::MOUSE_WHEEL:
            if ((_state == HOVER && (!_scrollWheelRequiresFocus || _previousState == FOCUS)) ||
                _state == FOCUS && _scrollWheelRequiresFocus)
            {
                if (_scrollingVelocity.isZero())
                {
                    _lastFrameTime = Game::getGameTime();
                }
                _scrolling = _scrollingMouseVertically = _scrollingMouseHorizontally = false;

                _scrollingVelocity.y += _scrollWheelSpeed * wheelDelta;

                if (_scrollBarOpacityClip && _scrollBarOpacityClip->isPlaying())
                {
                    _scrollBarOpacityClip->stop();
                    _scrollBarOpacityClip = NULL;
                }
                _scrollBarOpacity = 1.0f;
                _dirty = true;
                return _consumeInputEvents;
            }
            break;
    }

    return false;
}

bool Container::pointerEvent(bool mouse, char evt, int x, int y, int data)
{
    if (!isEnabled() || !isVisible())
    {
        return false;
    }

    // This event may run untrusted code by notifying listeners of events.
    // If the user calls exit() or otherwise releases this container, we
    // need to keep it alive until the method returns.
    addRef();

    bool eventConsumed = false;
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding& padding = getPadding();
    float xPos = border.left + padding.left;
    float yPos = border.top + padding.top;

    Vector2* offset = NULL;
    if (_scroll != SCROLL_NONE)
    {
        offset = &_scrollPosition;
    }

    std::vector<Control*>::const_iterator it;
    for (it = _controls.begin(); it < _controls.end(); it++)
    {
        Control* control = *it;
        GP_ASSERT(control);
        if (!control->isEnabled() || !control->isVisible())
        {
            continue;
        }

        const Rectangle& bounds = control->getBounds();
        float boundsX = bounds.x;
        float boundsY = bounds.y;
        if (offset)
        {
            boundsX += offset->x;
            boundsY += offset->y;
        }

        Control::State currentState = control->getState();
        if ((currentState != Control::NORMAL) ||
            ((evt == Touch::TOUCH_PRESS ||
              evt == Mouse::MOUSE_PRESS_LEFT_BUTTON ||
              evt == Mouse::MOUSE_PRESS_MIDDLE_BUTTON ||
              evt == Mouse::MOUSE_PRESS_RIGHT_BUTTON ||
              evt == Mouse::MOUSE_MOVE ||
              evt == Mouse::MOUSE_WHEEL) &&
                x >= xPos + boundsX &&
                x <= xPos + boundsX + bounds.width &&
                y >= yPos + boundsY &&
                y <= yPos + boundsY + bounds.height))
        {
            // Pass on the event's clip relative to the control.
            if (mouse)
                eventConsumed |= control->mouseEvent((Mouse::MouseEvent)evt, x - xPos - boundsX, y - yPos - boundsY, data);
            else
                eventConsumed |= control->touchEvent((Touch::TouchEvent)evt, x - xPos - boundsX, y - yPos - boundsY, (unsigned int)data);
        }
    }

    if (!isEnabled() || !isVisible())
    {
        _contactIndex = INVALID_CONTACT_INDEX;
        _contactIndices = 0;
        _scrolling = false;
        _scrollingMouseVertically = _scrollingMouseHorizontally = false;

        release();
        return (_consumeInputEvents | eventConsumed);
    }

    bool withinClipBounds = (x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                             y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height);
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (withinClipBounds)
        {
            setState(Control::ACTIVE);
        }
        else if (_contactIndices == 0)
        {
            setState(Control::NORMAL);
            _contactIndex = INVALID_CONTACT_INDEX;
            release();
            return false;
        }
        _contactIndices++;
        break;
    case Mouse::MOUSE_MOVE:
        if (_state != ACTIVE)
        {
            if (_state != HOVER && withinClipBounds)
            {
                _previousState = _state;
                setState(HOVER);
                notifyListeners(Control::Listener::ENTER);
            }
            else if (_state == HOVER && !withinClipBounds)
            {
                setState(_previousState);
                notifyListeners(Control::Listener::LEAVE);
            }
            else if (_state != HOVER)
            {
                release();
                return false;
            }
        }
        break;
    case Mouse::MOUSE_WHEEL:
        if (!withinClipBounds && !_scrollWheelRequiresFocus)
        {
            release();
            return false;
        }
        break;
    case Touch::TOUCH_RELEASE:
        if (_contactIndices > 0)
            _contactIndices--;

        if (!_contactIndices)
        {
			if (_state == ACTIVE && withinClipBounds)
			{
				setState(FOCUS);
			}
			else
			{
				setState(NORMAL);
			}
        }
        break;
    }

    if (!eventConsumed && _scroll != SCROLL_NONE &&
        (evt != Touch::TOUCH_PRESS || withinClipBounds))
    {
        if ((mouse && mouseEventScroll((Mouse::MouseEvent)evt, x - xPos, y - yPos, data)) ||
            (!mouse && touchEventScroll((Touch::TouchEvent)evt, x - xPos, y - yPos, (unsigned int)data)))
        {
            eventConsumed = true;
        }
    }

    release();
    return (_consumeInputEvents | eventConsumed);
}

Container::Scroll Container::getScroll(const char* scroll)
{
    if (!scroll)
        return Container::SCROLL_NONE;

    if (strcmp(scroll, "SCROLL_NONE") == 0)
    {
        return Container::SCROLL_NONE;
    }
    else if (strcmp(scroll, "SCROLL_HORIZONTAL") == 0)
    {
        return Container::SCROLL_HORIZONTAL;
    }
    else if (strcmp(scroll, "SCROLL_VERTICAL") == 0)
    {
        return Container::SCROLL_VERTICAL;
    }
    else if (strcmp(scroll, "SCROLL_BOTH") == 0)
    {
        return Container::SCROLL_BOTH;
    }
    else
    {
        GP_ERROR("Failed to get corresponding scroll state for unsupported value '%s'.", scroll);
    }

    return Container::SCROLL_NONE;
}

float Container::getScrollingFriction() const
{
    return _scrollingFriction;
}

void Container::setScrollingFriction(float friction)
{
    _scrollingFriction = friction;
}

float Container::getScrollWheelSpeed() const
{
    return _scrollWheelSpeed;
}

void Container::setScrollWheelSpeed(float speed)
{
    _scrollWheelSpeed = speed;
}

static bool sortControlsByZOrder(Control* c1, Control* c2)
{
    if (c1->getZIndex() < c2->getZIndex())
        return true;

    return false;
}

unsigned int Container::getAnimationPropertyComponentCount(int propertyId) const
{
    switch(propertyId)
    {
    case ANIMATE_SCROLLBAR_OPACITY:
        return 1;
    default:
        return Control::getAnimationPropertyComponentCount(propertyId);
    }
}

void Container::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    GP_ASSERT(value);

    switch(propertyId)
    {
    case ANIMATE_SCROLLBAR_OPACITY:
        value->setFloat(0, _scrollBarOpacity);
        break;
    default:
        Control::getAnimationPropertyValue(propertyId, value);
        break;
    }
}

void Container::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    GP_ASSERT(value);

    switch(propertyId)
    {
    case ANIMATE_SCROLLBAR_OPACITY:
        _scrollBarOpacity = Curve::lerp(blendWeight, _opacity, value->getFloat(0));
        _dirty = true;
        break;
    default:
        Control::setAnimationPropertyValue(propertyId, value, blendWeight);
        break;
    }
}

}
