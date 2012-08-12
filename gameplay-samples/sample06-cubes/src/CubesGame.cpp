#include "CubesGame.h"

// Declare our game instance
CubesGame game;

Effect * _flat;
VertexAttribute _vPosition;
Uniform* _uColor;
VertexAttributeBinding * _bindPosition;

/**
    A simple way to construct vertex arrays.
*/
struct float_buffer
{
    std::vector<float> data;
    
    void clear()
    {
        data.clear();
    }
    
    
    /** 
        The () operators allow easy construction of vertices.
    */
    float_buffer& operator()( float a )
    { 
        data.push_back( a );
        return *this;
    }
    
    float_buffer& operator()( float a, float b )
    { 
        data.push_back( a );
        data.push_back( b );
        return *this;
    }
    
    float_buffer& operator()( float a, float b, float c )
    { 
        data.push_back( a );
        data.push_back( b );
        data.push_back( c );
        return *this;
    }
    
    float_buffer& operator()( float a, float b, float c, float d )
    { 
        data.push_back( a );
        data.push_back( b );
        data.push_back( c );
        data.push_back( d );
        return *this;
    }
};
float_buffer _square;

CubesGame::CubesGame()
{
}

char const * shaderVertex = ""
    "attribute vec4 vPosition;"
    "void main() {"
    "  gl_Position = vPosition;"
    "}"
    ;
char const * shaderFragment = ""
    "uniform vec4 uColor;"
    "void main()"
    "{"
    "   gl_FragColor = uColor;"
    "}"
    ;
    
void CubesGame::initialize()
{
    _flat = Effect::createFromSource( shaderVertex, shaderFragment );
    
    _vPosition = _flat->getVertexAttribute( "vPosition" );
    GP_ASSERT(_vPosition != -1 );
    _uColor = _flat->getUniform( "uColor" );
    GP_ASSERT(_uColor);
    
    _square( -0.5, -0.5, 0 )
        ( -0.5, 0.5, 0 )
        ( 0.5, 0.5, 0 )
        ( 0.5, -0.5, 0 );

    VertexFormat::Element e( VertexFormat::POSITION, 3 );
    VertexFormat vf( &e, 1 );
    _bindPosition = VertexAttributeBinding::create(vf,&_square.data[0],_flat);
}

void CubesGame::finalize()
{
    SAFE_RELEASE(_flat);
}

void CubesGame::update(float elapsedTime)
{
}

void CubesGame::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4(0.30,0.30,0.25,1.0), 1.0f, 0);

    _flat->bind();

    //_bindPosition->setVertexAttribPointer(_vPosition, 3, GL_FLOAT, false, 0,  &_square.data[0] );
    //_bindPosition->bind();
    glEnableVertexAttribArray(_vPosition);
    glVertexAttribPointer(_vPosition, 3, GL_FLOAT, false, 0, &_square.data[0] );
    
    _flat->setValue( _uColor, Vector4(0,1.0,0.5,1.0) );
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4 );
}

void CubesGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void CubesGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        break;
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}
