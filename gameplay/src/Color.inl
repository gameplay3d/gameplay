#include "Color.h"

namespace gameplay
{
	inline const float& Color::operator[](int index) const {
		return components[index];
	}

	inline bool Color::operator==(const Color& color) const
	{
		return r == color.r && g == color.g && b == color.b && a == color.a;
	}

	inline bool Color::operator!=(const Color& color) const
	{
		return r != color.r || g != color.g || b != color.b || a != color.a;
	}

}
