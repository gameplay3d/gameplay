#include "Base.h"
#include "Viewport.h"

namespace gameplay
{

Viewport::Viewport()
{
}

Viewport::Viewport(int x, int y, int width, int height)
{
    set(x, y, width, height);
}

Viewport::Viewport(const Viewport& viewport)
{
    set(viewport);
}

Viewport::~Viewport()
{
}

void Viewport::set(int x, int y, int width, int height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

void Viewport::set(const Viewport& viewport)
{
    _x = viewport._x;
    _y = viewport._y;
    _width = viewport._width;
    _height = viewport._height;
}

int Viewport::getX() const
{
    return _x;
}

void Viewport::setX(int x)
{
    _x = x;
}

int Viewport::getY() const
{
    return _y;
}

void Viewport::setY(int y)
{
    _y = y;
}

int Viewport::getWidth() const
{
    return _width;
}

void Viewport::setWidth(int width)
{
    _width = width;
}

int Viewport::getHeight() const
{
    return _height;
}

void Viewport::setHeight(int height)
{
    _height = height;
}

void Viewport::bind()
{
    GL_ASSERT( glViewport(_x, _y, _width, _height) );
}

}
