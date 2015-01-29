#include "Base.h"
#include "Color.h"

namespace gameplay
{

Color::Color()
	: r(0.0f), g(0.0f), b(0.0f), a(0.0f)
{
}

Color::Color(float r, float g, float b, float a)
	: r(r), g(g), b(b), a(a)
{
}

Color::Color(float value)
	: r(value), g(value), b(value), a(value)
{
}

Color::Color(unsigned int argb)
{
	float a = (argb & 0xFF) / 255.0;
	argb >>= 8;
	float b = (argb & 0xFF) / 255.0;
	argb >>= 8;
	float g = (argb & 0xFF) / 255.0;
	argb >>= 8;
	float r = (argb & 0xFF) / 255.0;

	Color(r, g, b, a);
}

Color::Color(const float* array)
{
	GP_ASSERT(array);
	memcpy(components, array, 4);
}

Color::Color(const Color& copy)
{
	this->r = copy.r;
	this->g = copy.g;
	this->b = copy.b;
	this->a = copy.a;
}

Color::Color(const Vector4& copy)
{
	r = copy.x;
	g = copy.y;
	b = copy.z;
	a = copy.w;
}

Color::~Color()
{
}

void Color::set(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void Color::set(float value)
{
	r = value;
	g = value;
	b = value;
	a = value;
}

void Color::set(const Color& color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

void Color::set(const Vector4& vec)
{
	r = vec.x;
	g = vec.y;
	b = vec.z;
	a = vec.w;
}

unsigned int Color::get32() const
{
	unsigned int argb;
	argb = (unsigned int)(a * 255);
	argb <<= 8;
	argb |= (unsigned int)(r * 255);
	argb <<= 8;
	argb |= (unsigned int)(g * 255);
	argb <<= 8;
	argb |= (unsigned int)(b * 255);

	return argb;
}

float Color::getHue() const
{
	float vmin = min(r, g);
	vmin = min(vmin, b);
	float vmax = max(r, g);
	vmax = max(vmax, b);

	float delta = vmax - vmin;

	if (delta == 0)
		return 0;

	float h;
	if (r == vmax)
		h = (g - b) / delta;		// between yellow & magenta
	else if (g == vmax)
		h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		h = 4 + (r - g) / delta;	// between magenta & cyan

	h /= 6.0;
	if (h<0)
		h += 1.0;

	return h;
}

float Color::getSaturation() const
{
	float vmin = min(r, g);
	vmin = min(vmin, b);
	float vmax = max(r, g);
	vmax = max(vmax, b);

	float delta = vmax - vmin;

	return (vmax != 0) ? (delta / vmax) : 0;
}

float Color::getValue() const
{
	float vmax = max(r, g);
	vmax = max(vmax, b);

	return vmax;
}

void Color::setHSV(float hue, float saturation, float value, float alpha)
{
	int i;
	float f, p, q, t;

	a = alpha;

	if (saturation == 0)
	{
		r = g = b = value;
		return;
	}

	hue *= 6.0;
	i = floor(hue);
	f = hue - i;
	p = value * (1 - saturation);
	q = value * (1 - saturation * f);
	t = value * (1 - saturation * (1 - f));

	switch (i)
	{
	case 0:
		r = value;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = value;
		b = p;
		break;
	case 2:
		r = p;
		g = value;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = value;
		break;
	case 4:
		r = t;
		g = p;
		b = value;
		break;
	default:
		r = value;
		g = p;
		b = q;
		break;
	}
}

void Color::mix(const Color& color, float factor)
{
	r += factor * (color.r - r);
	g += factor * (color.g - g);
	b += factor * (color.b - b);
	a += factor * (color.a - a);
}

void Color::grayscale()
{
	float g = (r * 0.3) + (g * 0.59) + (b * 0.11);
	set(g, g, g, a);
}

void Color::invert()
{
	r = 1.0 - r;
	g = 1.0 - g;
	b = 1.0 - b;
}

}