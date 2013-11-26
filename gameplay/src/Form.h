#ifndef FORM_H_
#define FORM_H_

#include "Ref.h"
#include "Container.h"
#include "Mesh.h"
#include "Node.h"
#include "FrameBuffer.h"
#include "Touch.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

namespace gameplay
{

class Theme;

/**
 * Top-level container of UI controls.  The following properties are available for forms:

 @verbatim
    form <formID>
    {
        // Form properties.
        theme       = <Path to .theme File> // See Theme.h.
        layout      = <Layout::Type>        // A value from the Layout::Type enum.  e.g.: LAYOUT_VERTICAL
        style       = <styleID>             // A style from the referenced theme.
        position   = <x, y>                // Position of the form on-screen, measured in pixels.
        alignment  = <Control::Alignment>  // Note: 'position' will be ignored.
        autoWidth  = <bool>                // Will result in a form the width of the display.
        autoHeight = <bool>                // Will result in a form the height of the display.
        size       = <width, height>       // Size of the form, measured in pixels.
        width      = <width>               // Can be used in place of 'size', e.g. with 'autoHeight = true'
        height     = <height>              // Can be used in place of 'size', e.g. with 'autoWidth = true'
        consumeEvents = <bool>             // Whether the form propagates input events to the Game's input event handler. Default is false
      
        // All the nested controls within this form.
        container { }
        label { }
        textBox { }
        button { }
        checkBox { }
        radioButton { }
        slider { }
    }
 @endverbatim
 */
class Form : public Container
{
    friend class Platform;
    friend class Game;
    friend class Gamepad;
    friend class Control;
    friend class Container;

public:

    /**
     * Creates a form using the data from the Properties object defined at the specified URL, 
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional). 
     * 
     * @param url The URL pointing to the Properties object defining the form. 
     * @script{create}
     */
    static Form* create(const char* url);

    /**
     * Create a new Form.
     *
     * @param id The Form's ID.
     * @param style The Form's style.
     * @param layoutType The form's layout type.
     *
     * @return The new Form.
     * @script{create}
     */
    static Form* create(const char* id, Theme::Style* style, Layout::Type layoutType = Layout::LAYOUT_ABSOLUTE);

    /**
     * Get a form from its ID.
     *
     * @param id The ID of the form to search for.
     *
     * @return A form with the given ID, or null if one was not found.
     */
    static Form* getForm(const char* id);
    
    /**
     * Returns the current UI control that is in focus.
     *
     * @return The current control in focus, or NULL if no controls are in focus.
     */
    static Control* getFocusControl();

    /**
     * Removes focus from any currently focused UI control.
     */
    static void clearFocus();

    /**
     * @see Container#isForm()
     */
    bool isForm() const;

    /**
     * Gets the theme for the form.
     *
     * @return The theme for the form.
     */
    Theme* getTheme() const;

    /**
     * Attach this form to a node.
     *
     * A form can be drawn as part of the 3-dimensional world if it is attached to a node.
     * The form's contents will be rendered into a framebuffer which will be used to texture a quad.
     * This quad will be given the same dimensions as the form and must be transformed appropriately.
     * Alternatively, a quad can be set explicitly on a form with the setQuad() methods.
     *
     * @param node The node to attach this form to.
     */
    void setNode(Node* node);

    /**
     * Updates each control within this form, and positions them according to its layout.
     */
    void update(float elapsedTime);

    /**
     * Draws this form.
     */
    unsigned int draw();

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Returns the single currently active control within the UI system.
     *
     * @return The currently active control, or NULL if no controls are currently active.
     */
    static Control* getActiveControl();

protected:

    /**
     * @see Control::update
     */
    void update(const Control* container, const Vector2& offset);

private:
    
    /**
     * Constructor.
     */
    Form();

    /**
     * Constructor.
     */
    Form(const Form& copy);

    /**
     * Destructor.
     */
    virtual ~Form();

    /**
     * Initialize a quad for this form in order to draw it in 3D.
     *
     * @param mesh The mesh to create a model from.
     */
    void initializeQuad(Mesh* mesh);

    /**
     * Updates all visible, enabled forms.
     */
    static void updateInternal(float elapsedTime);

    /**
     * Propagate touch events to enabled forms.
     *
     * @return Whether the touch event was consumed by a form.
     */
    static bool touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Propagate key events to enabled forms.
     *
     * @return Whether the key event was consumed by a form.
     */
    static bool keyEventInternal(Keyboard::KeyEvent evt, int key);

    /**
     * Propagate mouse events to enabled forms.
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    static bool mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Propagate gamepad events to enabled forms.
     *
     * @see Control::gamepadEvent
     */
    static bool gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Fired by the platform when the game window resizes.
     *
     * @param width The new window width.
     * @param height The new window height.
     */
    static void resizeEventInternal(unsigned int width, unsigned int height);

    /**
     * Get the next highest power of two of an integer.  Used when creating framebuffers.
     *
     * @param x The number to start with.
     *
     * @return The next highest power of two after x, or x if it is already a power of two.
     */
    static unsigned int nextPowerOfTwo(unsigned int x);

    /**
     * Unproject a point (from a mouse or touch event) into the scene and then project it onto the form.
     *
     * @param x The x coordinate of the mouse/touch point.
     * @param y The y coordinate of the mouse/touch point.
     * @param point A destination vector to populate with the projected point, in the form's plane.
     *
     * @return True if the projected point lies within the form's plane, false otherwise.
     */
    bool projectPoint(int x, int y, Vector3* point);

    /**
     * Called when the form is resized to update its internal frame buffer.
     */
    void updateFrameBuffer();

    static bool pointerEventInternal(bool mouse, int evt, int x, int y, int param);

    static Control* findInputControl(int* x, int* y, bool focus);

    static Control* findInputControl(Control* control, int x, int y, bool focus);

    static Control* handlePointerPressRelease(int* x, int* y, bool pressed);

    static Control* handlePointerMove(int* x, int* y);

    static bool screenToForm(Control* ctrl, int* x, int* y);

    static void verifyRemovedControlState(Control* control);

    static void controlDisabled(Control* control);

    static void setFocusControl(Control* control);

    static void pollGamepads();

    static bool pollGamepad(Gamepad* gamepad);

    Theme* _theme;                      // The Theme applied to this Form.
    FrameBuffer* _frameBuffer;          // FBO the Form is rendered into for texturing the quad. 
    SpriteBatch* _spriteBatch;
    Node* _node;                        // Node for transforming this Form in world-space.
    Model* _nodeQuad;                   // Quad for rendering this Form in 3d space.
    Material* _nodeMaterial;            // Material for rendering this Form in 3d space.
    float _u2;
    float _v1;
    Matrix _projectionMatrix;           // Orthographic projection matrix to be set on SpriteBatch objects when rendering into the FBO.
    static Control* _focusControl;
    static Control* _activeControl;
    static Control::State _activeControlState;
};

}

#endif
