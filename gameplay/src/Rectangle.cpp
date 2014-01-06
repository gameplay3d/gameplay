#include "Base.h"
#include "Rectangle.h"

namespace gameplay
{

Rectangle::Rectangle()
    : x(0), y(0), width(0), height(0)
{
}

Rectangle::Rectangle(float width, float height) :
    x(0), y(0), width(width), height(height)
{
}

Rectangle::Rectangle(float x, float y, float width, float height) :
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

void Rectangle::set(float x, float y, float width, float height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Rectangle::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Rectangle::left() const
{
    return x;
}

float Rectangle::top() const
{
    return y;
}

float Rectangle::right() const
{
    return x + width;
}

float Rectangle::bottom() const
{
    return y + height;
}

bool Rectangle::contains(float x, float y) const
{
    return (x >= this->x && x <= (this->x + width) && y >= this->y && y <= (this->y + height));
}

bool Rectangle::contains(float x, float y, float width, float height) const
{
    return (contains(x, y) && contains(x + width, y + height));
}

bool Rectangle::contains(const Rectangle& r) const
{
    return contains(r.x, r.y, r.width, r.height);
}

bool Rectangle::intersects(float x, float y, float width, float height) const
{
    float t;
    if ((t = x - this->x) > this->width || -t > width)
        return false;
    if ((t = y - this->y) > this->height || -t > height)
        return false;
    return true;
}

bool Rectangle::intersects(const Rectangle& r) const
{
    return intersects(r.x, r.y, r.width, r.height);
}

bool Rectangle::intersect(const Rectangle& r1, const Rectangle& r2, Rectangle* dst)
{
    GP_ASSERT(dst);

    float xmin = max(r1.x, r2.x);
    float xmax = min(r1.right(), r2.right());
    if (xmax > xmin)
    {
        float ymin = max(r1.y, r2.y);
        float ymax = min(r1.bottom(), r2.bottom());
        if (ymax > ymin)
        {
            dst->set(xmin, ymin, xmax - xmin, ymax - ymin);
            return true;
        }
    }

    dst->set(0, 0, 0, 0);
    return false;
}

void Rectangle::combine(const Rectangle& r1, const Rectangle& r2, Rectangle* dst)
{
    GP_ASSERT(dst);

    dst->x = min(r1.x, r2.x);
    dst->y = min(r1.y, r2.y);
    dst->width = max(r1.x + r1.width, r2.x + r2.width) - dst->x;
    dst->height = max(r1.y + r1.height, r2.y + r2.height) - dst->y;
}

void Rectangle::inflate(float horizontalAmount, float verticalAmount)
{
    x -= horizontalAmount;
    y -= verticalAmount;
    width += horizontalAmount * 2;
    height += verticalAmount * 2;
}

Rectangle& Rectangle::operator = (const Rectangle& r)
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
