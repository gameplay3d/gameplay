#ifndef FORM_H_
#define FORM_H_

#include "Ref.h"
#include "Container.h"
#include "Mesh.h"
#include "Node.h"
#include "FrameBuffer.h"
#include "Touch.h"
#include "Keyboard.h"

namespace gameplay
{

class Theme;

class Form : public Container
{
    friend class Platform;

public:
    /**
     * Create from .form file.
     */
    static Form* create(const char* path);
    static Form* create(const char* id, const char* textureFile, Layout::Type type);
    static Form* getForm(const char* id);

    void setTheme(Theme* theme);
    Theme* getTheme() const;

    /**
     * Create a 3D quad to texture with this Form.
     */
    void setQuad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

    /**
     * Create a 2D quad to texture with this Form.
     */
    void setQuad(float x, float y, float width, float height);

    void setNode(Node* node);

    void update();
    void draw();

private:
    Form();
    Form(const Form& copy);
    virtual ~Form();

    void initQuad(Mesh* mesh);
    void draw(SpriteBatch* spriteBatch);
    void draw(SpriteBatch* spriteBatch, const Vector2& position);

    static void touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    static void keyEventInternal(Keyboard::KeyEvent evt, int key);

    Theme* _theme;              // The Theme applied to this Form.
    Model* _quad;               // Quad for rendering this Form in world-space.
    Node* _node;                // Node for transforming this Form in world-space.
    FrameBuffer* _frameBuffer;  // FBO the Form is rendered into for texturing the quad.
    Matrix _projectionMatrix;   // Orthographic projection matrix to be set on SpriteBatch objects when rendering into the FBO.
    Viewport* _viewport;        // Viewport for setting before rendering into the FBO.
};

}

#endif