/*
 * Font.h
 */

#ifndef FONT_H_
#define FONT_H_

#include "SpriteBatch.h"

namespace gameplay
{

/**
 * Defines a font for text rendering.
 */
class Font : public Ref
{
    friend class Package;

public:

    /**
     * Defines the set of allowable font styles.
     */
    enum Style
    {
        PLAIN = 0,
        BOLD = 1,
        ITALIC = 2,
        BOLD_ITALIC = 4
    };

    /**
     * Defines the set of allowable alignments when drawing text.
     */
    enum Justify
    {
        // Specify horizontal alignment, use default vertical alignment (ALIGN_TOP).
        ALIGN_LEFT = 0x01,
        ALIGN_HCENTER = 0x02,
        ALIGN_RIGHT = 0x04,
    
        // Specify vertical alignment, use default horizontal alignment (ALIGN_LEFT).
        ALIGN_TOP = 0x10,
        ALIGN_VCENTER = 0x20,
        ALIGN_BOTTOM = 0x40,

        // Specify both vertical and horizontal alignment.
        ALIGN_TOP_LEFT = ALIGN_TOP | ALIGN_LEFT,
        ALIGN_VCENTER_LEFT = ALIGN_VCENTER | ALIGN_LEFT,
        ALIGN_BOTTOM_LEFT = ALIGN_BOTTOM | ALIGN_LEFT,
        ALIGN_TOP_HCENTER = ALIGN_TOP | ALIGN_HCENTER,
        ALIGN_VCENTER_HCENTER = ALIGN_VCENTER | ALIGN_HCENTER,
        ALIGN_BOTTOM_HCENTER = ALIGN_BOTTOM | ALIGN_HCENTER,
        ALIGN_TOP_RIGHT = ALIGN_TOP | ALIGN_RIGHT,
        ALIGN_VCENTER_RIGHT = ALIGN_VCENTER | ALIGN_RIGHT,
        ALIGN_BOTTOM_RIGHT = ALIGN_BOTTOM | ALIGN_RIGHT
    };

    /**
     * Defines a font glyph within the texture map for a font.
     */
    class Glyph
    {
    public:
        /**
         * Glyph character code (decimal value).
         */
        unsigned int code;

        /**
         * Glyph width (in pixels).
         */
        unsigned int width;

        /**
         * Glyph texture coordinates.
         */
        float uvs[4];
    };

    /**
     * Creates a font from the given package.
     *
     * If the 'id' parameter is NULL, it is assumed that the Package at 'path'
     * contains exactly one Font resource. If the Package does not meet this criteria,
     * NULL is returned.
     *
     * If a font for the given path has already been loaded, the existing font will be
     * returned with its reference count increased.
     *
     * @param path The path to a package file containing a font resource.
     * @param id An optional ID of the font resource within the package (NULL for the first/only resource).
     * 
     * @return The specified font.
     */
    static Font* create(const char* path, const char* id = NULL);

    /**
     * Creates a font with the given characteristics from the specified glyph array and texture map.
     *
     * This method will create a new Font object regardless of whether another Font is already
     * created with the same attributes.
     *
     * @param family The font family name.
     * @param style The font style.
     * @param size The font size.
     * @param glyphs An array of font glyphs, defining each character in the font within the texture map.
     * @param glyphCount The number of items in the glyph array.
     * @param texture A texture map containing rendered glyphs.
     * 
     * @return The new Font.
     */
    static Font* create(const char* family, Style style, unsigned int size, Glyph* glyphs, int glyphCount, Texture* texture);

    /**
     * Begins text drawing for this font.
     */
    void begin();

    /**
     * Draws the specified text in a solid color, with a scaling factor.
     *
     * @param text The text to draw.
     * @param x The viewport x position to draw text at.
     * @param y The viewport y position to draw text at.
     * @param color The color of text.
     * @param scale The scaling factor.
     */
    void drawText(const char* text, int x, int y, const Vector4& color, float scale = 1.0f, bool rightToLeft = false);

    /**
     * Draws the specified text within a rectangular area, with a specified alignment and scale.
     * Clips text outside the viewport.  Optionally wraps text to fit within the width of the viewport.
     *
     * @param text The text to draw.
     * @param viewport The viewport area to draw within.  Text starts from the top-left of this rectangle.
     * @param color The color of text.
     * @param scale The text's scaling factor.
     * @param justify Justification of text within the viewport.
     * @param wrap Wraps text to fit within the width of the viewport if true.
     */
    void drawText(const char* text, const Rectangle& viewport, const Vector4& color, float scale = 1.0f,
                  Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool rightToLeft = false);

    /**
     * Ends text drawing for this font.
     */
    void end();

    /**
     * Measures a string's width and height without alignment, wrapping or clipping.
     *
     * @param text The text to measure.
     * @param width Destination for the text's width.
     * @param height Destination for the text's height.
     */
    void measureText(const char* text, unsigned int* width, unsigned int* height, float scale = 1.0f);

    /**
     * Measures a string's bounding box after alignment, wrapping and clipping within a viewport.
     *
     * @param text The text to measure.
     * @param out Destination rectangle to store the bounds in.
     * @param viewport The viewport area to align, wrap and clip text within while measuring.
     * @param scale The scaling factor to apply.
     * @param justify Justification of text within the viewport.
     * @param wrap Whether to measure text with wrapping applied.
     * @param clipped Whether to clip 'out' to the viewport.  Set true for the bounds of what would actually be drawn
     *                within the given viewport; false for bounds that are guaranteed to fit the entire string of text.
     */
    void measureText(const char* text, Rectangle* out, const Rectangle& viewport,
                     float scale = 1.0f, Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool clipped = true);


private:

    /**
     * Constructor.
     */
    Font();

    /**
     * Copy constructor.
     */
    Font(const Font& copy);

    /**
     * Destructor.
     */
    ~Font();

    // Utilities
    unsigned int getTokenWidth(const char* token, unsigned int length, float scale);
    void reverseLines(char* text);

    std::string _path;
    std::string _id;
    std::string _family;
    Style _style;
    unsigned int _size;
    Glyph* _glyphs;
    unsigned int _glyphCount;
    Texture* _texture;
    SpriteBatch* _batch;
};

}

#endif
