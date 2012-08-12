#include "CubesGame.h"

// Declare our game instance
CubesGame game;

Effect * _flat;
VertexAttribute _aPosition;
VertexAttribute _aColor;
Uniform * _uTransform;
VertexAttributeBinding * _bindPosition;
Font * _font;
float _angle;
bool _freeze;

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
float_buffer _square, _color;

CubesGame::CubesGame()
{
}

char const * shaderVertex = ""
    "attribute vec4 aPosition;"
    "attribute vec4 aColor;"
    "uniform mat4 uTransform;"
    "varying vec4 vColor;"
    "void main() {"
    "  gl_Position = uTransform * aPosition;"
    "  vColor = aColor;"
    "}"
    ;
char const * shaderFragment = ""
    "varying vec4 vColor;"
    "void main()"
    "{"
    "   gl_FragColor = vColor;"
    "}"
    ;
    
void CubesGame::initialize()
{
    _font = Font::create("res/arial40.gpb");
    _flat = Effect::createFromSource( shaderVertex, shaderFragment );
    
    _aPosition = _flat->getVertexAttribute( "aPosition" );
    GP_ASSERT(_aPosition != -1 );
    _aColor = _flat->getVertexAttribute( "aColor" );
    GP_ASSERT(_aColor != -1 );
    _uTransform = _flat->getUniform( "uTransform" );
    GP_ASSERT(_uTransform);
    
    //perhaps optimize this later
    _square
        (-1,-1,1)(1,-1,1)(-1,1,1)(1,1,1) //front
        (1,-1,1)(1,-1,-1)(1,1,1)(1,1,-1) //right
        (-1,-1,-1)(1,-1,-1)(-1,1,-1)(1,1,-1) //back
        (-1,1,-1)(-1,1,1)(-1,-1,-1)(-1,-1,1) //left
        (-1,-1,-1)(1,-1,-1)(-1,-1,1)(1,-1,1) //bottom
        (-1,1,1)(1,1,1)(-1,1,-1)(1,1,-1);  //top
        
    _color
        (1,0,0)(1,0,0)(1,0,0)(1,0,0)
        (0,1,0)(0,1,0)(0,1,0)(0,1,0)
        (0,0,1)(0,0,1)(0,0,1)(0,0,1)
        (1,0,1)(1,0,1)(1,0,1)(1,0,1)
        (1,1,0)(1,1,0)(1,1,0)(1,1,0)
        (1,1,1)(1,1,1)(1,1,1)(1,1,1);
     

//     VertexFormat::Element e( VertexFormat::POSITION, 3 );
//     VertexFormat vf( &e, 1 );
//     _bindPosition = VertexAttributeBinding::create(vf,&_square.data[0],_flat);
    
    _angle = 0;
    _freeze = false;
}

void CubesGame::finalize()
{
    SAFE_RELEASE(_flat);
    SAFE_RELEASE(_font);
}

void CubesGame::update(float elapsedTime)
{
    //convert to seconds
    elapsedTime /= 1000;
    
    if( _freeze )
        return;
        
    _angle += elapsedTime;
}

void CubesGame::render(float elapsedTime)
{
    //convert to seconds
    elapsedTime /= 1000;
    
    //TODO: where in the framework does this?
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glDepthMask( true );
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4(0.30,0.30,0.25,1.0), 1.0f, 0);

    _flat->bind();

    //_bindPosition->setVertexAttribPointer(_vPosition, 3, GL_FLOAT, false, 0,  &_square.data[0] );
    //_bindPosition->bind();
    glEnableVertexAttribArray(_aPosition);
    glVertexAttribPointer(_aPosition, 3, GL_FLOAT, false, 0, &_square.data[0] );
    glEnableVertexAttribArray(_aColor);
    glVertexAttribPointer(_aColor, 3, GL_FLOAT, false, 0, &_color.data[0] );
    
    //_flat->setValue( _uColor, Vector4(0,1.0,0.5,1.0) );

    Matrix trans = Matrix::identity();
    trans.rotate(Vector3(1,1,0), _angle);
    trans.scale(0.25);

    Matrix proj;
    float ratio = float(getWidth())/getHeight();
    Matrix::createOrthographicOffCenter(-ratio,ratio,-1,1,1,3,&proj);
    
    Matrix look;
    Matrix::createLookAt( Vector3(0,0,-2)/*eye*/, Vector3(0,0,0)/*center*/, Vector3(0,1,0)/*up*/, &look );
    
    trans = (proj * look) * trans;

    _flat->setValue( _uTransform, trans );

    for( int i=0; i < 6; ++i )
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4 );
    
    //-limited to ASCII
    //-drawText is in device coordinates
    char buffer[64];
    sprintf( buffer,"%d FPS", int(1.0/elapsedTime) );
    _font->start();
    _font->drawText(buffer, 5, 5, Vector4(1.0,1.0,1.0,1.0), _font->getSize() );
    _font->finish();
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
            _freeze = true;
            break;
        case Touch::TOUCH_RELEASE:
            _freeze = false;
            break;
        case Touch::TOUCH_MOVE:
            break;
    };
}
