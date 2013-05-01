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

Form::Form() : _theme(NULL), _frameBuffer(NULL), _spriteBatch(NULL), _node(NULL),
    _nodeQuad(NULL), _nodeMaterial(NULL) , _u2(0), _v1(0), _isGamepad(false)
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

    // Get default projection matrix.
    Game* game = Game::getInstance();
    Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, &form->_defaultProjectionMatrix);

    form->updateBounds();

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
    const char* themeFile = formProperties->getString("theme");
    const char* layoutString = formProperties->getString("layout");
        
    Layout* layout;
    switch (getLayoutType(layoutString))
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
        GP_ERROR("Unsupported layout type '%d'.", getLayoutType(layoutString));
        break;
    }

    Theme* theme = Theme::create(themeFile);
    GP_ASSERT(theme);

    Form* form = new Form();
    form->_layout = layout;
    form->_theme = theme;

    // Get default projection matrix.
    Game* game = Game::getInstance();
    Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, &form->_defaultProjectionMatrix);

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

    form->_consumeInputEvents = formProperties->getBool("consumeInputEvents", false);

    // Alignment
    if ((form->_alignment & Control::ALIGN_BOTTOM) == Control::ALIGN_BOTTOM)
    {
        form->_bounds.y = Game::getInstance()->getHeight() - form->_bounds.height;
    }
    else if ((form->_alignment & Control::ALIGN_VCENTER) == Control::ALIGN_VCENTER)
    {
        form->_bounds.y = Game::getInstance()->getHeight() * 0.5f - form->_bounds.height * 0.5f;
    }

    if ((form->_alignment & Control::ALIGN_RIGHT) == Control::ALIGN_RIGHT)
    {
        form->_bounds.x = Game::getInstance()->getWidth() - form->_bounds.width;
    }
    else if ((form->_alignment & Control::ALIGN_HCENTER) == Control::ALIGN_HCENTER)
    {
        form->_bounds.x = Game::getInstance()->getWidth() * 0.5f - form->_bounds.width * 0.5f;
    }

    form->_scroll = getScroll(formProperties->getString("scroll"));
    form->_scrollBarsAutoHide = formProperties->getBool("scrollBarsAutoHide");
    if (form->_scrollBarsAutoHide)
    {
        form->_scrollBarOpacity = 0.0f;
    }

    // Add all the controls to the form.
    form->addControls(theme, formProperties);

    SAFE_DELETE(properties);
    
    form->updateBounds();

    __forms.push_back(form);

    return form;
}

Form* Form::getForm(const char* id)
{
    std::vector<Form*>::const_iterator it;
    for (it = __forms.begin(); it < __forms.end(); ++it)
    {
        Form* f = *it;
        GP_ASSERT(f);
        if (strcmp(id, f->getId()) == 0)
        {
            return f;
        }
    }
    return NULL;
}

Theme* Form::getTheme() const
{
    return _theme;
}

void Form::setSize(float width, float height)
{
    if (_autoWidth)
    {
        width = Game::getInstance()->getWidth();
    }

    if (_autoHeight)
    {
        height = Game::getInstance()->getHeight();
    }

    if (width != 0.0f && height != 0.0f &&
        (width != _bounds.width || height != _bounds.height))
    {
        // Width and height must be powers of two to create a texture.
        unsigned int w = nextPowerOfTwo(width);
        unsigned int h = nextPowerOfTwo(height);
        _u2 = width / (float)w;
        _v1 = height / (float)h;

        // Create framebuffer if necessary. TODO: Use pool to cache.
        if (_frameBuffer)
            SAFE_RELEASE(_frameBuffer)
        
        _frameBuffer = FrameBuffer::create(_id.c_str(), w, h);
        GP_ASSERT(_frameBuffer);

        // Re-create projection matrix.
        Matrix::createOrthographicOffCenter(0, width, height, 0, 0, 1, &_projectionMatrix);

        // Re-create sprite batch.
        SAFE_DELETE(_spriteBatch);
        _spriteBatch = SpriteBatch::create(_frameBuffer->getRenderTarget()->getTexture());
        GP_ASSERT(_spriteBatch);

        // Clear the framebuffer black
        Game* game = Game::getInstance();
        FrameBuffer* previousFrameBuffer = _frameBuffer->bind();
        Rectangle previousViewport = game->getViewport();

        game->setViewport(Rectangle(0, 0, width, height));
        _theme->setProjectionMatrix(_projectionMatrix);
        game->clear(Game::CLEAR_COLOR, Vector4::zero(), 1.0, 0);
        _theme->setProjectionMatrix(_defaultProjectionMatrix);

        previousFrameBuffer->bind();
        game->setViewport(previousViewport);
    }
    _bounds.width = width;
    _bounds.height = height;
    _dirty = true;
}

void Form::setBounds(const Rectangle& bounds)
{
    setPosition(bounds.x, bounds.y);
    setSize(bounds.width, bounds.height);
}

void Form::setWidth(float width)
{
    setSize(width, _bounds.height);
}

void Form::setHeight(float height)
{
    setSize(_bounds.width, height);
}

void Form::setAutoWidth(bool autoWidth)
{
    if (_autoWidth != autoWidth)
    {
        _autoWidth = autoWidth;
        _dirty = true;

        if (_autoWidth)
        {
            setSize(_bounds.width, Game::getInstance()->getWidth());
        }
    }
}

void Form::setAutoHeight(bool autoHeight)
{
    if (_autoHeight != autoHeight)
    {
        _autoHeight = autoHeight;
        _dirty = true;

        if (_autoHeight)
        {
            setSize(_bounds.width, Game::getInstance()->getHeight());
        }
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
    // If the user wants a custom node then we need to create a 3D quad
    if (node && node != _node)
    {
        // Set this Form up to be 3D by initializing a quad.
        float x2 = _bounds.width;
        float y2 = _bounds.height;
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
        Texture::Sampler* sampler = Texture::Sampler::create(_frameBuffer->getRenderTarget()->getTexture());
        GP_ASSERT(sampler);
        sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
        _nodeMaterial->getParameter("u_texture")->setValue(sampler);
        sampler->release();

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
        updateBounds();

        // Cache themed attributes for performance.
        _skin = getSkin(_state);
        _opacity = getOpacity(_state);

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

void Form::updateBounds()
{   
    _clearBounds.set(_absoluteClipBounds);

    // Calculate the clipped bounds.
    float x = 0;
    float y = 0;
    float width = _bounds.width;
    float height = _bounds.height;

    Rectangle clip(0, 0, _bounds.width, _bounds.height);

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
    x = 0;
    y = 0;
    _absoluteBounds.set(x, y, _bounds.width, _bounds.height);

    // Calculate the absolute viewport bounds. Absolute bounds minus border and padding.
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding& padding = getPadding();

    x += border.left + padding.left;
    y += border.top + padding.top;
    width = _bounds.width - border.left - padding.left - border.right - padding.right;
    height = _bounds.height - border.top - padding.top - border.bottom - padding.bottom;

    _viewportBounds.set(x, y, width, height);

    // Calculate the clip area. Absolute bounds, minus border and padding, clipped to the parent container's clip area.
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
    _absoluteClipBounds.set(x - border.left - padding.left, y - border.top - padding.top,
                            width + border.left + padding.left + border.right + padding.right,
                            height + border.top + padding.top + border.bottom + padding.bottom);
    if (_clearBounds.isEmpty())
    {
        _clearBounds.set(_absoluteClipBounds);
    }

    // Get scrollbar images and diminish clipping bounds to make room for scrollbars.
    if ((_scroll & SCROLL_HORIZONTAL) == SCROLL_HORIZONTAL)
    {
        _scrollBarLeftCap = getImage("scrollBarLeftCap", _state);
        _scrollBarHorizontal = getImage("horizontalScrollBar", _state);
        _scrollBarRightCap = getImage("scrollBarRightCap", _state);

        _viewportClipBounds.height -= _scrollBarHorizontal->getRegion().height;
    }

    if ((_scroll & SCROLL_VERTICAL) == SCROLL_VERTICAL)
    {
        _scrollBarTopCap = getImage("scrollBarTopCap", _state);
        _scrollBarVertical = getImage("verticalScrollBar", _state);
        _scrollBarBottomCap = getImage("scrollBarBottomCap", _state);
        
        _viewportClipBounds.width -= _scrollBarVertical->getRegion().width;
    }
}

void Form::draw()
{
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
        GP_ASSERT(_frameBuffer);
        FrameBuffer* previousFrameBuffer = _frameBuffer->bind();

        Game* game = Game::getInstance();
        Rectangle prevViewport = game->getViewport();
        game->setViewport(Rectangle(0, 0, _bounds.width, _bounds.height));

        GP_ASSERT(_theme);
        _theme->setProjectionMatrix(_projectionMatrix);
        
        // By setting needsClear to true here, an optimization meant to clear and redraw only areas of the form
        // that have changed is disabled.  Currently, repositioning controls can result in areas of the screen being cleared
        // after another control has been drawn there.  This should probably be done in two passes -- one to clear areas where
        // dirty controls were last frame, and another to draw them where they are now.
        Container::draw(_theme->getSpriteBatch(), Rectangle(0, 0, _bounds.width, _bounds.height),
                        /*_skin != NULL*/ true, false, _bounds.height);
        _theme->setProjectionMatrix(_defaultProjectionMatrix);

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
        if (!_spriteBatch)
        {
            _spriteBatch = SpriteBatch::create(_frameBuffer->getRenderTarget()->getTexture());
            GP_ASSERT(_spriteBatch);
        }
        _spriteBatch->start();
        _spriteBatch->draw(_bounds.x, _bounds.y, 0, _bounds.width, _bounds.height, 0, _v1, _u2, 0, Vector4::one());
        _spriteBatch->finish();
    }
}

const char* Form::getType() const
{
    return "form";
}

void Form::updateInternal(float elapsedTime)
{
    size_t size = __forms.size();
    for (size_t i = 0; i < size; ++i)
    {
        Form* form = __forms[i];
        GP_ASSERT(form);

        if (form->isEnabled() && form->isVisible())
        {
            form->update(elapsedTime);
        }
    }
}

static bool shouldPropagateTouchEvent(Control::State state, Touch::TouchEvent evt, const Rectangle& bounds, int x, int y)
{
    return (state != Control::NORMAL ||
            (evt == Touch::TOUCH_PRESS &&
             x >= bounds.x &&
             x <= bounds.x + bounds.width &&
             y >= bounds.y &&
             y <= bounds.y + bounds.height));
}

bool Form::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // Check for a collision with each Form in __forms.
    // Pass the event on.
    size_t size = __forms.size();
    for (size_t i = 0; i < size; ++i)
    {
        Form* form = __forms[i];
        GP_ASSERT(form);

        if (form->isEnabled() && form->isVisible())
        {
            if (form->_node)
            {
                Vector3 point;
                if (form->projectPoint(x, y, &point))
                {
                    const Rectangle& bounds = form->getBounds();
                    if (shouldPropagateTouchEvent(form->getState(), evt, bounds, point.x, point.y))
                    {
                        if (form->touchEvent(evt, point.x - bounds.x, bounds.height - point.y - bounds.y, contactIndex))
                            return true;
                    }
                }
            }
            else
            {
                // Simply compare with the form's bounds.
                const Rectangle& bounds = form->getBounds();
                if (shouldPropagateTouchEvent(form->getState(), evt, bounds, x, y))
                {
                    // Pass on the event's position relative to the form.
                    if (form->touchEvent(evt, x - bounds.x, y - bounds.y, contactIndex))
                        return true;
                }
            }
        }
    }
    return false;
}

bool Form::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    size_t size = __forms.size();
    for (size_t i = 0; i < size; ++i)
    {
        Form* form = __forms[i];
        GP_ASSERT(form);
        if (form->isEnabled() && form->isVisible() && form->hasFocus() && !form->_isGamepad)
        {
            if (form->keyEvent(evt, key))
                return true;
        }
    }
    return false;
}

static bool shouldPropagateMouseEvent(Control::State state, Mouse::MouseEvent evt, const Rectangle& bounds, int x, int y)
{
    return (state != Control::NORMAL ||
            ((evt == Mouse::MOUSE_PRESS_LEFT_BUTTON ||
              evt == Mouse::MOUSE_PRESS_MIDDLE_BUTTON ||
              evt == Mouse::MOUSE_PRESS_RIGHT_BUTTON ||
              evt == Mouse::MOUSE_MOVE ||
              evt == Mouse::MOUSE_WHEEL) &&
                x >= bounds.x &&
                x <= bounds.x + bounds.width &&
                y >= bounds.y &&
                y <= bounds.y + bounds.height));
}

bool Form::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    for (size_t i = 0; i < __forms.size(); ++i)
    {
        Form* form = __forms[i];
        GP_ASSERT(form);

        if (form->isEnabled() && form->isVisible())
        {
            if (form->_node)
            {
                Vector3 point;
                if (form->projectPoint(x, y, &point))
                {
                    const Rectangle& bounds = form->getBounds();
                    if (shouldPropagateMouseEvent(form->getState(), evt, bounds, point.x, point.y))
                    {
                        if (form->mouseEvent(evt, point.x - bounds.x, bounds.height - point.y - bounds.y, wheelDelta))
                            return true;
                    }
                }
            }
            else
            {
                // Simply compare with the form's bounds.
                const Rectangle& bounds = form->getBounds();
                if (shouldPropagateMouseEvent(form->getState(), evt, bounds, x, y))
                {
                    // Pass on the event's position relative to the form.
                    if (form->mouseEvent(evt, x - bounds.x, y - bounds.y, wheelDelta))
                        return true;
                }
            }
        }
    }
    return false;
}

void Form::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    for (size_t i = 0; i < __forms.size(); ++i)
    {
        Form* form = __forms[i];
        GP_ASSERT(form);

        if (form->isEnabled() && form->isVisible() && form->hasFocus())
        {
            if (form->gamepadEvent(evt, gamepad, analogIndex))
                return;
        }
    }
}

bool Form::projectPoint(int x, int y, Vector3* point)
{
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

}
