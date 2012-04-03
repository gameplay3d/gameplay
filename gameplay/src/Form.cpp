#include "Base.h"
#include "Form.h"
#include "AbsoluteLayout.h"
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

    Form::Form() : _theme(NULL), _quad(NULL), _node(NULL), _frameBuffer(NULL)
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

        // Remove this Form from the global list.
        std::vector<Form*>::iterator it = std::find(__forms.begin(), __forms.end(), this);
        if (it != __forms.end())
        {
            __forms.erase(it);
        }
    }

    Form* Form::create(const char* path)
    {
        // Load Form from .form file.
        assert(path);

        Properties* properties = Properties::create(path);
        assert(properties);
        if (properties == NULL)
            return NULL;

        // Check if the Properties is valid and has a valid namespace.
        Properties* formProperties = properties->getNextNamespace();
        assert(formProperties);
        if (!formProperties || !(strcmp(formProperties->getNamespace(), "form") == 0))
        {
            SAFE_DELETE(properties);
            return NULL;
        }

        // Create new form with given ID, theme and layout.
        const char* themeFile = formProperties->getString("theme");
        const char* layoutString = formProperties->getString("layout");
        Form* form = Form::create(themeFile, getLayoutType(layoutString));

        Theme* theme = form->_theme;
        const char* styleName = formProperties->getString("style");
        form->init(theme->getStyle(styleName), formProperties);

        // Add all the controls to the form.
        form->addControls(theme, formProperties);

        SAFE_DELETE(properties);

        return form;
    }

    Form* Form::create(const char* themeFile, Layout::Type type)
    {
        Layout* layout;
        switch (type)
        {
        case Layout::LAYOUT_ABSOLUTE:
            layout = AbsoluteLayout::create();
            break;
        case Layout::LAYOUT_FLOW:
            break;
        case Layout::LAYOUT_VERTICAL:
            layout = VerticalLayout::create();
            break;
        }

        assert(themeFile);
        Theme* theme = Theme::create(themeFile);
        assert(theme);

        Form* form = new Form();
        form->_layout = layout;
        form->_theme = theme;

        __forms.push_back(form);

        return form;
    }

    Form* Form::getForm(const char* id)
    {
        std::vector<Form*>::const_iterator it;
        for (it = __forms.begin(); it < __forms.end(); it++)
        {
            Form* f = *it;
            if (strcmp(id, f->getID()) == 0)
            {
                return f;
            }
        }
        
        return NULL;
    }

    void Form::setQuad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
    {
        Mesh* mesh = Mesh::createQuad(p1, p2, p3, p4);
        initializeQuad(mesh);
        SAFE_RELEASE(mesh);
    }

    void Form::setQuad(float x, float y, float width, float height)
    {
        Mesh* mesh = Mesh::createQuad(x, y, width, height);
        initializeQuad(mesh);
        SAFE_RELEASE(mesh);
    }

    void Form::setNode(Node* node)
    {
        _node = node;

        if (_node && !_quad)
        {
            // Set this Form up to be 3D by initializing a quad, projection matrix and viewport.
            setQuad(0.0f, 0.0f, _size.x, _size.y);

            Matrix::createOrthographicOffCenter(0, _size.x, _size.y, 0, 0, 1, &_projectionMatrix);
            _theme->setProjectionMatrix(_projectionMatrix);
            
            _node->setModel(_quad);
        }
    }

    void Form::update()
    {
        Container::update(Rectangle(0, 0, _size.x, _size.y));
    }

    void Form::draw()
    {
        // If this form has a node then it's a 3D form.  The contents will be rendered
        // into a framebuffer which will be used to texture a quad.  The quad will be
        // given the same dimensions as the form and must be transformed appropriately
        // by the user, unless they call setQuad() themselves.

        // On the other hand, if this form has not been set on a node it will render
        // directly to the display.

        if (_node)
        {
            // Check whether this form has changed since the last call to draw()
            // and if so, render into the framebuffer.
            if (isDirty())
            {
                _frameBuffer->bind();

				Game* game = Game::getInstance();
				Rectangle prevViewport = game->getViewport();
                
				game->setViewport(Rectangle(_position.x, _position.y, _size.x, _size.y));

                draw(_theme->getSpriteBatch(), _clip);

                // Rebind the default framebuffer and game viewport.
                FrameBuffer::bindDefault();

				// restore the previous game viewport
				game->setViewport(prevViewport);
            }

            _quad->draw();
        }
        else
        {
            draw(_theme->getSpriteBatch(), _clip);
        }
    }

    void Form::draw(SpriteBatch* spriteBatch, const Rectangle& clip)
    {
        std::vector<Control*>::const_iterator it;

        // Batch all themed border and background sprites.
        spriteBatch->begin();

        // Draw the form's border and background.
        // We don't pass the form's position to itself or it will be applied twice!
        Control::drawBorder(spriteBatch, Rectangle(0, 0, _size.x, _size.y));

        // Draw each control's border and background.
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;

            //if (!_node || (*it)->isDirty())
            {
                control->drawBorder(spriteBatch, clip);

                // Add all themed foreground sprites (checkboxes etc.) to the same batch.
                control->drawImages(spriteBatch, clip);
            }
        }
        spriteBatch->end();

        // Draw all control foregrounds / text.
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;

            //if (!_node || (*it)->isDirty())
            {
                control->drawText(clip);
            }
        }

        _dirty = false;
    }

    void Form::initializeQuad(Mesh* mesh)
    {
        // Release current model.
        SAFE_RELEASE(_quad);

        // Create the model
        _quad = Model::create(mesh);

        // Create the material
        Material* material = _quad->setMaterial("res/shaders/textured.vsh", "res/shaders/textured.fsh");

        // Set the common render state block for the material
        RenderState::StateBlock* stateBlock = _theme->getSpriteBatch()->getStateBlock();
        stateBlock->setDepthWrite(true);
        //material->setStateBlock(_theme->getSpriteBatch()->getStateBlock());
        material->setStateBlock(stateBlock);

        // Bind the WorldViewProjection matrix
        material->setParameterAutoBinding("u_worldViewProjectionMatrix", RenderState::WORLD_VIEW_PROJECTION_MATRIX);

        // Create a FrameBuffer if necessary.
        if (!_frameBuffer)
        {
            _frameBuffer = FrameBuffer::create(_id.c_str());
        }

        // Use the FrameBuffer to texture the quad.
        if (!_frameBuffer->getRenderTarget())
        {
            RenderTarget* rt = RenderTarget::create(_id.c_str(), _size.x, _size.y);
            _frameBuffer->setRenderTarget(rt);
            SAFE_RELEASE(rt);
        }

        Texture::Sampler* sampler = Texture::Sampler::create(_frameBuffer->getRenderTarget()->getTexture());
        sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
        material->getParameter("u_texture")->setValue(sampler);
        material->getParameter("u_textureRepeat")->setValue(Vector2::one());
        material->getParameter("u_textureTransform")->setValue(Vector2::zero());

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

            if (form->isEnabled())
            {
                Node* node = form->_node;
                if (node)
                {
                    Scene* scene = node->getScene();
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
            if (form->isEnabled())
            {
                form->keyEvent(evt, key);
            }
        }
    }
}