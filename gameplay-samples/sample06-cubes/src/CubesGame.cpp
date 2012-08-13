#include "CubesGame.h"

// Declare our game instance
CubesGame game;

CubesGame::CubesGame()
{
    _flat = 0;
    _uTransform = 0;
    _bindPosition = 0;
    _font = 0;
    _form = 0;
    _sliderNumCubes = 0;
    _sliderScale = 0;
    _sliderDisperse =0;
    _checkOrthoView = 0;
    _checkUseBuffers = 0;
    _checkSingleMesh = 0;
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
        
    //though it might seem using bytes would lower bandwidth, somehow it slows everything down...
    _color
        (1,0,0)(1,0.25,0.25)(1,0.5,0.5)(1,0.75,0.75)
        (0,1,0)(0.25,1,0.25)(0.5,1,0.5)(0.75,1,0.75)
        (0,0,1)(0.25,0.25,1)(0.5,0.5,1)(0.75,0.75,1)
        (1,0,1)(1,0.25,1)(1,0.5,1)(1,0.75,1)
        (1,1,0)(1,1,0.25)(1,1,0.5)(1,1,0.75)
        (1,1,1)(1,1,1)(1,1,1)(1,1,1);

    //the versions which are a single mesh of triangles
    _squareComplete
        (-1,-1,1)(1,-1,1)(-1,1,1) (1,-1,1)(-1,1,1)(1,1,1) //front
        (1,-1,1)(1,-1,-1)(1,1,1) (1,-1,-1)(1,1,1)(1,1,-1) //right
        (-1,-1,-1)(1,-1,-1)(-1,1,-1) (1,-1,-1)(-1,1,-1)(1,1,-1) //back
        (-1,1,-1)(-1,1,1)(-1,-1,-1) (-1,1,1)(-1,-1,-1)(-1,-1,1) //left
        (-1,-1,-1)(1,-1,-1)(-1,-1,1) (1,-1,-1)(-1,-1,1)(1,-1,1) //bottom
        (-1,1,1)(1,1,1)(-1,1,-1) (1,1,1)(-1,1,-1)(1,1,-1);  //top
        
    _colorComplete
        (1,0,0)(1,0.25,0.25)(1,0.5,0.5) (1,0.25,0.25)(1,0.5,0.5)(1,0.75,0.75)
        (0,1,0)(0.25,1,0.25)(0.5,1,0.5) (0.25,1,0.25)(0.5,1,0.5)(0.75,1,0.75)
        (0,0,1)(0.25,0.25,1)(0.5,0.5,1) (0.25,0.25,1)(0.5,0.5,1)(0.75,0.75,1)
        (1,0,1)(1,0.25,1)(1,0.5,1) (1,0.25,1)(1,0.5,1)(1,0.75,1)
        (1,1,0)(1,1,0.25)(1,1,0.5) (1,1,0.25)(1,1,0.5)(1,1,0.75)
        (1,1,1)(1,1,1)(1,1,1) (1,1,1)(1,1,1)(1,1,1);
        
        
     //create vertex buffers in GL
     GLuint buffers[4];
     glGenBuffers(4,buffers);
     _bufSquare = buffers[0];
     _bufColor = buffers[1];
     _bufSquareComplete = buffers[2];
     _bufColorComplete = buffers[3];
     
     glBindBuffer(GL_ARRAY_BUFFER,_bufSquare);
     glBufferData(GL_ARRAY_BUFFER,_square.byte_size(),&_square.data[0],GL_STATIC_DRAW);
     
     glBindBuffer(GL_ARRAY_BUFFER,_bufColor);
     glBufferData(GL_ARRAY_BUFFER,_color.byte_size(),&_color.data[0],GL_STATIC_DRAW);

     glBindBuffer(GL_ARRAY_BUFFER,_bufSquareComplete);
     glBufferData(GL_ARRAY_BUFFER,_squareComplete.byte_size(),&_squareComplete.data[0],GL_STATIC_DRAW);
     
     glBindBuffer(GL_ARRAY_BUFFER,_bufColorComplete);
     glBufferData(GL_ARRAY_BUFFER,_colorComplete.byte_size(),&_colorComplete.data[0],GL_STATIC_DRAW);
     
    _angle = 0;
    _freeze = false;

    _eyeInclination = M_PI/6;
    _eyeAzimuth = M_PI/8;
    _eyeDist = 2;

    _form = Form::create("res/editor.form");
    _form->setConsumeInputEvents(false);
    
    _sliderNumCubes = (Slider*)_form->getControl("numCubes");
    _sliderScale = (Slider*)_form->getControl("scale");
    _sliderDisperse = (Slider*)_form->getControl("disperse");
    _checkOrthoView = (CheckBox*)_form->getControl("orthoView");
    _checkUseBuffers = (CheckBox*)_form->getControl("useBuffers");
    _checkSingleMesh = (CheckBox*)_form->getControl("singleMesh");
    
    _sliderNumCubes->addListener(this, Listener::VALUE_CHANGED);
    _sliderScale->addListener(this, Listener::VALUE_CHANGED);
    _checkOrthoView->addListener(this, Listener::VALUE_CHANGED);
    _sliderDisperse->addListener(this, Listener::VALUE_CHANGED);
    _checkUseBuffers->addListener(this, Listener::VALUE_CHANGED);
    _checkSingleMesh->addListener(this, Listener::VALUE_CHANGED);

    _showForm = true;
    //use whatever the form has set as defaults
    readForm();
    
    updateEye();
}

void CubesGame::readForm()
{
    setProjMatrix( _checkOrthoView->isChecked() );
    _disperse = _sliderDisperse->getValue();
    _scale = _sliderScale->getValue();
    _grid = (int)_sliderNumCubes->getValue();
    _useBuffers = _checkUseBuffers->isChecked();
    _singleMesh = _checkSingleMesh->isChecked();
}

void CubesGame::finalize()
{
    SAFE_RELEASE(_flat);
    SAFE_RELEASE(_font);
    SAFE_RELEASE(_bindPosition);
    SAFE_RELEASE(_form);
    
    SAFE_RELEASE(_sliderNumCubes);
    SAFE_RELEASE(_sliderScale);
    SAFE_RELEASE(_sliderDisperse);
    SAFE_RELEASE(_checkOrthoView);
}

void CubesGame::update(float elapsedTime)
{
    if( _showForm )
        _form->update(elapsedTime);
    
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
    //glEnable( GL_CULL_FACE ); //some of our faces wind incorrectly, so we can't enable this
    
    clear(CLEAR_COLOR_DEPTH, Vector4(0.30,0.30,0.25,1.0), 1.0f, 0);

    _flat->bind();

    glEnableVertexAttribArray(_aPosition);
    glEnableVertexAttribArray(_aColor);
    
    if( _useBuffers )
    {
        glBindBuffer(GL_ARRAY_BUFFER, _singleMesh ? _bufSquareComplete : _bufSquare );
        glVertexAttribPointer(_aPosition, 3, GL_FLOAT, false, 0, 0 );
    
        glBindBuffer(GL_ARRAY_BUFFER, _singleMesh ? _bufColorComplete : _bufColor );
        glVertexAttribPointer(_aColor, 3, GL_FLOAT, false, 0, 0 );
    }
    else
    {
        glVertexAttribPointer(_aPosition, 3, GL_FLOAT, false, 0, 
            _singleMesh ? &_squareComplete.data[0] : &_square.data[0] );
        glVertexAttribPointer(_aColor, 3, GL_FLOAT, false, 0, 
            _singleMesh ? &_colorComplete.data[0] : & _color.data[0] );
    }
    
    float cell = _disperse / _grid;
    Matrix rot = Matrix::identity();
    rot.rotate(Vector3(1,1,0), _angle);
    rot.scale(cell*_scale);

    //the calculation time in this loop becomes significant at 27K cubes
    //accounting for a significant fraction of the FPS drop
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
                trans = matView  * (trans * rot);
                
                _flat->setValue( _uTransform, trans );
                if( _singleMesh )
                    glDrawArrays(GL_TRIANGLES,0,6*6);
                else
                {
                    for( int i=0; i < 6; ++i )
                        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4 );
                }
            } //z
        } //y
    } //x

    //draw on top
    glDepthFunc( GL_ALWAYS );
    glDepthMask( false );
    
    //-limited to ASCII
    //-drawText is in device coordinates
    char buffer[128];
    sprintf( buffer,"%d FPS\n%d Cubes", getFrameRate(), _grid*_grid*_grid );
    _font->start();
    _font->drawText(buffer, 5, 5, Vector4(1.0,1.0,1.0,1.0), _font->getSize() );
    _font->finish();
    
    if( _showForm )
        _form->draw();
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
            //TODO: key is 0 for space! (Linux)
            case ' ':
                print( "TOGGLE: %d\n", _showForm);
                _showForm = !_showForm;
                break;
            //just because toggling can't work due to defect
            case 'z':
                _showForm = false;
                break;
            case 'x':
                _showForm = true;
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
    }
    return false;
}

void CubesGame::controlEvent(Control* control, EventType evt)
{
    switch( evt )
    {
        case Listener::VALUE_CHANGED:
            readForm();
            updateEye();
            break;
    }
}

void CubesGame::updateEye()
{
    _eye = Vector3(
        _eyeDist * sin( _eyeInclination ) * cos( _eyeAzimuth ),
        _eyeDist * sin( _eyeInclination ) * sin( _eyeAzimuth ),
        _eyeDist * cos( _eyeInclination )
        );

    Matrix::createLookAt( _eye, Vector3(0,0,0)/*center*/, Vector3(0,1,0)/*up*/, &matLook );
    
    matView = matProj * matLook;
}

void CubesGame::setProjMatrix( bool ortho )
{
    float ratio = float(getWidth())/getHeight();

    float zNear = std::max(0.1, _eyeDist-1.5);
    float zFar = zNear+3;
    
    if( ortho )
        Matrix::createOrthographicOffCenter(-ratio,ratio,-1,1,zNear,zFar,&matProj);
    else
        Matrix::createFrustum(-ratio/2,ratio/2,-0.5,0.5, zNear, zFar, &matProj);
}
