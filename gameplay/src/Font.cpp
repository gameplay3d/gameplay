/*
 * Font.cpp
 */

#include "Base.h"
#include "Font.h"
#include "Game.h"
#include "FileSystem.h"
#include "Package.h"

// Default font vertex shader
#define FONT_VSH \
    "uniform mat4 u_projectionMatrix;\n" \
    "attribute vec3 a_position;\n" \
    "attribute vec2 a_texcoord;\n" \
    "attribute vec4 a_color;\n" \
    "varying vec2 v_texcoord;\n" \
    "varying vec4 v_color;\n" \
    "void main()\n" \
    "{\n" \
        "gl_Position = u_projectionMatrix * vec4(a_position, 1);\n" \
        "v_texcoord = a_texcoord;\n" \
        "v_color = a_color;\n" \
    "}\n"

// Default font fragment shader
#define FONT_FSH \
    "#ifdef OPENGL_ES\n" \
    "precision highp float;\n" \
    "#endif\n" \
    "varying vec2 v_texcoord;\n" \
    "varying vec4 v_color;\n" \
    "uniform sampler2D u_texture;\n" \
    "void main()\n" \
    "{\n" \
        "gl_FragColor = v_color;\n" \
        "gl_FragColor.a = texture2D(u_texture, v_texcoord).a;\n" \
    "}"

namespace gameplay
{

static std::vector<Font*> __fontCache;

static Effect* __fontEffect = NULL;

Font::Font() :
    _style(PLAIN), _size(0), _glyphs(NULL), _glyphCount(0), _texture(NULL), _batch(NULL)
{
}

Font::Font(const Font& copy)
{
    // hidden
}

Font::~Font()
{
    // Remove this Font from the font cache.
    std::vector<Font*>::iterator itr = find(__fontCache.begin(), __fontCache.end(), this);
    if (itr != __fontCache.end())
    {
        __fontCache.erase(itr);
    }

    SAFE_DELETE(_batch);
    SAFE_DELETE_ARRAY(_glyphs);
    SAFE_RELEASE(_texture);
}

Font* Font::create(const char* path, const char* id)
{
    // Search the font cache for a font with the given path and ID.
    for (unsigned int i = 0, count = __fontCache.size(); i < count; ++i)
    {
        Font* f = __fontCache[i];
        if (f->_path == path && (id == NULL || f->_id == id))
        {
            // Found a match.
            f->addRef();
            return f;
        }
    }

    // Load the package.
    Package* pkg = Package::create(path);
    if (pkg == NULL)
    {
        return NULL;
    }

    Font* font = NULL;

    if (id == NULL)
    {
        // Get the ID of the first/only object in the package (assume it's a Font).
        const char* id;
        if (pkg->getObjectCount() != 1 || (id = pkg->getObjectID(0)) == NULL)
        {
            return NULL;
        }

        // Load the font using the ID of the first object in the package.
        font = pkg->loadFont(pkg->getObjectID(0));
    }
    else
    {
        // Load the font with the given ID.
        font = pkg->loadFont(id);
    }

    if (font)
    {
        // Add this font to the cache.
        __fontCache.push_back(font);
    }

    SAFE_RELEASE(pkg);

    return font;
}

Font* Font::create(const char* family, Style style, unsigned int size, Glyph* glyphs, int glyphCount, Texture* texture)
{
    // Create the effect for the font's sprite batch.
    if (__fontEffect == NULL)
    {
        __fontEffect = Effect::createFromSource(FONT_VSH, FONT_FSH);
        if (__fontEffect == NULL)
        {
            LOG_ERROR("Failed to create effect for font.");
            SAFE_RELEASE(texture);
            return NULL;
        }
    }
    else
    {
        __fontEffect->addRef();
    }

    // Create batch for the font.
    SpriteBatch* batch = SpriteBatch::create(texture, __fontEffect, 128);

    // Release __fontEffect since the SpriteBatch keeps a reference to it
    SAFE_RELEASE(__fontEffect);

    if (batch == NULL)
    {
        LOG_ERROR("Failed to create batch for font.");
        return NULL;
    }

    // Increase the ref count of the texture to retain it.
    texture->addRef();

    Font* font = new Font();
    font->_family = family;
    font->_style = style;
    font->_size = size;
    font->_texture = texture;
    font->_batch = batch;

    // Copy the glyphs array.
    font->_glyphs = new Glyph[glyphCount];
    memcpy(font->_glyphs, glyphs, sizeof(Glyph) * glyphCount);
    font->_glyphCount = glyphCount;

    return font;
}

void Font::begin()
{
    _batch->begin();
}

void Font::drawText(const char* text, int x, int y, const Vector4& color, float scale, bool rightToLeft)
{
    if (rightToLeft)
    {
        char* textCursor = const_cast<char*>(text);
        reverseLines(textCursor);
    }

    const int length = strlen(text);
    const int size = (int)_size * scale;
    int xPos = x, yPos = y;
    for (int i = 0; i < length; ++i)
    {
        char c = text[i];

        // Draw this character.
        switch (c)
        {
        case ' ':
            xPos += size>>1;
            break;
        case '\r':
        case '\n':
            yPos += size;
            xPos = x;
            break;
        case '\t':
            xPos += (size>>1)*4;
            break;
        default:
            unsigned int index = c - 32; // HACK for ASCII
            if (index >= 0 && index < _glyphCount)
            {
                Glyph& g = _glyphs[index];
                _batch->draw(xPos, yPos, g.width * scale, size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                xPos += g.width * scale + (size>>3);
                break;
            }
        }
    }
}

void Font::drawText(const char* text, const Rectangle& area, const Vector4& color, float scale, Justify justify, bool wrap, bool rightToLeft)
{
    char* token = const_cast<char*>(text);
    const int length = strlen(text);
    const int fontSize = (int)(_size * scale);
    int yPos = area.y;

    Justify vAlign = static_cast<Justify>(justify & 0xF0);
    if (vAlign == 0)
    {
        vAlign = ALIGN_TOP;
    }

    Justify hAlign = static_cast<Justify>(justify & 0x0F);
    if (hAlign == 0)
    {
        hAlign = ALIGN_LEFT;
    }

    if (rightToLeft)
    {
        reverseLines(token);
    }

    // For alignments other than top-left, need to calculate the y position to begin drawing from
    // and the starting x position of each line.
    std::vector<int> xPositions;
    if (vAlign != ALIGN_TOP || hAlign != ALIGN_LEFT)
    {
        token = const_cast<char*>(text);
        int lineWidth = 0;
        int delimWidth = 0;

        if (wrap)
        {
            // Go a word at a time.
            bool reachedEOF = false;
            while (token[0] != 0)
            {
                unsigned int tokenWidth = 0;

                // Handle delimiters until next token.
                char delimiter = token[0];
                while (delimiter == ' ' ||
                       delimiter == '\t' ||
                       delimiter == '\r' ||
                       delimiter == '\n' ||
                       delimiter == 0)
                {
                    switch (delimiter)
                    {
                        case ' ':
                            delimWidth += fontSize>>1;
                            break;
                        case '\r':
                        case '\n':
                            yPos += fontSize;

                            if (lineWidth > 0)
                            {
                                int hWhitespace = area.width - lineWidth;
                                if (hAlign == ALIGN_HCENTER)
                                {
                                    xPositions.push_back(area.x + hWhitespace / 2);
                                }
                                else if (hAlign == ALIGN_RIGHT)
                                {
                                    xPositions.push_back(area.x + hWhitespace);
                                }
                            }

                            lineWidth = 0;
                            delimWidth = 0;
                            break;
                        case '\t':
                            delimWidth += (fontSize>>1)*4;
                            break;
                        case 0:
                            reachedEOF = true;
                            break;
                    }

                    if (reachedEOF)
                    {
                        break;
                    }

                    token++;
                    delimiter = token[0];
                }

                if (reachedEOF || token == NULL)
                {
                    break;
                }

                unsigned int tokenLength = strcspn(token, " \r\n\t");
                tokenWidth += getTokenWidth(token, tokenLength, scale);

                // Wrap if necessary.
                if (lineWidth + tokenWidth + delimWidth > area.width)
                {
                    yPos += fontSize;

                    // Push position of current line.
                    int hWhitespace = area.width - lineWidth;

                    if (hAlign == ALIGN_HCENTER)
                    {
                        xPositions.push_back(area.x + hWhitespace / 2);
                    }
                    else if (hAlign == ALIGN_RIGHT)
                    {
                        xPositions.push_back(area.x + hWhitespace);
                    }

                    // Move token to the next line.
                    lineWidth = 0;
                    delimWidth = 0;
                }
                else
                {
                    lineWidth += delimWidth;
                    delimWidth = 0;
                }

                lineWidth += tokenWidth;
                token += tokenLength;
            }

            // Final calculation of vertical position.
            int textHeight = yPos - area.y;
            int vWhiteSpace = area.height - textHeight;
            if (vAlign == ALIGN_VCENTER)
            {
                yPos = area.y + vWhiteSpace / 2;
            }
            else if (vAlign == ALIGN_BOTTOM)
            {
                yPos = area.y + vWhiteSpace;
            }

            // Calculation of final horizontal position.
            int hWhitespace = area.width - lineWidth;
            if (hAlign == ALIGN_HCENTER)
            {
                xPositions.push_back(area.x + hWhitespace / 2);
            }
            else if (hAlign == ALIGN_RIGHT)
            {
                xPositions.push_back(area.x + hWhitespace);
            }
        }
        else
        {
            // Go a line at a time.
            while (token[0] != 0)
            {
                char delimiter = token[0];
                while (delimiter == '\n')
                {
                    yPos += fontSize;
                    ++token;
                    delimiter = token[0];
                }

                unsigned int tokenLength = strcspn(token, "\n");
                if (tokenLength == 0)
                {
                    tokenLength = strlen(token);
                }

                int lineWidth = getTokenWidth(token, tokenLength, scale);
                int whitespace = area.width - lineWidth;
                if (hAlign == ALIGN_HCENTER)
                {
                    xPositions.push_back(area.x + whitespace / 2);
                }
                else if (hAlign == ALIGN_RIGHT)
                {
                    xPositions.push_back(area.x + whitespace);
                }

                token += tokenLength;
            }

            int textHeight = yPos - area.y;
            int vWhiteSpace = area.height - textHeight;
            if (vAlign == ALIGN_VCENTER)
            {
                yPos = area.y + vWhiteSpace / 2;
            }
            else if (vAlign == ALIGN_BOTTOM)
            {
                yPos = area.y + vWhiteSpace;
            }
        }

        if (vAlign == ALIGN_TOP)
        {
            yPos = area.y;
        }
    }

    // Now we have the info we need in order to render.
    int xPos = area.x;
    std::vector<int>::const_iterator xPositionsIt = xPositions.begin();
    if (xPositionsIt != xPositions.end())
    {
        xPos = *xPositionsIt++;
    }

    token = const_cast<char*>(text);
    while (token[0] != 0)
    {
        // Handle delimiters until next token.
        char delimiter = token[0];
        bool nextLine = true;
        bool reachedEOF = false;
        while (delimiter == ' ' ||
               delimiter == '\t' ||
               delimiter == '\r' ||
               delimiter == '\n' ||
               delimiter == 0)
        {
            switch (delimiter)
            {
                case ' ':
                    xPos += fontSize>>1;
                    break;
                case '\r':
                case '\n':
                    yPos += fontSize;

                    // Only use next xPos for first newline character (in case of multiple consecutive newlines).
                    if (nextLine)
                    {
                        if (xPositionsIt != xPositions.end())
                        {
                            xPos = *xPositionsIt++;
                        }
                        else
                        {
                            xPos = area.x;
                        }
                        nextLine = false;
                    }
                    break;
                case '\t':
                    xPos += (fontSize>>1)*4;
                    break;
                case 0:
                    reachedEOF = true;
                    break;
            }

            if (reachedEOF)
            {
                break;
            }

            token++;
            delimiter = token[0];
        }

        if (reachedEOF || token == NULL)
        {
            break;
        }

        unsigned int tokenLength = strcspn(token, " \r\n\t");
        delimiter = token[tokenLength];
        bool truncated = false;
        unsigned int tokenWidth = getTokenWidth(token, tokenLength, scale);

        // Wrap if necessary.
        if (wrap &&
            xPos + tokenWidth > area.x + area.width)
        {
            yPos += fontSize;

            if (xPositionsIt != xPositions.end())
            {
                xPos = *xPositionsIt++;
            }
            else
            {
                xPos = area.x;
            }
        }

        bool draw = true;
        if (yPos < area.y)
        {
            // Skip drawing until linebreak or wrap.
            draw = false;
        }
        else if (yPos > area.y + area.height)
        {
            // Truncate below area's vertical limit.
            break;
        }

        for (unsigned int i = 0; i < tokenLength; ++i)
        {
            char c = token[i];
            unsigned int glyphIndex = c - 32; // HACK for ASCII
        
            if (glyphIndex >= 0 && glyphIndex < _glyphCount)
            {
                Glyph& g = _glyphs[glyphIndex];

                if (xPos + (int)(g.width*scale) >= area.x + area.width)
                {
                    // Truncate this line and go on to the next one.
                    truncated = true;
                    break;
                }
                else if (xPos >= area.x)
                {
                    // Draw this character.
                    if (draw)
                    {
                        _batch->draw(xPos, yPos, g.width * scale, fontSize, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                    }
                }
                xPos += g.width*scale + (fontSize>>3);
            }
        }

        if (!truncated)
        {
            // Get next token.
            token += tokenLength;
        }
        else
        {
            // Skip the rest of this line.
            unsigned int tokenLength = strcspn(token, "\n");

            if (tokenLength > 0)
            {                
                // Get first token of next line.
                token += tokenLength;
            }
        }
    }
}

void Font::end()
{
    _batch->end();
}

void Font::measureText(const char* text, unsigned int* width, unsigned int* height, float scale)
{
    const int length = strlen(text);
    char* token = const_cast<char*>(text);
    unsigned int fontSize = _size * scale;

    *width = 0;
    *height = 0;

    // Measure a line at a time.
    while (token[0] != 0)
    {
        while (token[0] == '\n')
        {
            *height += fontSize;
            ++token;
        }

        unsigned int tokenLength = strcspn(token, "\n");
        unsigned int tokenWidth = getTokenWidth(token, tokenLength, scale);
        if (tokenWidth > *width)
        {
            *width = tokenWidth;
        }

        token += tokenLength;
    }
}

void Font::measureText(const char* text, Rectangle* out, const Rectangle& viewport, float scale, Justify justify, bool wrap, bool clipped)
{
    Justify vAlign = static_cast<Justify>(justify & 0xF0);
    if (vAlign == 0)
    {
        vAlign = ALIGN_TOP;
    }

    Justify hAlign = static_cast<Justify>(justify & 0x0F);
    if (hAlign == 0)
    {
        hAlign = ALIGN_LEFT;
    }

    int fontSize = (int)(_size * scale);
    char* token = const_cast<char*>(text);
    std::vector<bool> emptyLines;
    std::vector<Vector2> lines;

    unsigned int lineWidth = 0;
    int yPos = viewport.y;

    if (wrap)
    {
        unsigned int delimWidth = 0;
        bool reachedEOF = false;
        while (token[0] != 0)
        {
            // Handle delimiters until next token.
            char delimiter = token[0];
            while (delimiter == ' ' ||
                    delimiter == '\t' ||
                    delimiter == '\r' ||
                    delimiter == '\n' ||
                    delimiter == 0)
            {
                switch (delimiter)
                {
                    case ' ':
                        delimWidth += fontSize>>1;
                        break;
                    case '\r':
                    case '\n':
                        // Add line-height to vertical cursor.
                        yPos += fontSize;

                        if (lineWidth > 0)
                        {
                            // Determine horizontal position and width.
                            int hWhitespace = viewport.width - lineWidth;
                            int xPos = viewport.x;
                            if (hAlign == ALIGN_HCENTER)
                            {
                                xPos += hWhitespace / 2;
                            }
                            else if (hAlign == ALIGN_RIGHT)
                            {
                                xPos += hWhitespace;
                            }

                            // Record this line's size.
                            emptyLines.push_back(false);
                            lines.push_back(Vector2(xPos, lineWidth));
                        }
                        else
                        {
                            // Record the existence of an empty line.
                            emptyLines.push_back(true);
                            lines.push_back(Vector2(FLT_MAX, 0));
                        }

                        lineWidth = 0;
                        delimWidth = 0;
                        break;
                    case '\t':
                        delimWidth += (fontSize>>1)*4;
                        break;
                    case 0:
                        reachedEOF = true;
                        break;
                }

                if (reachedEOF)
                {
                    break;
                }

                token++;
                delimiter = token[0];
            }

            if (reachedEOF)
            {
                break;
            }

            // Measure the next token.
            unsigned int tokenLength = strcspn(token, " \r\n\t");
            unsigned int tokenWidth = getTokenWidth(token, tokenLength, scale);

            // Wrap if necessary.
            if (lineWidth + tokenWidth + delimWidth > viewport.width)
            {
                // Add line-height to vertical cursor.
                yPos += fontSize;

                // Determine horizontal position and width.
                int hWhitespace = viewport.width - lineWidth;
                int xPos = viewport.x;
                if (hAlign == ALIGN_HCENTER)
                {
                    xPos += hWhitespace / 2;
                }
                else if (hAlign == ALIGN_RIGHT)
                {
                    xPos += hWhitespace;
                }

                // Record this line's size.
                emptyLines.push_back(false);
                lines.push_back(Vector2(xPos, lineWidth));
                lineWidth = 0;
            }
            else
            {
                lineWidth += delimWidth;
            }

            delimWidth = 0;
            lineWidth += tokenWidth;
            token += tokenLength;
        }
    }
    else
    {
        // Measure a whole line at a time.
        int emptyLinesCount = 0;
        while (token[0] != 0)
        {
            // Handle any number of consecutive newlines.
            bool nextLine = true;
            while (token[0] == '\n')
            {
                if (nextLine)
                {
                    // Add line-height to vertical cursor.
                    yPos += fontSize * (emptyLinesCount+1);
                    nextLine = false;
                    emptyLinesCount = 0;
                    emptyLines.push_back(false);
                }
                else
                {
                    // Record the existence of an empty line.
                    ++emptyLinesCount;
                    emptyLines.push_back(true);
                    lines.push_back(Vector2(FLT_MAX, 0));
                }
                
                token++;
            }

            // Measure the next line.
            unsigned int tokenLength = strcspn(token, "\n");
            lineWidth = getTokenWidth(token, tokenLength, scale);
            
            // Determine horizontal position and width.
            int xPos = viewport.x;
            int hWhitespace = viewport.width - lineWidth;
            if (hAlign == ALIGN_HCENTER)
            {
                xPos += hWhitespace / 2;
            }
            else if (hAlign == ALIGN_RIGHT)
            {
                xPos += hWhitespace;
            }

            // Record this line's size.
            lines.push_back(Vector2(xPos, lineWidth));

            token += tokenLength;
        }

        yPos += fontSize;
    }

    if (wrap)
    {
        // Record the size of the last line.
        int hWhitespace = viewport.width - lineWidth;
        int xPos = viewport.x;
        if (hAlign == ALIGN_HCENTER)
        {
            xPos += hWhitespace / 2;
        }
        else if (hAlign == ALIGN_RIGHT)
        {
            xPos += hWhitespace;
        }

        lines.push_back(Vector2(xPos, lineWidth));
    }

    int x = INT_MAX;
    int y = viewport.y;
    unsigned int width = 0;
    int height = yPos - viewport.y;

    // Calculate top of text without clipping.
    int vWhitespace = viewport.height - height;
    if (vAlign == ALIGN_VCENTER)
    {
        y += vWhitespace / 2;
    }
    else if (vAlign == ALIGN_BOTTOM)
    {
        y += vWhitespace;
    }

    int clippedTop = 0;
    int clippedBottom = 0;
    if (clipped)
    {
        // Trim rect to fit text that would actually be drawn within the given viewport.
        if (y >= viewport.y)
        {
            // Text goes off the bottom of the viewport.
            clippedBottom = (height - viewport.height) / fontSize + 1;
            if (clippedBottom > 0)
            {
                // Also need to crop empty lines above non-empty lines that have been clipped.
                unsigned int emptyIndex = emptyLines.size() - clippedBottom;
                while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
                {
                    height -= fontSize;
                    emptyIndex++;
                }

                height -= fontSize * clippedBottom;
            }
            else
            {
                clippedBottom = 0;
            }
        }
        else
        {
            // Text goes above the top of the viewport.
            clippedTop = (viewport.y - y) / fontSize + 1;
            if (clippedTop < 0)
            {
                clippedTop = 0;
            }

            // Also need to crop empty lines below non-empty lines that have been clipped.
            unsigned int emptyIndex = clippedTop;
            while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
            {
                y += fontSize;
                height -= fontSize;
                emptyIndex++;
            }

            if (vAlign == ALIGN_VCENTER)
            {
                // In this case lines may be clipped off the bottom as well.
                clippedBottom = (height - viewport.height + vWhitespace/2 + 0.01) / fontSize + 1;
                if (clippedBottom > 0)
                {
                    emptyIndex = emptyLines.size() - clippedBottom;
                    while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
                    {
                        height -= fontSize;
                        emptyIndex++;
                    }

                    height -= fontSize * clippedBottom;
                }
                else
                {
                    clippedBottom = 0;
                }
            }

            y = y + fontSize * clippedTop;
            height = height - fontSize * clippedTop;
        }
    }

    // Determine left-most x coordinate and largest width out of lines that have not been clipped.
    for (unsigned int i = clippedTop; i < lines.size() - clippedBottom; ++i)
    {
        if (lines[i].x < x)
        {
            x = lines[i].x;
        }
        if (lines[i].y > width)
        {
            width = lines[i].y;
        }
    }

    if (clipped)
    {
        // Guarantee that the output rect will fit within the viewport.
        out->x = (x >= viewport.x)? x : viewport.x;
        out->y = (y >= viewport.y)? y : viewport.y;
        out->width = (width <= viewport.width)? width : viewport.width;
        out->height = (height <= viewport.height)? height : viewport.height;
    }
    else
    {
        out->x = x;
        out->y = y;
        out->width = width;
        out->height = height;
    }
}

unsigned int Font::getTokenWidth(const char* token, unsigned int length, float scale)
{
    // Calculate width of word or line.
    unsigned int tokenWidth = 0;
    const int size = (int)(_size * scale);
    for (unsigned int i = 0; i < length; ++i)
    {
        char c = token[i];
        switch (c)
        {
        case ' ':
            tokenWidth += size>>1;
            break;
        case '\t':
            tokenWidth += (size>>1)*4;
            break;
        default:
            unsigned int glyphIndex = c - 32;
            if (glyphIndex >= 0 && glyphIndex < _glyphCount)
            {
                Glyph& g = _glyphs[glyphIndex];
                tokenWidth += g.width * scale + (size>>3);
            }
            break;
        }
    }

    return tokenWidth;
}

void Font::reverseLines(char* text)
{
    // Naive approach: reverse each line, then render left-to-right as usual.
    while (text[0] != 0)
    {
        while (text[0] == '\n')
        {
            ++text;
        }

        unsigned int textLength = strcspn(text, "\n");

        std::string line = std::string(text, textLength);
        std::string reversedLine = std::string(line.rbegin(), line.rend());
        memcpy(text, reversedLine.c_str(), textLength);

        text += textLength;
    }
}

}
