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
Vector3 _eye;
float _eyeAzimuth, _eyeInclination;
float _eyeDist;

void updateEye()
{
    _eye = Vector3(
        _eyeDist * sin( _eyeInclination ) * cos( _eyeAzimuth ),
        _eyeDist * sin( _eyeInclination ) * sin( _eyeAzimuth ),
        _eyeDist * cos( _eyeInclination )
        );
}

//I'm keeping this high enough to slow the framerate on my system, looking for optimizations
int _grid = 9;
float _scale = 0.25;
float _disperse = 1.5;

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
    // fake light to show distance a bit
    "   float d = 1.0 - gl_FragCoord.z;"
    //"   float d = 1.0;"
    "   gl_FragColor = vec4(vColor.x*d,vColor.y*d,vColor.z*d,1.0);"
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

    _eyeInclination = M_PI/6;
    _eyeAzimuth = M_PI/8;
    _eyeDist = 2;
    updateEye();
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
    //glEnable( GL_CULL_FACE ); //TODO: some of our faces wind incorrectly
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

    float cell = _disperse / _grid;
    Matrix rot = Matrix::identity();
    rot.rotate(Vector3(1,1,0), _angle);
    rot.scale(cell*_scale);

    //TODO: This should be done only on size change, what is the interface for this?
    Matrix proj;
    float ratio = float(getWidth())/getHeight();
    
    float zNear = std::max(0.1, _eyeDist-1.5);
    float zFar = zNear+3;
    Matrix::createOrthographicOffCenter(-ratio,ratio,-1,1,zNear,zFar,&proj);
    //Matrix::createFrustum(-ratio/2,ratio/2,-0.5,0.5, zNear, zFar, &proj);
    
    Matrix look;
    Matrix::createLookAt( _eye, Vector3(0,0,0)/*center*/, Vector3(0,1,0)/*up*/, &look );
    
    Matrix view = proj * look;

    Matrix trans;
    for( int x=0; x < _grid; ++x )
    {
        float tx = (-0.5*_disperse) + (x+0.5) * cell;
        for( int y=0; y < _grid; ++y )
        {
            float ty = (-0.5*_disperse) + (y+0.5) * cell;
            for( int z=0; z < _grid; ++z )
            {
                float tz = (-0.5*_disperse) + (z+0.5)*cell;
                trans.setIdentity();
                trans.translate(tx,ty,tz);
                _flat->setValue( _uTransform, view * (trans * rot) );
                for( int i=0; i < 6; ++i )
                    glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4 );
            }
        }
    }

    //-limited to ASCII
    //-drawText is in device coordinates
    char buffer[128];
    sprintf( buffer,"%d FPS\n%d Cubes", getFrameRate(), _grid*_grid*_grid );
    _font->start();
    _font->drawText(buffer, 5, 5, Vector4(1.0,1.0,1.0,1.0), _font->getSize() );
    _font->finish();
}

void CubesGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    //TODO: This is causing double-strokes, CHAR and RELEASE don't work (Linux platform)
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
            case Keyboard::KEY_ESCAPE:
                exit();
                break;
                
            case Keyboard::KEY_KP_PLUS:
                _grid++;
                break;
                
            case Keyboard::KEY_KP_MINUS:
                if( _grid > 1 )
                    _grid--;
                break;
        }
    }
}

int _lastX, _lastY;

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
        {
            int deltaX = x - _lastX;
            int deltaY = y - _lastY;
            if( _freeze )
            {
                //this isn't really what we want, it moves the camera within a sphere (we want relative movement)
                _eyeInclination += deltaY / 50.0;
                _eyeAzimuth -= deltaX / 50.0;
                updateEye();
            }
            break;
        }
    };
    
    _lastX = x;
    _lastY = y;
}

bool CubesGame::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch(evt)
    {
        case Mouse::MOUSE_WHEEL:
            _scale *= pow( 0.5, -wheelDelta );
            return true;
    }
    return false;
}
