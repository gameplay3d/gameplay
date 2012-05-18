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

namespace gameplay
{

static std::vector<Form*> __forms;

Form::Form() : _theme(NULL), _quad(NULL), _node(NULL), _frameBuffer(NULL), _spriteBatch(NULL)
{
}

Form::Form(const Form& copy)
{
}

Form::~Form()
{
    SAFE_RELEASE(_quad);
    SAFE_RELEASE(_node);
    SAFE_RELEASE(_frameBuffer);
    SAFE_RELEASE(_theme);
    SAFE_DELETE(_spriteBatch);

    // Remove this Form from the global list.
    std::vector<Form*>::iterator it = std::find(__forms.begin(), __forms.end(), this);
    if (it != __forms.end())
    {
        __forms.erase(it);
    }
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
    }

    Theme* theme = Theme::create(themeFile);
    GP_ASSERT(theme);

    Form* form = new Form();
    form->_layout = layout;
    form->_theme = theme;

    const char* styleName = formProperties->getString("style");
    form->initialize(theme->getStyle(styleName), formProperties);

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

    form->_scrollState = getScrollState(formProperties->getString("scroll"));

    // Add all the controls to the form.
    form->addControls(theme, formProperties);

    Game* game = Game::getInstance();
    Matrix::createOrthographicOffCenter(0, game->getWidth(), game->getHeight(), 0, 0, 1, &form->_defaultProjectionMatrix);

    SAFE_DELETE(properties);

    __forms.push_back(form);

    return form;
}

Form* Form::getForm(const char* id)
{
    std::vector<Form*>::const_iterator it;
    for (it = __forms.begin(); it < __forms.end(); it++)
    {
        Form* f = *it;
        GP_ASSERT(f);
        if (strcmp(id, f->getID()) == 0)
        {
            return f;
        }
    }
        
    return NULL;
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

    if (width != _bounds.width || height != _bounds.height)
    {
        // Width and height must be powers of two to create a texture.
        int w = width;
        int h = height;

        if (!((w & (w - 1)) == 0))
        {
            w = nextHighestPowerOfTwo(w);
        }

        if (!((h & (h - 1)) == 0))
        {
            h = nextHighestPowerOfTwo(h);
        }

        _u2 = width / (float)w;
        _v1 = height / (float)h;

        // Create framebuffer if necessary.
        if (!_frameBuffer)
        {
            _frameBuffer = FrameBuffer::create(_id.c_str());
            GP_ASSERT(_frameBuffer);
        }
     
        // Re-create render target.
        RenderTarget* rt = RenderTarget::create(_id.c_str(), w, h);
        GP_ASSERT(rt);
        _frameBuffer->setRenderTarget(rt);
        SAFE_RELEASE(rt);

        // Re-create projection matrix.
        Matrix::createOrthographicOffCenter(0, width, height, 0, 0, 1, &_projectionMatrix);

        // Re-create sprite batch.
        SAFE_DELETE(_spriteBatch);
        _spriteBatch = SpriteBatch::create(_frameBuffer->getRenderTarget()->getTexture());
        GP_ASSERT(_spriteBatch);

        _bounds.width = width;
        _bounds.height = height;
        _dirty = true;
    }
}

void Form::setBounds(const Rectangle& bounds)
{
    setPosition(bounds.x, bounds.y);
    setSize(bounds.width, bounds.height);
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

void Form::setQuad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
{
    Mesh* mesh = Mesh::createQuad(p1, p2, p3, p4);

    initializeQuad(mesh);
    SAFE_RELEASE(mesh);
}

void Form::setQuad(float x, float y, float width, float height)
{
    float x2 = x + width;
    float y2 = y + height;

    float vertices[] =
    {
        x, y2, 0,   0, _v1,
        x, y, 0,    0, 0,
        x2, y2, 0,  _u2, _v1,
        x2, y, 0,   _u2, 0
    };

    VertexFormat::Element elements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2)
    };
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), 4, false);
    assert(mesh);

    mesh->setPrimitiveType(Mesh::TRIANGLE_STRIP);
    mesh->setVertexData(vertices, 0, 4);

    initializeQuad(mesh);
    SAFE_RELEASE(mesh);
}

void Form::setNode(Node* node)
{
    _node = node;
        
    if (_node)
    {
        // Set this Form up to be 3D by initializing a quad.
        setQuad(0.0f, 0.0f, _bounds.width, _bounds.height);
        _node->setModel(_quad);
    }
}

void Form::update(long elapsedTime)
{
    if (isDirty())
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

        _absoluteClipBounds.set(x - border.left - padding.left, y - border.top - padding.top,
            width + border.left + padding.left + border.right + padding.right,
            height + border.top + padding.top + border.bottom + padding.bottom);
        if (_clearBounds.isEmpty())
        {
            _clearBounds.set(_absoluteClipBounds);
        }

        // Cache themed attributes for performance.
        _skin = getSkin(_state);
        _opacity = getOpacity(_state);

        // Get scrollbar images and diminish clipping bounds to make room for scrollbars.
        if ((_scrollState & SCROLL_HORIZONTAL) == SCROLL_HORIZONTAL)
        {
            _scrollBarLeftCap = getImage("scrollBarLeftCap", _state);
            _scrollBarHorizontal = getImage("horizontalScrollBar", _state);
            _scrollBarRightCap = getImage("scrollBarRightCap", _state);

            _viewportClipBounds.height -= _scrollBarHorizontal->getRegion().height;
        }

        if ((_scrollState & SCROLL_VERTICAL) == SCROLL_VERTICAL)
        {
            _scrollBarTopCap = getImage("scrollBarTopCap", _state);
            _scrollBarVertical = getImage("verticalScrollBar", _state);
            _scrollBarBottomCap = getImage("scrollBarBottomCap", _state);
        
            _viewportClipBounds.width -= _scrollBarVertical->getRegion().width;
        }

        GP_ASSERT(_layout);
        _layout->update(this, elapsedTime);

        if (_scrollState != SCROLL_NONE)
            this->updateScroll(this, elapsedTime);
    }
}

void Form::draw()
{
    /*
    The first time a form is drawn, its contents are rendered into a framebuffer.
    The framebuffer will only be drawn into again when the contents of the form change.

    If this form has a node then it's a 3D form and the framebuffer will be used
    to texture a quad.  The quad will be given the same dimensions as the form and
    must be transformed appropriately by the user, unless they call setQuad() themselves.

    On the other hand, if this form has not been set on a node, SpriteBatch will be used
    to render the contents of the frambuffer directly to the display.
    */

    // Check whether this form has changed since the last call to draw()
    // and if so, render into the framebuffer.
    if (isDirty())
    {
        GP_ASSERT(_frameBuffer);
        _frameBuffer->bind();

        Game* game = Game::getInstance();
        Rectangle prevViewport = game->getViewport();
        game->setViewport(Rectangle(0, 0, _bounds.width, _bounds.height));

        GP_ASSERT(_theme);
        _theme->setProjectionMatrix(_projectionMatrix);
        draw(_theme->getSpriteBatch(), _viewportClipBounds);
        _theme->setProjectionMatrix(_defaultProjectionMatrix);

        // Rebind the default framebuffer and game viewport.
        FrameBuffer::bindDefault();

        // restore the previous game viewport
        game->setViewport(prevViewport);
    }

    if (_node)
    {
        GP_ASSERT(_quad);
        _quad->draw();
    }
    else
    {
        if (!_spriteBatch)
        {
            _spriteBatch = SpriteBatch::create(_frameBuffer->getRenderTarget()->getTexture());
            GP_ASSERT(_spriteBatch);
        }

        _spriteBatch->begin();
        _spriteBatch->draw(_bounds.x, _bounds.y, 0, _bounds.width, _bounds.height, 0, _v1, _u2, 0, Vector4::one());
        _spriteBatch->end();
    }
}

void Form::draw(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    GP_ASSERT(spriteBatch);

    std::vector<Control*>::const_iterator it;

    // Batch each font individually.
    std::set<Font*>::const_iterator fontIter;
    for (fontIter = _theme->_fonts.begin(); fontIter != _theme->_fonts.end(); fontIter++)
    {
        Font* font = *fontIter;
        if (font)
        {
            font->begin();
        }
    }

    // Batch for all themed border and background sprites.
    spriteBatch->begin();

    Container::draw(spriteBatch, Rectangle(0, 0, _bounds.width, _bounds.height), _skin == NULL, _bounds.height);

    // Done all batching.
    spriteBatch->end();

    for (fontIter = _theme->_fonts.begin(); fontIter != _theme->_fonts.end(); fontIter++)
    {
        Font* font = *fontIter;
        if (font)
        {
            font->end();
        }
    }
}

void Form::initializeQuad(Mesh* mesh)
{
    // Release current model.
    SAFE_RELEASE(_quad);

    // Create the model.
    _quad = Model::create(mesh);

    // Create the material.
    Material* material = _quad->setMaterial("res/shaders/textured.vsh", "res/shaders/textured.fsh");
    GP_ASSERT(material);

    // Set the common render state block for the material.
    GP_ASSERT(_theme);
    GP_ASSERT(_theme->getSpriteBatch());
    RenderState::StateBlock* stateBlock = _theme->getSpriteBatch()->getStateBlock();
    GP_ASSERT(stateBlock);
    stateBlock->setDepthWrite(true);
    material->setStateBlock(stateBlock);

    // Bind the WorldViewProjection matrix.
    material->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);

    // Bind the texture.
    Texture::Sampler* sampler = Texture::Sampler::create(_frameBuffer->getRenderTarget()->getTexture());
    GP_ASSERT(sampler);
    sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    material->getParameter("u_diffuseTexture")->setValue(sampler);
    material->getParameter("u_diffuseColor")->setValue(Vector4::one());

    SAFE_RELEASE(sampler);
}

bool Form::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    // Check for a collision with each Form in __forms.
    // Pass the event on.
    std::vector<Form*>::const_iterator it;
    for (it = __forms.begin(); it < __forms.end(); it++)
    {
        Form* form = *it;
        GP_ASSERT(form);

        if (form->isEnabled())
        {
            Node* node = form->_node;
            if (node)
            {
                Scene* scene = node->getScene();
                GP_ASSERT(scene);
                Camera* camera = scene->getActiveCamera();

                if (camera)
                {
                    // Get info about the form's position.
                    Matrix m = node->getMatrix();
                    Vector3 min(0, 0, 0);
                    m.transformPoint(&min);

                    // Unproject point into world space.
                    Ray ray;
                    camera->pickRay(Game::getInstance()->getViewport(), x, y, &ray);

                    // Find the quad's plane.
                    // We know its normal is the quad's forward vector.
                    Vector3 normal = node->getForwardVectorWorld();

                    // To get the plane's distance from the origin,
                    // we'll find the distance from the plane defined
                    // by the quad's forward vector and one of its points
                    // to the plane defined by the same vector and the origin.
                    const float& a = normal.x; const float& b = normal.y; const float& c = normal.z;
                    const float d = -(a*min.x) - (b*min.y) - (c*min.z);
                    const float distance = abs(d) /  sqrt(a*a + b*b + c*c);
                    Plane plane(normal, -distance);

                    // Check for collision with plane.
                    float collisionDistance = ray.intersects(plane);
                    if (collisionDistance != Ray::INTERSECTS_NONE)
                    {
                        // Multiply the ray's direction vector by collision distance
                        // and add that to the ray's origin.
                        Vector3 point = ray.getOrigin() + collisionDistance*ray.getDirection();

                        // Project this point into the plane.
                        m.invert();
                        m.transformPoint(&point);

                        // Pass the touch event on.
                        const Rectangle& bounds = form->getBounds();
                        if (form->getState() == Control::FOCUS ||
                            (evt == Touch::TOUCH_PRESS &&
                                point.x >= bounds.x &&
                                point.x <= bounds.x + bounds.width &&
                                point.y >= bounds.y &&
                                point.y <= bounds.y + bounds.height))
                        {
                            if (form->touchEvent(evt, point.x - bounds.x, bounds.height - point.y - bounds.y, contactIndex))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
            else
            {
                // Simply compare with the form's bounds.
                const Rectangle& bounds = form->getBounds();
                if (form->getState() == Control::FOCUS ||
                    (evt == Touch::TOUCH_PRESS &&
                        x >= bounds.x &&
                        x <= bounds.x + bounds.width &&
                        y >= bounds.y &&
                        y <= bounds.y + bounds.height))
                {
                    // Pass on the event's position relative to the form.
                    if (form->touchEvent(evt, x - bounds.x, y - bounds.y, contactIndex))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Form::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    std::vector<Form*>::const_iterator it;
    for (it = __forms.begin(); it < __forms.end(); it++)
    {
        Form* form = *it;
        GP_ASSERT(form);
        if (form->isEnabled())
        {
            form->keyEvent(evt, key);
        }
    }
}

int Form::nextHighestPowerOfTwo(int x)
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

}
