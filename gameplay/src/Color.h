/*
 * Color.h
 */

#ifndef COLOR_H_
#define COLOR_H_

namespace gameplay
{

#define COLOR_SIZE sizeof(float) * 4

/**
 * Defines a color made up of red, green, blue, and alpha components.
 *
 * Note: The alpha value defaults to 1.0 (opaque).
 */
class Color
{
public:

    /**
     * The red component of the color. The range is 0.0f - 1.0f. The default is 0.0f.
     */
    float r;
    /**
     * The green component of the color. The range is 0.0f - 1.0f. The default is 0.0f.
     */
    float g;
    /**
     * The blue component of the color. The range is 0.0f - 1.0f. The default is 0.0f.
     */
    float b;
    /**
     * The alpha component of the color. The range is 0.0f - 1.0f. The default is 1.0f.
     */
    float a;

    /**
     * Constructs a new color initialized to solid (opaque) black.
     */
    Color();

    /**
     * Constructs a new color initialized to the specified values.
     *
     * @param red The red component.
     * @param green The green component
     * @param blue The blue component.
     * @param alpha The alpha component.
     */
    Color(float red, float green, float blue, float alpha = 1.0f);

    /**
     * Construct a new color from a copy.
     *
     * @param copy The copy of the color.
     */
    Color(const Color& copy);

    /**
     * Destructor.
     */
    ~Color();

    /**
     * Sets the color.
     *
     * @param red The red component.
     * @param green The green component.
     * @param blue The blue component.
     * @param alpha The alpha component. Default is 1.0f.
     */
    void set(float red, float green, float blue, float alpha = 1.0f);

    /**
     * Constructs a new color from the given color.
     *
     * @param color The color to copy.
     */
    void set(const Color& color);

    /**
     * The color white.
     *
     * @return rgba(1, 1, 1, 1).
     */
    static const Color& white();

    /**
     * The color black.
     *
     * @return rgba(0, 0, 0, 0).
     */
    static const Color& black();

    /**
     * The color red.
     *
     * @return rgba(1, 0, 0, 1).
     */
    static const Color& red();

    /**
     * The color green.
     *
     * @return green(0, 1, 0, 1).
     */
    static const Color& green();

    /**
     * The color blue.
     *
     * @return blue(0, 0, 1, 1).
     */
    static const Color& blue();

    /**
     * The color orange.
     *
     * @return orange(1, 0.4f, 0, 1).
     */
    static const Color& orange();

    /**
     * The color yellow.
     *
     * @return yellow(1, 1, 0, 1).
     */
    static const Color& yellow();

    /**
     * The color empty.
     *
     * @return empty(0, 0, 0, 0).
     */
    static const Color& empty();

    /**
     * The color transparent. Same as empty().
     *
     * @return transparent(0, 0, 0, 0).
     */
    static const Color& transparent();

    /**
     * operator =
     */
    Color& operator = (const Color& color);

    /**
     * operator ==
     */
    bool operator == (const Color& color) const;

    /**
     * operator !=
     */
    bool operator != (const Color& color) const;
};

}

#endif
