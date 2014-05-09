#include "Base.h"
#include "Form.h"
#include "AbsoluteLayout.h"
#include "FlowLayout.h"
#include "VerticalLayout.h"
#include "Game.h"
#include "Theme.h"
#include "Label.h"
#include "Button.h"
#include "CheckBox.h"
#include "Scene.h"

// Scroll speed when using a DPad -- max scroll speed when using a joystick.
static const float GAMEPAD_SCROLL_SPEED = 500.0f;
// Distance a joystick must be pushed in order to trigger focus-change and/or scrolling.
static const float JOYSTICK_THRESHOLD = 0.75f;
// If the DPad or joystick is held down, this is the initial delay in milliseconds between focus changes.
static const float GAMEPAD_FOCUS_REPEAT_DELAY = 300.0f;

// Shaders used for drawing offscreen quad when form is attached to a node
#define FORM_VSH "res/shaders/sprite.vert"
#define FORM_FSH "res/shaders/sprite.frag"

namespace gameplay
{

static std::vector<Form*> __forms;
static Control* __focusControl = NULL;
static Control* __activeControl[Touch::MAX_TOUCH_POINTS];
static bool __shiftKeyDown = false;

/**
 * Static initializer for forms.
 * @script{ignore}
 */
struct FormInit
{
    FormInit()
    {
        memset(__activeControl, 0, sizeof(__activeControl));
    }
};
static FormInit __init;

Form::Form() : _node(NULL), _batched(true)
{
}

Form::~Form()
{
    // Remove this Form from the global list.
    std::vector<Form*>::iterator it = std::find(__forms.begin(), __forms.end(), this);
    if (it != __forms.end())
    {
        __forms.erase(it);
    }
}

Form* Form::create(const char* url)
{
    Form* form = new Form();

    // Load Form from .form file.
    Properties* properties = Properties::create(url);
    if (!properties)
    {
        GP_WARN("Failed to load properties file for Form.");
        return NULL;
    }
    // Check if the Properties is valid and has a valid namespace.
    Properties* formProperties = (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
    if (!formProperties || !(strcmpnocase(formProperties->getNamespace(), "form") == 0))
    {
        GP_WARN("Invalid properties file for form: %s", url);
        SAFE_DELETE(properties);
        return NULL;
    }

    // Load the form's theme style.
    Theme* theme = NULL;
    Theme::Style* style = NULL;
    if (formProperties->exists("theme"))
    {
        std::string themeFile;
        if (formProperties->getPath("theme", &themeFile))
        {
            theme = Theme::create(themeFile.c_str());
        }
    }
    if (!theme)
    {
        theme = Theme::getDefault();
    }

    if (theme)
    {
        // Load the form's style
        const char* styleName = formProperties->getString("style", "Form");
        style = theme->getStyle(styleName);
        if (!style)
            style = theme->getEmptyStyle();
    }

    form->_batched = formProperties->getBool("batchingEnabled", true);

    // Initialize the form and all of its child controls
    form->initialize("Form", style, formProperties);

    // Release the theme: its lifetime is controlled by addRef() and release() calls in initialize (above) and ~Control.
    if (theme != Theme::getDefault())
    {
        SAFE_RELEASE(theme);
    }

    SAFE_DELETE(properties);

    return form;
}

Form* Form::create(const char* id, Theme::Style* style, Layout::Type layoutType)
{
	Form* form = new Form();
	form->_id = id ? id : "";
	form->_layout = createLayout(layoutType);
	form->initialize("Form", style, NULL);
	return form;
}

void Form::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Container::initialize(typeName, style, properties);

    __forms.push_back(this);

    // After creation, update our bounds once so code that runs immediately after form
    // creation has access to up-to-date bounds.
    if (updateBoundsInternal(Vector2::zero()))
        updateBoundsInternal(Vector2::zero());
}

Form* Form::getForm(const char* id)
{
    for (size_t i = 0, size = __forms.size(); i < size; ++i)
    {
        Form* f = __forms[i];
        GP_ASSERT(f);
        if (strcmp(id, f->getId()) == 0)
        {
            return f;
        }
    }
    return NULL;
}

Control* Form::getActiveControl(unsigned int touchPoint)
{
    if (touchPoint >= Touch::MAX_TOUCH_POINTS)
        return NULL;

    return __activeControl[touchPoint];
}

Control* Form::getFocusControl()
{
    return __focusControl;
}

void Form::clearFocus()
{
    setFocusControl(NULL);
}

bool Form::isForm() const
{
    return true;
}

void Form::setNode(Node* node)
{
    if (_node != node)
    {
        _node = node;
    }
}

static unsigned int nextPowerOfTwo(unsigned int v)
{
    if (!((v & (v - 1)) == 0))
    {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        return v + 1;
    }

    return v;
}

void Form::update(float elapsedTime)
{
    Container::update(elapsedTime);

    // Do a two-pass bounds update:
    //  1. First pass updates leaf controls
    //  2. Second pass updates parent controls that depend on child sizes
    if (updateBoundsInternal(Vector2::zero()))
        updateBoundsInternal(Vector2::zero());
}

void Form::startBatch(SpriteBatch* batch)
{
    // TODO (note: might want to pass a level number/depth here so that batch draw calls can be sorted correctly, such as all text on top)
    if (!batch->isStarted())
    {
        batch->setProjectionMatrix(_projectionMatrix);
        batch->start();

        if (_batched)
            _batches.push_back(batch);
    }
}

void Form::finishBatch(SpriteBatch* batch)
{
    if (!_batched)
    {
        batch->finish();
    }
}

const Matrix& Form::getProjectionMatrix() const
{
    return  _projectionMatrix;
}

unsigned int Form::draw()
{
    if (!_visible || _absoluteClipBounds.width == 0 || _absoluteClipBounds.height == 0)
        return 0;

    Game* game = Game::getInstance();
    Rectangle viewport = game->getViewport();

    // If we're drawing in 2D (i.e. not attached to a node), we need to clear the depth buffer
    if (_node)
    {
        // Drawing in 3D.
        // Setup a projection matrix for drawing the form via the node's world transform.
        Matrix world(_node->getWorldMatrix());
        world.scale(1, -1, 1);
        world.translate(0, -_absoluteClipBounds.height, 0);
        Matrix::multiply(_node->getViewProjectionMatrix(), world, &_projectionMatrix);
    }
    else
    {
        // Drawing in 2D, so we need to clear the depth buffer
        Game::getInstance()->clear(Game::CLEAR_DEPTH, Vector4::zero(), 1, 0);

        // Setup an ortho matrix that maps to the current viewport
        const Rectangle& viewport = Game::getInstance()->getViewport();
        Matrix::createOrthographicOffCenter(0, viewport.width, viewport.height, 0, 0, 1, &_projectionMatrix);
    }

    // Draw the form
    unsigned int drawCalls = Container::draw(this, _absoluteClipBounds);

    // Flush all batches that were queued during drawing and then empty the batch list
    if (_batched)
    {
        unsigned int batchCount = _batches.size();
        for (unsigned int i = 0; i < batchCount; ++i)
            _batches[i]->finish();
        _batches.clear();
        drawCalls = batchCount;
    }

    return drawCalls;
}

const char* Form::getType() const
{
    return "form";
}


bool Form::isBatchingEnabled() const
{
    return _batched;
}

void Form::setBatchingEnabled(bool enabled)
{
    _batched = enabled;
}

void Form::updateInternal(float elapsedTime)
{
    pollGamepads();

    for (size_t i = 0, size = __forms.size(); i < size; ++i)
    {
        Form* form = __forms[i];

        if (form && form->isEnabled() && form->isVisible())
        {
            form->update(elapsedTime);
        }
    }
}

bool Form::screenToForm(Control* ctrl, int* x, int* y)
{
    Form* form = ctrl->getTopLevelForm();
    if (form)
    {
        if (form->_node)
        {
            // Form is attached to a scene node, so project the screen space point into the
            // form's coordinate space (which may be transformed by the node).
            Vector3 point;
            if (form->projectPoint(*x, *y, &point))
            {
                *x = (int)point.x;
                *y = form->_absoluteBounds.height - (int)point.y;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

Control* Form::findInputControl(int* x, int* y, bool focus, unsigned int contactIndex)
{
    for (int i = (int)__forms.size() - 1; i >= 0; --i)
    {
        Form* form = __forms[i];
        if (!form || !form->isEnabled() || !form->isVisible())
            continue;

        // Convert to local form coordinates
        int formX = *x;
        int formY = *y;
        if (!screenToForm(form, &formX, &formY))
            continue;

        // Search for an input control within this form
        Control* ctrl = findInputControl(form, formX, formY, focus, contactIndex);
        if (ctrl)
        {
            *x = formX;
            *y = formY;
            return ctrl;
        }

        // If the form consumes input events and the point intersects the form,
        // don't traverse other forms below it.
        if (form->_consumeInputEvents && form->_absoluteClipBounds.contains(formX, formY))
            return NULL;
    }

    return NULL;
}

Control* Form::findInputControl(Control* control, int x, int y, bool focus, unsigned int contactIndex)
{
    if (!(control->_visible && control->isEnabled()))
        return NULL;

    Control* result = NULL;

    // Does the passed in control's bounds intersect the specified coordinates - and 
    // does the control support the specified input state?
    if (control->_consumeInputEvents && (!focus || control->canFocus()))
    {
        if (control->_absoluteClipBounds.contains(x, y))
            result = control;
    }

    // If the control has children, search for an input control inside it that also
    // supports the above conditions.
    if (control->isContainer())
    {
        Container* container = static_cast<Container*>(control);
        for (unsigned int i = 0, childCount = container->getControlCount(); i < childCount; ++i)
        {
            Control* ctrl = findInputControl(container->getControl(i), x, y, focus, contactIndex);
            if (ctrl)
                result = ctrl;
        }
    }

    return result;
}

Control* Form::handlePointerPressRelease(int* x, int* y, bool pressed, unsigned int contactIndex)
{
    if (contactIndex >= Touch::MAX_TOUCH_POINTS)
        return NULL;

    Control* ctrl = NULL;

    int newX = *x;
    int newY = *y;

    if (pressed)
    {
        // Update active state changes
        if ((ctrl = findInputControl(&newX, &newY, false, contactIndex)) != NULL)
        {
            if (__activeControl[contactIndex] != ctrl || ctrl->_state != ACTIVE)
            {
                if (__activeControl[contactIndex])
                {
                    __activeControl[contactIndex]->_state = NORMAL;
                    __activeControl[contactIndex]->setDirty(DIRTY_STATE);
                }

                __activeControl[contactIndex] = ctrl;
                ctrl->_state = ACTIVE;
                ctrl->setDirty(DIRTY_STATE);
            }

            ctrl->notifyListeners(Control::Listener::PRESS);
        }
    }
    else // !pressed
    {
        Control* active = (__activeControl[contactIndex] && __activeControl[contactIndex]->_state == ACTIVE) ? __activeControl[contactIndex] : NULL;

        if (active)
        {
            active->addRef(); // protect against event-hanlder evil

            // Release happened for an active control (that was pressed)
            ctrl = active;

            // Transform point to form-space
            screenToForm(ctrl, &newX, &newY);

            // No longer any active control
            active->setDirty(DIRTY_STATE);
            active->_state = NORMAL;
            __activeControl[contactIndex] = NULL;
        }
        else
        {
            // Update active and hover control state on release
            Control* inputControl = findInputControl(&newX, &newY, false, contactIndex);
            if (inputControl)
            {
                ctrl = inputControl;

                if (__activeControl[contactIndex] != ctrl || ctrl->_state != HOVER)
                {
                    if (__activeControl[contactIndex])
                    {
                        __activeControl[contactIndex]->_state = NORMAL;
                        __activeControl[contactIndex]->setDirty(Control::DIRTY_STATE);
                    }

                    __activeControl[contactIndex] = ctrl;
                    ctrl->_state = HOVER;
                    ctrl->setDirty(DIRTY_STATE);
                }
            }
            else
            {
                // Control no longer active
                if (__activeControl[contactIndex])
                {
                    __activeControl[contactIndex]->setDirty(Control::DIRTY_STATE);
                    __activeControl[contactIndex]->_state = NORMAL;
                    __activeControl[contactIndex] = NULL;
                }
            }
        }

        if (active)
        {
            // Fire release event for the previously active control
            active->notifyListeners(Control::Listener::RELEASE);

            // If the release event was received on the same control that was
            // originally pressed, fire a click event
            if (active->_absoluteClipBounds.contains(newX, newY))
            {
                if (!active->_parent || !active->_parent->isScrolling())
                {
                    active->notifyListeners(Control::Listener::CLICK);
                }
            }

            active->release();
        }
    }

    *x = newX;
    *y = newY;

    return ctrl;
}

Control* Form::handlePointerMove(int* x, int* y, unsigned int contactIndex)
{
    if (contactIndex >= Touch::MAX_TOUCH_POINTS)
        return NULL;

    Control* ctrl = NULL;

    // Handle hover control changes on move, only if there is no currently active control
    // (i.e. when the mouse or a finger is not down).
    if (__activeControl[contactIndex] && __activeControl[contactIndex]->_state == ACTIVE)
    {
        // Active controls always continue receiving pointer events, even when the pointer
        // is not on top of the control.
        ctrl = __activeControl[contactIndex];
        screenToForm(ctrl, x, y);
    }
    else
    {
        ctrl = findInputControl(x, y, false, contactIndex);
        if (ctrl)
        {
            // Update hover control
            if (__activeControl[contactIndex] != ctrl || ctrl->_state != HOVER)
            {
                if (__activeControl[contactIndex])
                {
                    __activeControl[contactIndex]->_state = NORMAL;
                    __activeControl[contactIndex]->setDirty(DIRTY_STATE);
                }

                __activeControl[contactIndex] = ctrl;
                ctrl->_state = HOVER;
                ctrl->setDirty(DIRTY_STATE);
            }
        }
        else
        {
            // No active/hover control
            if (__activeControl[contactIndex])
            {
                __activeControl[contactIndex]->setDirty(DIRTY_STATE);
                __activeControl[contactIndex]->_state = NORMAL;
                __activeControl[contactIndex] = NULL;
            }
        }
    }

    return ctrl;
}

void Form::verifyRemovedControlState(Control* control)
{
    if (__focusControl == control)
    {
        __focusControl = NULL;
    }

    if (control->_state == ACTIVE || control->_state == HOVER)
    {
        for (unsigned int i = 0; i < Touch::MAX_TOUCH_POINTS; ++i)
        {
            if (__activeControl[i] == control)
            {
                __activeControl[i] = NULL;
            }
        }
        control->_state = NORMAL;
    }
}

// Generic pointer event handler that both touch and mouse events map to.
// Mappings:
//   mouse - true for mouse events, false for touch events
//   evt - Mouse::MouseEvent or Touch::TouchEvent
//   x, y - Point of event
//   param - wheelData for mouse events, contactIndex for touch events
bool Form::pointerEventInternal(bool mouse, int evt, int x, int y, int param)
{
    // Do not process mouse input when mouse is captured
    if (mouse && Game::getInstance()->isMouseCaptured())
        return false;

    // Is this a press event (TOUCH_PRESS has the same value as MOUSE_PRESS_LEFT_BUTTON)
    bool pressEvent = evt == Touch::TOUCH_PRESS || (mouse && (evt == Mouse::MOUSE_PRESS_MIDDLE_BUTTON || evt == Mouse::MOUSE_PRESS_RIGHT_BUTTON));

    Control* ctrl = NULL;
    int formX = x;
    int formY = y;
    unsigned int contactIndex = mouse ? 0 : param;

    // Note: TOUCH_PRESS and TOUCH_RELEASE have same values as MOUSE_PRESS_LEFT_BUTTON and MOUSE_RELEASE_LEFT_BUTTON
    if (evt == Touch::TOUCH_PRESS)
    {
        ctrl = handlePointerPressRelease(&formX, &formY, true, contactIndex);
    }
    else if (evt == Touch::TOUCH_RELEASE)
    {
        ctrl = handlePointerPressRelease(&formX, &formY, false, contactIndex);
    }
    else if ((mouse && evt == Mouse::MOUSE_MOVE) || (!mouse && evt == Touch::TOUCH_MOVE))
    {
        ctrl = handlePointerMove(&formX, &formY, contactIndex);
    }

    // Dispatch input events to all controls that intersect this point
    if (ctrl == NULL)
    {
        formX = x;
        formY = y;
        ctrl = findInputControl(&formX, &formY, false, contactIndex);
    }

    if (ctrl)
    {
        // Handle setting focus for all press events
        if (pressEvent)
        {
            Control* focusControl = ctrl;
            while (focusControl && !focusControl->setFocus())
                focusControl = focusControl->_parent;

            if (focusControl == NULL)
            {
                // Nothing got focus on this press, so remove current focused control
                setFocusControl(NULL);
            }
        }

        // Dispatch the event from the bottom upwards, until a control intersecting the point consumes the event
        while (ctrl)
        {
            int localX = formX - ctrl->_absoluteBounds.x;
            int localY = formY - ctrl->_absoluteBounds.y;
            if (mouse)
            {
                if (ctrl->mouseEvent((Mouse::MouseEvent)evt, localX, localY, param))
                    return true;

                // Forward to touch event hanlder if unhandled by mouse handler
                switch (evt)
                {
                case Mouse::MOUSE_PRESS_LEFT_BUTTON:
                    if (ctrl->touchEvent(Touch::TOUCH_PRESS, localX, localY, 0))
                        return true;
                    break;
                case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
                    if (ctrl->touchEvent(Touch::TOUCH_RELEASE, localX, localY, 0))
                        return true;
                    break;
                case Mouse::MOUSE_MOVE:
                    if (ctrl->touchEvent(Touch::TOUCH_MOVE, localX, localY, 0))
                        return true;
                    break;
                }
            }
            else
            {
                if (ctrl->touchEvent((Touch::TouchEvent)evt, localX, localY, contactIndex))
                    return true;
            }

            // Handle container scrolling
            Control* tmp = ctrl;
            while (tmp)
            {
                if (tmp->isContainer())
                {
                    Container* container = static_cast<Container*>(tmp);
                    if (container->_scroll != SCROLL_NONE)
                    {
                        if (mouse)
                        {
                            if (container->mouseEventScroll((Mouse::MouseEvent)evt, formX - tmp->_absoluteBounds.x, formY - tmp->_absoluteBounds.y, param))
                                return true;
                        }
                        else
                        {
                            if (container->touchEventScroll((Touch::TouchEvent)evt, formX - tmp->_absoluteBounds.x, formY - tmp->_absoluteBounds.y, param))
                                return true;
                        }
                        break; // scrollable parent container found
                    }
                }
                tmp = tmp->_parent;
            }

            // Consume all input events anyways?
            if (ctrl->getConsumeInputEvents())
                return true;

            ctrl = ctrl->getParent();
        }
    }
    else
    {
        // If this was a press event, remove all focus
        if (pressEvent)
        {
            setFocusControl(NULL);
        }
    }

    return false;
}

bool Form::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    return pointerEventInternal(false, evt, x, y, (int)contactIndex);
}

bool Form::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return pointerEventInternal(true, evt, x, y, wheelDelta);
}

bool Form::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    switch (key)
    {
    case Keyboard::KEY_ESCAPE:
        return false; // ignore escape key presses

    case Keyboard::KEY_SHIFT:
        if (evt == Keyboard::KEY_PRESS)
            __shiftKeyDown = true;
        else if (evt == Keyboard::KEY_RELEASE)
            __shiftKeyDown = false;
        break;
    }
    if (key == Keyboard::KEY_ESCAPE)
        return false;

    // Handle focus changing
    if (__focusControl)
    {
        switch (evt)
        {
        case Keyboard::KEY_CHAR:
            switch (key)
            {
            case Keyboard::KEY_TAB:
                if (__focusControl->_parent)
                {
                    if (__focusControl->_parent->moveFocus(__shiftKeyDown ? Container::PREVIOUS : Container::NEXT))
                        return true;
                }
                break;
            }
            break;
        }
    }

    // Dispatch key events
    Control* ctrl = __focusControl;
    while (ctrl)
    {
        if (ctrl->isEnabled() && ctrl->isVisible())
        {
            if (ctrl->keyEvent(evt, key))
                return true;
        }

        ctrl = ctrl->getParent();
    }

    return false;
}

void Form::pollGamepads()
{
    Game* game = Game::getInstance();

    // If no gamepads are connected, return immediately
    unsigned int gamepadCount = game->getGamepadCount();
    if (gamepadCount == 0)
        return;

    // For now, always use gamepad zero for controlling the UI.
    // Possibly allow the developer to set the active gamepad for UI later.
    Gamepad* gamepad = game->getGamepad(0, true);
    if (!gamepad)
        return;

    pollGamepad(gamepad);
}

bool Form::pollGamepad(Gamepad* gamepad)
{
    // Get the currently focused control's container for focus management and scrolling
    if (!__focusControl)
        return false;

    // Get parent container
    Container* parentContainer = NULL;
    if (__focusControl->_parent)
        parentContainer = __focusControl->_parent;

    // Get scroll container
    Container* scrollContainer = NULL;
    if (__focusControl->isContainer())
    {
        scrollContainer = static_cast<Container*>(__focusControl);
        if (scrollContainer->_scroll == SCROLL_NONE)
            scrollContainer = NULL;
    }
    if (!scrollContainer && parentContainer && parentContainer->_scroll != SCROLL_NONE)
        scrollContainer = parentContainer;

    // Static static maintained across function calls
    static bool scrolling = false;
    static double lastFocusChangeTime = 0;

    bool focusPressed = false;
    bool stillScrolling = false;
    double currentTime = Game::getAbsoluteTime();
    double focusChangeElapsedTime = currentTime - lastFocusChangeTime;

    // Is a selection button down (i.e. buttons used for UI clicking/interactions)?
    bool selectButtonDown = gamepad->isButtonDown(Gamepad::BUTTON_A) || gamepad->isButtonDown(Gamepad::BUTTON_X);

    if (!selectButtonDown)
    {
        // Get values of analog joysticks 1 and 2 (assume left and right analog sticks)
        Vector2 joystick;
        unsigned int joystickCount = gamepad->getJoystickCount();
        gamepad->getJoystickValues(0, &joystick);

        if (parentContainer)
        {
            // The Dpad and left analog stick (i.e. first analog stick when there are two joysticks) controls focus
            if (gamepad->isButtonDown(Gamepad::BUTTON_UP) || (joystickCount > 1 && joystick.y > JOYSTICK_THRESHOLD))
            {
                focusPressed = true;
                if (focusChangeElapsedTime > GAMEPAD_FOCUS_REPEAT_DELAY && parentContainer->moveFocus(UP))
                {
                    lastFocusChangeTime = currentTime;
                }
            }

            if (gamepad->isButtonDown(Gamepad::BUTTON_DOWN) || (joystickCount > 1 && joystick.y < -JOYSTICK_THRESHOLD))
            {
                focusPressed = true;
                if (focusChangeElapsedTime > GAMEPAD_FOCUS_REPEAT_DELAY && parentContainer->moveFocus(DOWN))
                {
                    lastFocusChangeTime = currentTime;
                }
            }

            if (gamepad->isButtonDown(Gamepad::BUTTON_LEFT) || (joystickCount > 1 && joystick.x < -JOYSTICK_THRESHOLD))
            {
                focusPressed = true;
                if (focusChangeElapsedTime > GAMEPAD_FOCUS_REPEAT_DELAY && parentContainer->moveFocus(LEFT))
                {
                    lastFocusChangeTime = currentTime;
                }
            }

            if (gamepad->isButtonDown(Gamepad::BUTTON_RIGHT) || (joystickCount > 1 && joystick.x > JOYSTICK_THRESHOLD))
            {
                focusPressed = true;
                if (focusChangeElapsedTime > GAMEPAD_FOCUS_REPEAT_DELAY && parentContainer->moveFocus(RIGHT))
                {
                    lastFocusChangeTime = currentTime;
                }
            }
        }

        // The RIGHT analog stick (i.e. second), or ONLY analog stick (when only 1 joystick), is used to scroll.
        if (scrollContainer && joystickCount > 0)
        {
            if (joystickCount > 1)
                gamepad->getJoystickValues(1, &joystick);
            if (std::fabs(joystick.x) > JOYSTICK_THRESHOLD || std::fabs(joystick.y) > JOYSTICK_THRESHOLD)
            {
                scrollContainer->startScrolling(GAMEPAD_SCROLL_SPEED * joystick.x, GAMEPAD_SCROLL_SPEED * joystick.y, !scrolling);
                scrolling = stillScrolling = true;
            }
        }
    }

    if (!focusPressed)
    {
        // Reset focus repeat
        lastFocusChangeTime = 0;
    }

    if (scrolling && !stillScrolling)
    {
        scrolling = false;
        if (scrollContainer)
            scrollContainer->stopScrolling();
    }

    return focusPressed || scrolling;
}

bool Form::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    if (!__focusControl)
        return false;

    bool selectButtonPressed = gamepad->isButtonDown(Gamepad::BUTTON_A) || gamepad->isButtonDown(Gamepad::BUTTON_X);

    // Fire press, release and click events to focused controls
    switch (evt)
    {
    case Gamepad::BUTTON_EVENT:
        if (selectButtonPressed && __focusControl->_state != ACTIVE)
        {
            if (__activeControl[0])
                __activeControl[0]->setDirty(DIRTY_STATE);

            __activeControl[0] = __focusControl;
            __focusControl->_state = ACTIVE;
            __focusControl->notifyListeners(Control::Listener::PRESS);
            return true;
        }
        else if (!selectButtonPressed && __focusControl->_state == ACTIVE)
        {
            if (__activeControl[0])
                __activeControl[0]->setDirty(DIRTY_STATE);

            for (unsigned int i = 0; i < Touch::MAX_TOUCH_POINTS; ++i)
            {
                if (__activeControl[i] == __focusControl)
                {
                    __activeControl[i] = NULL;
                }
            }

            __focusControl->_state = NORMAL;
            __focusControl->notifyListeners(Control::Listener::RELEASE);
            __focusControl->notifyListeners(Control::Listener::CLICK);
            return true;
        }
        break;
    }

    // Dispatch gamepad events to focused controls (or their parents)
    Control * ctrl = __focusControl;
    while (ctrl)
    {
        if (ctrl->isEnabled() && ctrl->isVisible())
        {
            if (ctrl->gamepadEvent(evt, gamepad, analogIndex))
                return true;
        }

        ctrl = ctrl->getParent();
    }

    return false;
}

void Form::resizeEventInternal(unsigned int width, unsigned int height)
{
    for (size_t i = 0, size = __forms.size(); i < size; ++i)
    {
        Form* form = __forms[i];
        if (form)
        {
            // Dirty the form
            form->setDirty(Control::DIRTY_STATE);
        }
    }
}

bool Form::projectPoint(int x, int y, Vector3* point)
{
    if (!_node)
        return false;

    Scene* scene = _node->getScene();
    Camera* camera;

    if (scene && (camera = scene->getActiveCamera()))
    {
        // Get info about the form's position.
        Matrix m = _node->getWorldMatrix();
        Vector3 pointOnPlane(0, 0, 0);
        m.transformPoint(&pointOnPlane);

        // Unproject point into world space.
        Ray ray;
        camera->pickRay(Game::getInstance()->getViewport(), x, y, &ray);

        // Find the quad's plane.  We know its normal is the quad's forward vector.
        Vector3 normal = _node->getForwardVectorWorld().normalize();

        // To get the plane's distance from the origin, we project a point on the
        // plane onto the plane's normal vector.
        const float distance = fabs(Vector3::dot(pointOnPlane, normal));
        Plane plane(normal, -distance);

        // Check for collision with plane.
        float collisionDistance = ray.intersects(plane);
        if (collisionDistance != Ray::INTERSECTS_NONE)
        {
            // Multiply the ray's direction vector by collision distance and add that to the ray's origin.
            point->set(ray.getOrigin() + collisionDistance*ray.getDirection());

            // Project this point into the plane.
            m.invert();
            m.transformPoint(point);

            return true;
        }
    }
    return false;
}

void Form::controlDisabled(Control* control)
{
    if (__focusControl && (__focusControl == control || __focusControl->isChild(control)))
    {
        setFocusControl(NULL);
    }

    if (control->_state == ACTIVE || control->_state == HOVER)
    {
        for (unsigned int i = 0; i < Touch::MAX_TOUCH_POINTS; ++i)
        {
            if (__activeControl[i] == control)
            {
                __activeControl[i] = NULL;
            }
        }
    }
}

void Form::setFocusControl(Control* control)
{
    Control* oldFocus = __focusControl;

    __focusControl = control;

    // Deactivate the old focus control
    if (oldFocus)
    {
        oldFocus->setDirty(Control::DIRTY_STATE);
        oldFocus->notifyListeners(Control::Listener::FOCUS_LOST);
    }

    // Activate the new focus control
    if (__focusControl)
    {
        __focusControl->setDirty(Control::DIRTY_STATE);
        __focusControl->notifyListeners(Control::Listener::FOCUS_GAINED);

        // Set the activeControl property of the control's parent container
        Container* parent = NULL;
        if (__focusControl->_parent)
        {
            parent = __focusControl->_parent;
            parent->_activeControl = __focusControl;
        }

        // If this control is inside a scrollable container and is not fully visible,
        // scroll the container so that it is.
        if (parent && parent->_scroll != SCROLL_NONE && !parent->_viewportBounds.isEmpty())
        {
            const Rectangle& bounds = __focusControl->getBounds();
            if (bounds.x < parent->_scrollPosition.x)
            {
                // Control is to the left of the scrolled viewport.
                parent->_scrollPosition.x = -bounds.x;
            }
            else if (bounds.x + bounds.width > parent->_scrollPosition.x + parent->_viewportBounds.width)
            {
                // Control is off to the right.
                parent->_scrollPosition.x = -(bounds.x + bounds.width - parent->_viewportBounds.width);
            }

            if (bounds.y < parent->_viewportBounds.y - parent->_scrollPosition.y)
            {
                // Control is above the viewport.
                parent->_scrollPosition.y = -bounds.y;
            }
            else if (bounds.y + bounds.height > parent->_viewportBounds.height - parent->_scrollPosition.y)
            {
                // Control is below the viewport.
                parent->_scrollPosition.y = -(bounds.y + bounds.height - parent->_viewportBounds.height);
            }
        }
    }
}

}
