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
    friend class TextBox;

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
     * Returns the font size (max height of glyphs) in pixels.
     */
    unsigned int getSize();

    /**
     * Begins text drawing for this font.
     */
    void begin();

    /**
     * Ends text batching for this font and renders all drawn text.
     */
    void end();

    /**
     * Draws the specified text in a solid color, with a scaling factor.
     *
     * @param text The text to draw.
     * @param x The viewport x position to draw text at.
     * @param y The viewport y position to draw text at.
     * @param color The color of text.
     * @param size The size to draw text (0 for default size).
     */
    void drawText(const char* text, int x, int y, const Vector4& color, unsigned int size = 0, bool rightToLeft = false);

    /**
     * Draws the specified text within a rectangular area, with a specified alignment and scale.
     * Clips text outside the viewport.  Optionally wraps text to fit within the width of the viewport.
     *
     * @param text The text to draw.
     * @param area The viewport area to draw within.  Text will be clipped outside this rectangle.
     * @param color The color of text.
     * @param size The size to draw text (0 for default size).
     * @param justify Justification of text within the viewport.
     * @param wrap Wraps text to fit within the width of the viewport if true.
     * @param rightToLeft Whether to draw text from right to left.
     * @param clip A region to clip text within after applying justification to the viewport area.
     */
    void drawText(const char* text, const Rectangle& area, const Vector4& color,
        unsigned int size = 0, Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool rightToLeft = false, const Rectangle* clip = NULL);

    /**
     * Measures a string's width and height without alignment, wrapping or clipping.
     *
     * @param text The text to measure.
     * @param size
     * @param width Destination for the text's width.
     * @param height Destination for the text's height.
     */
    void measureText(const char* text, unsigned int size, unsigned int* widthOut, unsigned int* heightOut);

    /**
     * Measures a string's bounding box after alignment, wrapping and clipping within a viewport.
     *
     * @param text The text to measure.
     * @param out Destination rectangle to store the bounds in.
     * @param viewport The viewport area to align, wrap and clip text within while measuring.
     * @param scale The scaling factor to apply.
     * @param justify Justification of text within the viewport.
     * @param wrap Whether to measure text with wrapping applied.
     * @param ignoreClip Whether to clip 'out' to the viewport.  Set false for the bounds of what would actually be drawn
     *                within the given viewport; true for bounds that are guaranteed to fit the entire string of text.
     */
    void measureText(const char* text, const Rectangle& clip, unsigned int size, Rectangle* out,
                     Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool ignoreClip = false);

    // Get an index into a string corresponding to the character nearest the given location within the clip region.
    unsigned int getIndexAtLocation(const char* text, const Rectangle& clip, unsigned int size, const Vector2& inLocation, Vector2* outLocation,
                                    Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool rightToLeft = false);

    // Get the location of the character at the given index.
    void getLocationAtIndex(const char* text, const Rectangle& clip, unsigned int size, Vector2* outLocation, const unsigned int destIndex,
                            Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool rightToLeft = false);

    SpriteBatch* getSpriteBatch();

    static Justify getJustifyFromString(const char* justify);


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

    // Used by both getIndexAtLocation and getLocationAtIndex.
    unsigned int getIndexOrLocation(const char* text, const Rectangle& clip, unsigned int size, const Vector2& inLocation, Vector2* outLocation,
                                    const int destIndex = -1, Justify justify = ALIGN_TOP_LEFT, bool wrap = true, bool rightToLeft = false);

    // Utilities
    unsigned int getTokenWidth(const char* token, unsigned int length, unsigned int size, float scale);
    unsigned int getReversedTokenLength(const char* token, const char* bufStart);

    // Returns 0 if EOF was reached, 1 if delimiters were handles correctly, and 2 if the stopAtPosition was reached while handling delimiters.
    int handleDelimiters(const char** token, const unsigned int size, const int iteration, const int areaX, int* xPos, int* yPos, unsigned int* lineLength,
                          std::vector<int>::const_iterator* xPositionsIt, std::vector<int>::const_iterator xPositionsEnd, unsigned int* charIndex = NULL,
                          const Vector2* stopAtPosition = NULL, const int currentIndex = -1, const int destIndex = -1);
    void addLineInfo(const Rectangle& area, int lineWidth, int lineLength, Justify hAlign,
                     std::vector<int>* xPositions, std::vector<unsigned int>* lineLengths, bool rightToLeft);

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
