#ifndef COLOR_H_
#define COLOR_H_

#include "Vector4.h"

namespace gameplay
{

/**
* Defines a color object with a R, G, B and A components with values between 0.0 and 1.0.
* Methods and operations to work with colors.
*/
class Color
{
public:
	/**
	* Color components
	*/
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float components[4];
	};

	/**
	* Constructs a new color initialized to all zeros.
	*/
	Color();

	/**
	* Constructs a new color initialized to the specified values.
	*
	* @param r The r coordinate.
	* @param g The g coordinate.
	* @param b The b coordinate.
	* @param a The a coordinate.
	*/
	Color(float r, float g, float b, float a = 1.0f);

	/**
	* Constructs a new color initializing r, g, b and a to the specified values.
	*
	* @param value The r, g, b and a components.
	*/
	Color(float value);

	/**
	* Constructs a new color from a 32-bit ARGB value specified color.
	*
	* @param copy The Color to copy.
	*/
	Color(unsigned int argb);

	/**
	* Constructs a new vector from the values in the specified array.
	*
	* @param array An array containing the elements of the vector in the order x, y, z, w.
	*/
	Color(const float* array);

	/**
	* Constructs a new color that is a copy of the specified color.
	*
	* @param copy The Color to copy.
	*/
	Color(const Color& copy);

	/**
	* Constructs a new color that is a copy of the specified Vector4.
	*
	* @param copy The Vector4 to copy.
	*/
	Color(const Vector4& copy);

	/**
	* Destructor.
	*/
	~Color();

	/**
	* Sets the color components to the specified values.
	*
	* @param r The new r coordinate.
	* @param g The new g coordinate.
	* @param b The new b coordinate.
	* @param a The new a coordinate.
	*/
	void set(float r, float g, float b, float a = 1.0f);

	/**
	* Sets the color components to the specified value.
	*
	* @param value The value to set r,g,b and a.
	*/
	void set(float value);

	/**
	* Sets the color components to those in the specified color.
	*
	* @param color The color to copy.
	*/
	void set(const Color& color);

	/**
	* Sets the color components to those in the specified vector4.
	*
	* @param vec The vector to copy.
	*/
	void set(const Vector4& vec);

	/**
	* Returns a 32-bit value (ARGB) from the r, g, b and a components.
	*
	* @return 32-bit value (ARGB) corresponding to the color components.
	*/
	unsigned int get32() const;

	/**
	* Get the hue component of the color.
	*
	* @return The hue component from 0.0 to 1.0.
	*/
	float getHue() const;

	/**
	* Get the saturation component of the color.
	*
	* @return The saturation component from 0.0 to 1.0
	*/
	float getSaturation() const;

	/**
	* Get the value component of the color.
	*
	* @return The value component from 0.0 to 1.0
	*/
	float getValue() const;

	/**
	* Set color rgba from the specified hsv and alpha values.
	*
	* @param h The r coordinate.
	* @param s The g coordinate.
	* @param v The b coordinate.
	* @param a The a coordinate.
	*/
	void setHSV(float hue, float saturation, float value, float alpha = 1.0f);

	/**
	* Mix this color with a given color through a factor
	*
	* @param color The color to mix
	* @param factor The value to use to interpolate between colors
	*/
	void mix(const Color& color, float factor);

	/**
	* Set this color to grayscale
	*/
	void grayscale();

	/**
	* Invert the color
	*/
	void invert();

	/**
	* Return R, G, B or A componemt through array indexing
	*
	* @param index The array index matching the color component.
	*
	* @return The color componemt matching the index
	*/
	inline const float& operator[](int index) const;

	/**
	* Determines if this color is equal to the color vector.
	*
	* @param color The color to compare against.
	*
	* @return True if this color is equal to the given color, false otherwise.
	*/
	inline bool operator==(const Color& color) const;

	/**
	* Determines if this color is not equal to the color vector.
	*
	* @param color The color to compare against.
	*
	* @return True if this color is not equal to the given color, false otherwise.
	*/
	inline bool operator!=(const Color& color) const;
};

}

#include "Color.inl"

#endif