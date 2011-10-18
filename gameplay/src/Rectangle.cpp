/*
 * Rectangle.cpp
 */

#include "Base.h"
#include "Rectangle.h"

namespace gameplay
{

Rectangle::Rectangle()
    : x(0), y(0), width(0), height(0)
{
}

Rectangle::Rectangle(int width, int height) :
    x(0), y(0), width(width), height(height)
{
}

Rectangle::Rectangle(int x, int y, int width, int height) :
    x(x), y(y), width(width), height(height)
{
}

Rectangle::Rectangle(const Rectangle& copy)
{
    set(copy);
}

Rectangle::~Rectangle()
{
}

const Rectangle& Rectangle::empty()
{
    static Rectangle empty;
    return empty;
}

bool Rectangle::isEmpty() const
{
    return (x == 0 && y == 0 && width == 0 && height == 0);
}

void Rectangle::set(const Rectangle& r)
{
    set(r.x, r.y, r.width, r.height);
}

void Rectangle::set(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

int Rectangle::left() const
{
    return x;
}

int Rectangle::top() const
{
    return y;
}

int Rectangle::right() const
{
    return x + width;
}

int Rectangle::bottom() const
{
    return y + height;
}

bool Rectangle::contains(int x, int y) const
{
    return (x >= x && x <= (x + width) && y >= y && y <= (y + height));
}

bool Rectangle::contains(int x, int y, int width, int height) const
{
    return (contains(x, y) && contains(x + width, y + height));
}

bool Rectangle::contains(const Rectangle& r) const
{
    return contains(r.x, r.y, r.width, r.height);
}

bool Rectangle::intersects(int x, int y, int width, int height) const
{
    const int left   = fmaxf(static_cast<int>(this->x), x);
    const int top    = fmaxf(this->y, y);
    const int right  = fminf(x + width, x + width);
    const int bottom = fminf(y + height, y + height);

    return (right > left && bottom > top);
}

bool Rectangle::intersects(const Rectangle& r) const
{
    return intersects(r.x, r.y, r.width, r.height);
}

void Rectangle::combine(const Rectangle& r1, const Rectangle& r2, Rectangle* dst)
{
    dst->x = fminf(r1.x, r2.x);
    dst->y = fminf(r1.y, r2.y);
    dst->width = fmaxf(r1.x + r1.width, r2.x + r2.width) - dst->x;
    dst->height = fmaxf(r1.y + r1.height, r2.y + r2.height) - dst->y;
}

void Rectangle::inflate(int horizontalAmount, int verticalAmount)
{
    x -= horizontalAmount;
    y -= verticalAmount;
    width += horizontalAmount * 2;
    height += verticalAmount * 2;
}

const Rectangle& Rectangle::operator = (const Rectangle& r)
{
    x = r.x;
    y = r.y;
    width = r.width;
    height = r.height;
    return *this;
}

bool Rectangle::operator == (const Rectangle& r) const
{
    return (x == r.x && width == r.width && y == r.y && height == r.height);
}

bool Rectangle::operator != (const Rectangle& r) const
{
    return (x != r.x || width != r.width || y != r.y || height != r.height);
}

}
