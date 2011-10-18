/*
 * Color.cpp
 */

#include "Base.h"
#include "Color.h"

namespace gameplay
{

Color::Color()
    : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
{
}

Color::Color(float red, float green, float blue, float alpha)
{
    set(red, green, blue, alpha);
}

Color::Color(const Color& copy)
{
    set(copy);
}

Color::~Color()
{
}

void Color::set(const Color& color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    a = color.a;
}

void Color::set(float red, float green, float blue, float alpha)
{
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

const Color& Color::white()
{
    static Color white(1, 1, 1, 1);
    return white;
}

const Color& Color::black()
{
    static Color black(0, 0, 0, 1);
    return black;
}

const Color& Color::red()
{
    static Color red(1, 0, 0, 1);
    return red;
}

const Color& Color::green()
{
    static Color green(0, 1, 0, 1);
    return green;
}

const Color& Color::blue()
{
    static Color blue(0, 0, 1, 1);
    return blue;
}

const Color& Color::orange()
{
    static Color orange(1, 0.4f, 0, 1);
    return orange;
}

const Color& Color::yellow()
{
    static Color yellow(1, 1, 0, 1);
    return yellow;
}

const Color& Color::empty()
{
    static Color empty(0, 0, 0, 0);
    return empty;
}

const Color& Color::transparent()
{
    static Color transparent(0, 0, 0, 0);
    return transparent;
}

Color& Color::operator = (const Color& color)
{
    memcpy(&r, &color.r, COLOR_SIZE);
    return (*this);
}

bool Color::operator == (const Color& color) const
{
    return (memcmp(&r, &color.r, COLOR_SIZE) == 0);
}

bool Color::operator != (const Color& color) const
{
    return (memcmp(&r, &color.r, COLOR_SIZE) != 0);
}

}
