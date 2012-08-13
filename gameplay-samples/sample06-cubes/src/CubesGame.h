#ifndef TEMPLATEGAME_H_
#define TEMPLATEGAME_H_

#include "gameplay.h"
#include "float_buffer.h"

using namespace gameplay;

/**
 * Main game class.
 */
class CubesGame: public Game, Control::Listener
{
public:

    /**
     * Constructor.
     */
    CubesGame();

	void keyEvent(Keyboard::KeyEvent evt, int key);
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    void controlEvent(Control* control, EventType evt);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:
    //data buffers
    data_buffer<GLfloat> _square;
    data_buffer<GLfloat> _color;
    
    GLuint _bufSquare, _bufColor;
    
    //dimensions of the grid
    int _grid;
    //scale of the cubes relative to their cell size
    float _scale;
    //spacing of the cells
    float _disperse;
    
    Effect * _flat;
    VertexAttribute _aPosition;
    VertexAttribute _aColor;
    Uniform * _uTransform;
    VertexAttributeBinding * _bindPosition;
    Font * _font;
    float _angle;
    bool _freeze;
    Vector3 _eye;
    float _eyeAzimuth, _eyeInclination;
    float _eyeDist;
    Form * _form;
    bool _showForm;
    bool _useBuffers;
    
    Slider * _sliderNumCubes;
    Slider * _sliderScale;
    Slider * _sliderDisperse;
    CheckBox * _checkOrthoView;
    CheckBox * _checkUseBuffers;
    
    Matrix matProj, matLook, matView;
    void setProjMatrix( bool ortho );
    void updateEye();
    
    void readForm();
};

#endif
