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
     * Draws the specified text.
     *
     * @param text The text to draw.
     * @param x The viewport x position to draw text at.
     * @param y The viewport y position to draw text at.
     * @param color The color of text.
     */
    void drawText(const char* text, int x, int y, const Vector4& color);

    /**
     * Ends text drawing for this font.
     */
    void end();

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
