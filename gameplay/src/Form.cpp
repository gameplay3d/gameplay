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

// Default form shaders
#define FORM_VSH "res/shaders/form.vert"
#define FORM_FSH "res/shaders/form.frag"

namespace gameplay
{

static Effect* __formEffect = NULL;
static std::vector<Form*> __forms;
Control* Form::_focusControl = NULL;
Control* Form::_activeControl = NULL;
Control::State Form::_activeControlState = Control::NORMAL;

Form::Form() : _theme(NULL), _frameBuffer(NULL), _spriteBatch(NULL), _node(NULL),
    _nodeQuad(NULL), _nodeMaterial(NULL) , _u2(0), _v1(0)
{
}

Form::~Form()
{
    SAFE_DELETE(_spriteBatch);
    SAFE_RELEASE(_frameBuffer);
    SAFE_RELEASE(_theme);

    if (__formEffect)
    {
        if (__formEffect->getRefCount() == 1)
        {
            __formEffect->release();
            __formEffect = NULL;
        }
    }

    // Remove this Form from the global list.
    std::vector<Form*>::iterator it = std::find(__forms.begin(), __forms.end(), this);
    if (it != __forms.end())
    {
        __forms.erase(it);
    }
}

Form* Form::create(const char* id, Theme::Style* style, Layout::Type layoutType)
{
    GP_ASSERT(style);

    Layout* layout;
    switch (layoutType)
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
    default:
        GP_ERROR("Unsupported layout type '%d'.", layoutType);
        break;
    }

    Form* form = new Form();
    if (id)
        form->_id = id;
    form->_style = style;
    form->_layout = layout;
    form->_theme = style->getTheme();
    form->_theme->addRef();

    form->updateFrameBuffer();

    __forms.push_back(form);

    return form;
}

Form* Form::create(const char* url)
{
    // Load Form from .form file.
    Properties* properties = Properties::create(url);
    if (properties == NULL)
    {
        GP_ASSERT(properties);
        return NULL;
    }

    // Check if the Properties is valid and has a valid namespace.
    Properties* formProperties = (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
    assert(formProperties);
    if (!formProperties || !(strcmp(formProperties->getNamespace(), "form") == 0))
    {
        GP_ASSERT(formProperties);
        SAFE_DELETE(properties);
        return NULL;
    }

    // Create new form with given ID, theme and layout.
    std::string themeFile;
    formProperties->getPath("theme", &themeFile);

    // Parse layout
    Layout* layout = NULL;
    Properties* layoutNS = formProperties->getNamespace("layout", true, false);
    if (layoutNS)
    {
        Layout::Type layoutType = getLayoutType(layoutNS->getString("type"));
        switch (layoutType)
        {
        case Layout::LAYOUT_ABSOLUTE:
            layout = AbsoluteLayout::create();
            break;
        case Layout::LAYOUT_FLOW:
            layout = FlowLayout::create();
            static_cast<FlowLayout*>(layout)->setSpacing(layoutNS->getInt("horizontalSpacing"), layoutNS->getInt("verticalSpacing"));
            break;
        case Layout::LAYOUT_VERTICAL:
            layout = VerticalLayout::create();
            static_cast<VerticalLayout*>(layout)->setSpacing(layoutNS->getInt("spacing"));
            break;
        }
    }
    else
    {
        switch (getLayoutType(formProperties->getString("layout")))
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
    }
    if (layout == NULL)
    {
        GP_ERROR("Unsupported layout type for form: %s", url);
    }

    Theme* theme = Theme::create(themeFile.c_str());
    GP_ASSERT(theme);

    Form* form = new Form();
    form->_layout = layout;
    form->_theme = theme;

    Theme::Style* style = NULL;
    const char* styleName = formProperties->getString("style");
    if (styleName)
    {
        style = theme->getStyle(styleName);
    }
    else
    {
        style = theme->getEmptyStyle();
    }
    form->initialize(style, formProperties);

    form->_consumeInputEvents = formProperties->getBool("consumeInputEvents", true);

    form->_scroll = getScroll(formProperties->getString("scroll"));
    form->_scrollBarsAutoHide = formProperties->getBool("scrollBarsAutoHide");
    if (form->_scrollBarsAutoHide)
    {
        form->_scrollBarOpacity = 0.0f;
    }

    // Add all the controls to the form.
    form->addControls(theme, formProperties);

    SAFE_DELETE(properties);

    form->updateFrameBuffer();

    __forms.push_back(form);

    return form;
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

bool Form::isForm() const
{
    return true;
}

Theme* Form::getTheme() const
{
    return _theme;
}

void Form::updateFrameBuffer()
{
    float width = _absoluteClipBounds.width;
    float height = _absoluteClipBounds.height;

    SAFE_RELEASE(_frameBuffer);
    SAFE_DELETE(_spriteBatch);

    if (width != 0.0f && height != 0.0f)
    {
        // Width and height must be powers of two to create a texture.
        unsigned int w = nextPowerOfTwo(width);
        unsigned int h = nextPowerOfTwo(height);
        _u2 = width / (float)w;
        _v1 = height / (float)h;
        
        _frameBuffer = FrameBuffer::create(_id.c_str(), w, h);
        GP_ASSERT(_frameBuffer);

        // Re-create projection matrix for drawing onto framebuffer
        Matrix::createOrthographicOffCenter(0, width, height, 0, 0, 1, &_projectionMatrix);

        // Re-create sprite batch
        _spriteBatch = SpriteBatch::create(_frameBuffer->getRenderTarget()->getTexture());
        GP_ASSERT(_spriteBatch);

        // Compute full-viewport ortho matrix for drawing frame buffer onto screen
        Matrix viewportProjection;
        Matrix::createOrthographicOffCenter(0, Game::getInstance()->getViewport().width, Game::getInstance()->getViewport().height, 0, 0, 1, &viewportProjection);
        _spriteBatch->setProjectionMatrix(viewportProjection);

        // Clear the framebuffer black
        Game* game = Game::getInstance();
        FrameBuffer* previousFrameBuffer = _frameBuffer->bind();
        Rectangle previousViewport = game->getViewport();

        game->setViewport(Rectangle(0, 0, width, height));
        _theme->setProjectionMatrix(_projectionMatrix);
        game->clear(Game::CLEAR_COLOR, Vector4::zero(), 1.0, 0);

        previousFrameBuffer->bind();
        game->setViewport(previousViewport);

        // Force any attached node to be updated
        setNode(_node);
    }
}

static Effect* createEffect()
{
    Effect* effect = NULL;
    if (__formEffect == NULL)
    {
        __formEffect = Effect::createFromFile(FORM_VSH, FORM_FSH);
        if (__formEffect == NULL)
        {
            GP_ERROR("Unable to load form effect.");
            return NULL;
        }
        effect = __formEffect;
    }
    else
    {
        effect = __formEffect;
    }
    return effect;
}

void Form::setNode(Node* node)
{
    // If we were already attached to a node, remove ourself from it
    if (_node)
    {
        _node->setModel(NULL);
        _nodeQuad = NULL;
        _nodeMaterial = NULL;
        _node = NULL;
    }

    if (node)
    {
        // Set this Form up to be 3D by initializing a quad.
        float x2 = _absoluteBounds.width;
        float y2 = _absoluteBounds.height;
        float vertices[] =
        {
            0, y2, 0,   0, _v1,
            0, 0, 0,    0, 0,
            x2, y2, 0,  _u2, _v1,
            x2, 0, 0,   _u2, 0
        };
        VertexFormat::Element elements[] =
        {
            VertexFormat::Element(VertexFormat::POSITION, 3),
            VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
        };
        Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), 4, false);
        GP_ASSERT(mesh);
        mesh->setPrimitiveType(Mesh::TRIANGLE_STRIP);
        mesh->setVertexData(vertices, 0, 4);

        _nodeQuad = Model::create(mesh);
        SAFE_RELEASE(mesh);
        GP_ASSERT(_nodeQuad);

        // Create the effect and material
        Effect* effect = createEffect();
        GP_ASSERT(effect);
        _nodeMaterial = Material::create(effect);

        GP_ASSERT(_nodeMaterial);
        _nodeQuad->setMaterial(_nodeMaterial);
        _nodeMaterial->release();
        node->setModel(_nodeQuad);
        _nodeQuad->release();

        // Bind the WorldViewProjection matrix.
        _nodeMaterial->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);

        // Bind the texture from the framebuffer and set the texture to clamp
        if (_frameBuffer)
        {
            Texture::Sampler* sampler = Texture::Sampler::create(_frameBuffer->getRenderTarget()->getTexture());
            GP_ASSERT(sampler);
            sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
            _nodeMaterial->getParameter("u_texture")->setValue(sampler);
            sampler->release();
        }

        RenderState::StateBlock* rsBlock = _nodeMaterial->getStateBlock();
        rsBlock->setDepthWrite(true);
        rsBlock->setBlend(true);
        rsBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
        rsBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
    }

    _node = node;
}

void Form::update(float elapsedTime)
{
    if (isDirty())
    {
        update(NULL, Vector2::zero());

        Control::State state = getState();

        // Cache themed attributes for performance.
        _skin = getSkin(state);
        _opacity = getOpacity(state);

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
}

void Form::update(const Control* container, const Vector2& offset)
{
    // Store previous absolute bounds
    Rectangle oldAbsoluteClipBounds = _absoluteClipBounds;

    _layout->align(this, NULL);

    Container::update(container, offset);

    if (_absoluteClipBounds.width != oldAbsoluteClipBounds.width || _absoluteClipBounds.height != oldAbsoluteClipBounds.height)
    {
        updateFrameBuffer();
    }
}

unsigned int Form::draw()
{
    if (!_visible || !_frameBuffer)
        return 0;

    // The first time a form is drawn, its contents are rendered into a framebuffer.
    // The framebuffer will only be drawn into again when the contents of the form change.
    // If this form has a node then it's a 3D form and the framebuffer will be used
    // to texture a quad.  The quad will be given the same dimensions as the form and
    // must be transformed appropriately by the user, unless they call setQuad() themselves.
    // On the other hand, if this form has not been set on a node, SpriteBatch will be used
    // to render the contents of the framebuffer directly to the display.

    // Check whether this form has changed since the last call to draw() and if so, render into the framebuffer.
    if (isDirty())
    {
        FrameBuffer* previousFrameBuffer = _frameBuffer->bind();

        Game* game = Game::getInstance();
        Rectangle prevViewport = game->getViewport();
        game->setViewport(Rectangle(0, 0, _absoluteClipBounds.width, _absoluteClipBounds.height));

        GP_ASSERT(_theme);
        _theme->setProjectionMatrix(_projectionMatrix);

        // By setting needsClear to true here, an optimization meant to clear and redraw only areas of the form
        // that have changed is disabled.  Currently, repositioning controls can result in areas of the screen being cleared
        // after another control has been drawn there.  This should probably be done in two passes -- one to clear areas where
        // dirty controls were last frame, and another to draw them where they are now.
        Container::draw(_theme->getSpriteBatch(), _absoluteClipBounds, /*_skin != NULL*/ true, false, _absoluteClipBounds.height);

        // Restore the previous game viewport.
        game->setViewport(prevViewport);
        // Rebind the previous framebuffer and game viewport.
        previousFrameBuffer->bind();
    }

    // Draw either with a 3D quad or sprite batch.
    if (_node)
    {
         // If we have the node set, then draw a 3D quad model.
        _nodeQuad->draw();
    }
    else
    {
        // Otherwise we draw the framebuffer in ortho space with a spritebatch.
        _spriteBatch->start();
        _spriteBatch->draw(_bounds.x, _bounds.y, 0, _bounds.width, _bounds.height, 0, _v1, _u2, 0, Vector4::one());
        _spriteBatch->finish();
    }
    return 2;
}

const char* Form::getType() const
{
    return "form";
}

Control* Form::getActiveControl()
{
    return _activeControl;
}

void Form::updateInternal(float elapsedTime)
{
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
    Form* form = ctrl->getForm();
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
                *y = form->_bounds.height - (int)point.y;
            }
            else
            {
                return false;
            }
        }

        *x -= form->_bounds.x;
        *y -= form->_bounds.y;

        return true;
    }

    return false;
}

Control* Form::findInputControl(int* x, int* y, bool focus)
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
        Control* ctrl = findInputControl(form, formX, formY, focus);
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

Control* Form::findInputControl(Control* control, int x, int y, bool focus)
{
    Control* result = NULL;

    // Does the passed in control's bounds intersect the specified coordinates - and 
    // does the control support the specified input state?
    if (control->_consumeInputEvents && control->_visible && control->_enabled && (!focus || control->canFocus()))
    {
        if (control->_absoluteClipBounds.contains(x, y))
            result = control;
    }

    // If the control has children, search for an input control inside it that also
    // supports the above conditions.
    if (control->isContainer())
    {
        Container* container = static_cast<Container*>(control);
        for (int i = (int)container->getControlCount() - 1; i >= 0; --i)
        {
            Control* ctrl = findInputControl(container->getControl((unsigned int)i), x, y, focus);
            if (ctrl)
                result = ctrl;
        }
    }

    return result;
}

Control* Form::handlePointerPressRelease(int* x, int* y, bool pressed)
{
    Control* ctrl = NULL;

    int newX = *x;
    int newY = *y;

    if (pressed)
    {
        // Update active state changes
        if ((ctrl = findInputControl(&newX, &newY, false)) != NULL)
        {
            if (_activeControl != ctrl || _activeControlState != Control::ACTIVE)
            {
                if (_activeControl)
                    _activeControl->_dirty = true;

                _activeControl = ctrl;
                _activeControlState = Control::ACTIVE;
                _activeControl->_dirty = true;
            }

            ctrl->notifyListeners(Control::Listener::PRESS);
        }

        // Update focus state?
        if (!(ctrl && ctrl->canFocus()))
        {
            newX = *x;
            newY = *y;
            ctrl = findInputControl(&newX, &newY, true);
        }

        // Update focus
        if (_focusControl != ctrl)
        {
            setFocusControl(ctrl);
        }
    }
    else // !pressed
    {
        Control* active = _activeControlState == Control::ACTIVE ? _activeControl : NULL;

        if (active)
        {
            active->addRef(); // protect against event-hanlder evil

            // Release happened for the active control (that was pressed)
            ctrl = active;

            // Transform point to form-space
            screenToForm(ctrl, &newX, &newY);

            // No longer any active control
            _activeControl->_dirty = true;
            _activeControl = NULL;
            _activeControlState = Control::NORMAL;
        }
        else
        {
            // Update active and hover control state on release
            Control* inputControl = findInputControl(&newX, &newY, false);
            if (inputControl)
            {
                ctrl = inputControl;

                if (_activeControl != ctrl || _activeControlState != Control::HOVER)
                {
                    if (_activeControl)
                        _activeControl->_dirty = true;

                    _activeControl = ctrl;
                    _activeControlState = Control::HOVER;
                    _activeControl->_dirty = true;
                }
            }
            else
            {
                // No longer any active control
                if (_activeControl)
                    _activeControl->_dirty = true;

                _activeControl = NULL;
                _activeControlState = Control::NORMAL;
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
                Control* parent = active->getParent();
                if (!parent || (parent->isContainer() && !static_cast<Container*>(parent)->isScrolling()))
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

Control* Form::handlePointerMove(int* x, int* y)
{
    Control* ctrl = NULL;

    // Handle hover control changes on move, only if there is no currently active control
    // (i.e. when the mouse or a finger is not down).
    if (_activeControl && (_activeControlState == Control::ACTIVE))
    {
        ctrl = _activeControl;
        screenToForm(ctrl, x, y);
    }
    else
    {
        ctrl = findInputControl(x, y, false);
        if (ctrl)
        {
            // Update hover control
            if (_activeControl != ctrl || _activeControlState != Control::HOVER)
            {
                if (_activeControl)
                    _activeControl->_dirty = true;

                _activeControl = ctrl;
                _activeControlState = Control::HOVER;
                _activeControl->_dirty = true;
            }
        }
        else
        {
            // No active/hover control
            if (_activeControl)
                _activeControl->_dirty = true;

            _activeControl = NULL;
            _activeControlState = Control::NORMAL;
        }
    }

    return ctrl;
}

void Form::verifyRemovedControlState(Control* control)
{
    if (_focusControl == control)
        _focusControl = NULL;

    if (_activeControl == control)
    {
        _activeControl = NULL;
        _activeControlState = Control::NORMAL;
    }
}

bool Form::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    Control* ctrl = NULL;
    int formX = x;
    int formY = y;

    // Handle focus, active and hover state changes
    if (contactIndex == 0)
    {
        switch (evt)
        {
        case Touch::TOUCH_PRESS:
        case Touch::TOUCH_RELEASE:
            ctrl = handlePointerPressRelease(&formX, &formY, evt == Touch::TOUCH_PRESS);
            break;

        case Touch::TOUCH_MOVE:
            ctrl = handlePointerMove(&formX, &formY);
            break;
        }
    }

    // Dispatch input events to all controls that intersect this point
    if (ctrl == NULL)
    {
        formX = x;
        formY = y;
        ctrl = findInputControl(&formX, &formY, false);
    }

    if (ctrl)
    {
        // Dispatch the event from the bottom upwards, until a control intersecting the point consumes the event
        while (ctrl)
        {
            if (ctrl->touchEvent(evt, formX - ctrl->_absoluteBounds.x, formY - ctrl->_absoluteBounds.y, contactIndex))
                return true;

            // Consume all input events anyways?
            if (ctrl->getConsumeInputEvents())
                return true;

            ctrl = ctrl->getParent();
        }
    }

    return false;
}

bool Form::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    // Do not process mouse input when mouse is captured
    if (Game::getInstance()->isMouseCaptured())
        return false;

    Control* ctrl = NULL;
    int formX = x;
    int formY = y;

    // Handle focus, active and hover state changes
    switch (evt)
    {
    case Mouse::MOUSE_PRESS_LEFT_BUTTON:
    case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
        ctrl = handlePointerPressRelease(&formX, &formY, evt == Mouse::MOUSE_PRESS_LEFT_BUTTON);
        break;

    case Mouse::MOUSE_MOVE:
        ctrl = handlePointerMove(&formX, &formY);
        break;
    }

    // Dispatch input events to all controls that intersect this point
    if (ctrl == NULL)
    {
        formX = x;
        formY = y;
        ctrl = findInputControl(&formX, &formY, false);
    }

    if (ctrl)
    {
        // Handle container scrolling
        Control* tmp = ctrl;
        while (tmp)
        {
            if (tmp->isContainer())
            {
                Container* container = static_cast<Container*>(tmp);
                if (container->_scroll != SCROLL_NONE)
                {
                    if (container->mouseEventScroll(evt, formX - tmp->_absoluteBounds.x, formY - tmp->_absoluteBounds.y, wheelDelta))
                        return true;
                    break; // scrollable parent container found
                }
            }
            tmp = tmp->_parent;
        }

        // Dispatch the event from the bottom upwards, until a control intersecting the point consumes the event
        while (ctrl)
        {
            int localX = formX - ctrl->_absoluteBounds.x;
            int localY = formY - ctrl->_absoluteBounds.y;
            if (ctrl->mouseEvent(evt, localX, localY, wheelDelta))
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

            // Consume all input events anyways?
            if (ctrl->getConsumeInputEvents())
                return true;

            ctrl = ctrl->getParent();
        }
    }

    return false;
}

bool Form::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    // Ignore the escape key
    if (key == Keyboard::KEY_ESCAPE)
        return false;

    // Handle focus changing
    if (_focusControl)
    {
        switch (evt)
        {
        case Keyboard::KeyEvent::KEY_PRESS:
            switch (key)
            {
            case Keyboard::KEY_TAB:
                if (_focusControl->_parent && _focusControl->_parent->isContainer())
                {
                    if (static_cast<Container*>(_focusControl->_parent)->moveFocus(Container::NEXT))
                        return true;
                }
                break;
            }
            break;
        }
    }

    // Dispatch key events
    Control* ctrl = _focusControl;
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

void Form::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    for (int i = (int)__forms.size() - 1; i >= 0; --i)
    {
        Form* form = __forms[i];

        if (form && form->isEnabled() && form->isVisible() && form->hasFocus())
        {
            if (form->gamepadEvent(evt, gamepad, analogIndex))
                return;
        }
    }
}

void Form::resizeEventInternal(unsigned int width, unsigned int height)
{
    for (size_t i = 0, size = __forms.size(); i < size; ++i)
    {
        Form* form = __forms[i];
        if (form)
        {
            if (form->_spriteBatch)
            {
                // Update viewport projection matrix
                Matrix viewportProjection;
                Matrix::createOrthographicOffCenter(0, Game::getInstance()->getViewport().width, Game::getInstance()->getViewport().height, 0, 0, 1, &viewportProjection);
                form->_spriteBatch->setProjectionMatrix(viewportProjection);
            }

            // Dirty the form
            form->_dirty = true;
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

unsigned int Form::nextPowerOfTwo(unsigned int v)
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
    else
    {
        return v;
    }
}

void Form::controlDisabled(Control* control)
{
    if (Form::_focusControl && (Form::_focusControl == control || Form::_focusControl->isChild(control)))
    {
        setFocusControl(NULL);
    }

    if (Form::_activeControl)
    {
        if (Form::_activeControl == control || Form::_activeControl->isChild(control))
        {
            Form::_activeControl = NULL;
            Form::_activeControlState = Control::NORMAL;
        }
    }
}

void Form::setFocusControl(Control* control)
{
    Control* oldFocus = _focusControl;

    _focusControl = control;

    if (oldFocus)
    {
        oldFocus->_dirty = true;
        oldFocus->notifyListeners(Control::Listener::FOCUS_LOST);
    }

    if (_focusControl)
    {
        _focusControl->_dirty = true;
        _focusControl->notifyListeners(Control::Listener::FOCUS_GAINED);
    }
}

}
