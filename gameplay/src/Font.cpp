#include "Base.h"
#include "Font.h"
#include "Game.h"
#include "FileSystem.h"
#include "Package.h"

// Default font vertex shader
#define FONT_VSH \
    "uniform mat4 u_projectionMatrix;\n" \
    "attribute vec3 a_position;\n" \
    "attribute vec2 a_texCoord;\n" \
    "attribute vec4 a_color;\n" \
    "varying vec2 v_texCoord;\n" \
    "varying vec4 v_color;\n" \
    "void main()\n" \
    "{\n" \
        "gl_Position = u_projectionMatrix * vec4(a_position, 1);\n" \
        "v_texCoord = a_texCoord;\n" \
        "v_color = a_color;\n" \
    "}\n"

// Default font fragment shader
#define FONT_FSH \
    "#ifdef OPENGL_ES\n" \
    "precision highp float;\n" \
    "#endif\n" \
    "varying vec2 v_texCoord;\n" \
    "varying vec4 v_color;\n" \
    "uniform sampler2D u_texture;\n" \
    "void main()\n" \
    "{\n" \
        "gl_FragColor = v_color;\n" \
        "gl_FragColor.a = texture2D(u_texture, v_texCoord).a;\n" \
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

unsigned int Font::getSize()
{
    return _size;
}

void Font::begin()
{
    _batch->begin();
}

void Font::drawText(const char* text, int x, int y, const Vector4& color, unsigned int size, bool rightToLeft)
{
    float scale = (float)size / _size;
    char* cursor = NULL;
    if (rightToLeft)
    {
        cursor = const_cast<char*>(text);
    }

    int xPos = x, yPos = y;
    bool done = false;

    while (!done)
    {
        int length;
        int startIndex;
        int iteration;
        if (rightToLeft)
        {
            char delimiter = cursor[0];
            while (!done &&
                   (delimiter == ' ' ||
                   delimiter == '\t' ||
                   delimiter == '\r' ||
                   delimiter == '\n' ||
                   delimiter == 0))
            {
                switch (delimiter)
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
                case 0:
                    done = true;
                    break;
                }

                if (!done)
                {
                    ++cursor;
                    delimiter = cursor[0];
                }
            }

            length = strcspn(cursor, "\r\n");
            startIndex = length - 1;
            iteration = -1;
        }
        else
        {
            length = strlen(text);
            startIndex = 0;
            iteration = 1;
        }

        for (int i = startIndex; i < length && i >= 0; i += iteration)
        {
            char c = 0;
            if (rightToLeft)
            {
                c = cursor[i];
            }
            else
            {
                c = text[i];
            }

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
                int index = c - 32; // HACK for ASCII
                if (index >= 0 && index < (int)_glyphCount)
                {
                    Glyph& g = _glyphs[index];
                    _batch->draw(xPos, yPos, g.width * scale, size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                    xPos += g.width * scale + (size>>3);
                    break;
                }
            }
        }

        if (rightToLeft)
        {
            cursor += length;   
        }
        else
        {
            done = true;
        }
    }
}

void Font::drawText(const char* text, const Rectangle& area, const Vector4& color, unsigned int size, Justify justify, bool wrap, bool rightToLeft)
{
    float scale = (float)size / _size;
    char* token = const_cast<char*>(text);
    const int length = strlen(text);
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

    token = const_cast<char*>(text);

    // For alignments other than top-left, need to calculate the y position to begin drawing from
    // and the starting x position of each line.  For right-to-left text, need to determine
    // the number of characters on each line.
    std::vector<int> xPositions;
    std::vector<unsigned int> lineLengths;
    if (vAlign != ALIGN_TOP || hAlign != ALIGN_LEFT || rightToLeft)
    {
        int lineWidth = 0;
        int delimWidth = 0;

        if (wrap)
        {
            // Go a word at a time.
            bool reachedEOF = false;
            unsigned int lineLength = 0;
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
                            delimWidth += size>>1;
                            lineLength++;
                            break;
                        case '\r':
                        case '\n':
                            yPos += size;

                            if (lineWidth > 0)
                            {
                                addLineInfo(area, lineWidth, lineLength, hAlign, &xPositions, &lineLengths, rightToLeft);
                            }

                            lineWidth = 0;
                            lineLength = 0;
                            delimWidth = 0;
                            break;
                        case '\t':
                            delimWidth += (size>>1)*4;
                            lineLength++;
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
                tokenWidth += getTokenWidth(token, tokenLength, size, scale);

                // Wrap if necessary.
                if (lineWidth + tokenWidth + delimWidth > area.width)
                {
                    yPos += size;

                    // Push position of current line.
                    if (lineLength)
                    {
                        addLineInfo(area, lineWidth, lineLength-1, hAlign, &xPositions, &lineLengths, rightToLeft);
                    }
                    else
                    {
                        addLineInfo(area, lineWidth, tokenLength, hAlign, &xPositions, &lineLengths, rightToLeft);
                    }

                    // Move token to the next line.
                    lineWidth = 0;
                    lineLength = 0;
                    delimWidth = 0;
                }
                else
                {
                    lineWidth += delimWidth;
                    delimWidth = 0;
                }

                lineWidth += tokenWidth;
                lineLength += tokenLength;
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
            addLineInfo(area, lineWidth, lineLength, hAlign, &xPositions, &lineLengths, rightToLeft);
        }
        else
        {
            // Go a line at a time.
            while (token[0] != 0)
            {
                char delimiter = token[0];
                while (delimiter == '\n')
                {
                    yPos += size;
                    ++token;
                    delimiter = token[0];
                }

                unsigned int tokenLength = strcspn(token, "\n");
                if (tokenLength == 0)
                {
                    tokenLength = strlen(token);
                }

                int lineWidth = getTokenWidth(token, tokenLength, size, scale);
                addLineInfo(area, lineWidth, tokenLength, hAlign, &xPositions, &lineLengths, rightToLeft);

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
    
    int iteration = 1;
    unsigned int lineLength;
    unsigned int currentLineLength = 0;
    char* lineStart;
    std::vector<unsigned int>::const_iterator lineLengthsIt;
    if (rightToLeft)
    {
        lineStart = token;
        lineLengthsIt = lineLengths.begin();
        lineLength = *lineLengthsIt++;
        token += lineLength - 1;
        iteration = -1;
    }

    while (token[0] != 0)
    {
        // Handle delimiters until next token.
        if (!handleDelimiters(&token, size, iteration, area.x, &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
        {
            break;
        }

        bool truncated = false;
        unsigned int tokenLength;
        unsigned int tokenWidth;
        unsigned int startIndex;
        if (rightToLeft)
        {
            tokenLength = getReversedTokenLength(token, text);
            currentLineLength += tokenLength;
            token -= (tokenLength - 1);
            tokenWidth = getTokenWidth(token, tokenLength, size, scale);
            iteration = -1;
            startIndex = tokenLength - 1;
        }
        else
        {
            tokenLength = strcspn(token, " \r\n\t");
            tokenWidth = getTokenWidth(token, tokenLength, size, scale);
            iteration = 1;
            startIndex = 0;
        }

        // Wrap if necessary.
        if (wrap &&
            xPos + tokenWidth > area.x + area.width ||
            (rightToLeft && currentLineLength > lineLength))
        {
            yPos += size;
            currentLineLength = tokenLength;

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
            // Skip drawing until line break or wrap.
            draw = false;
        }
        else if (yPos > area.y + area.height)
        {
            // Truncate below area's vertical limit.
            break;
        }

        for (int i = startIndex; i < (int)tokenLength && i >= 0; i += iteration)
        {
            char c = token[i];
            int glyphIndex = c - 32; // HACK for ASCII
        
            if (glyphIndex >= 0 && glyphIndex < (int)_glyphCount)
            {
                Glyph& g = _glyphs[glyphIndex];

                if (xPos + (int)(g.width*scale) > area.x + area.width)
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
                        _batch->draw(xPos, yPos, g.width * scale, size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                    }
                }
                xPos += g.width*scale + (size>>3);
            }
        }

        if (!truncated)
        {
            if (rightToLeft)
            {
                if (token == lineStart)
                {
                    token += lineLength;
                    
                    // Now handle delimiters going forwards.
                    if (!handleDelimiters(&token, size, 1, area.x, &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
                    {
                        break;
                    }

                    if (lineLengthsIt != lineLengths.end())
                    {
                        lineLength = *lineLengthsIt++;
                    }
                    lineStart = token;
                    token += lineLength-1;
                }
                else
                {
                    token--;
                }
            }
            else
            {
                token += tokenLength;
            }
        }
        else
        {
            if (rightToLeft)
            {
                token = lineStart + lineLength;
                
                if (!handleDelimiters(&token, size, 1, area.x, &xPos, &yPos, &currentLineLength, &xPositionsIt, xPositions.end()))
                {
                    break;
                }

                if (lineLengthsIt != lineLengths.end())
                {
                    lineLength = *lineLengthsIt++;
                }
                lineStart = token;
                token += lineLength-1;
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
}

void Font::end()
{
    _batch->end();
}

void Font::measureText(const char* text, unsigned int size, unsigned int* width, unsigned int* height)
{
    float scale = (float)size / _size;
    const int length = strlen(text);
    char* token = const_cast<char*>(text);

    *width = 0;
    *height = 0;

    // Measure a line at a time.
    while (token[0] != 0)
    {
        while (token[0] == '\n')
        {
            *height += size;
            ++token;
        }

        unsigned int tokenLength = strcspn(token, "\n");
        unsigned int tokenWidth = getTokenWidth(token, tokenLength, size, scale);
        if (tokenWidth > *width)
        {
            *width = tokenWidth;
        }

        token += tokenLength;
    }
}

void Font::measureText(const char* text, const Rectangle& viewport, unsigned int size, Rectangle* out, Justify justify, bool wrap, bool ignoreClip)
{
    float scale = (float)size / _size;
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
                        delimWidth += size>>1;
                        break;
                    case '\r':
                    case '\n':
                        // Add line-height to vertical cursor.
                        yPos += size;

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
                        delimWidth += (size>>1)*4;
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
            unsigned int tokenWidth = getTokenWidth(token, tokenLength, size, scale);

            // Wrap if necessary.
            if (lineWidth + tokenWidth + delimWidth > viewport.width)
            {
                // Add line-height to vertical cursor.
                yPos += size;

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
                    yPos += size * (emptyLinesCount+1);
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
            lineWidth = getTokenWidth(token, tokenLength, size, scale);
            
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

        yPos += size;
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
    if (!ignoreClip)
    {
        // Trim rect to fit text that would actually be drawn within the given viewport.
        if (y >= viewport.y)
        {
            // Text goes off the bottom of the viewport.
            clippedBottom = (height - viewport.height) / size + 1;
            if (clippedBottom > 0)
            {
                // Also need to crop empty lines above non-empty lines that have been clipped.
                unsigned int emptyIndex = emptyLines.size() - clippedBottom;
                while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
                {
                    height -= size;
                    emptyIndex++;
                }

                height -= size * clippedBottom;
            }
            else
            {
                clippedBottom = 0;
            }
        }
        else
        {
            // Text goes above the top of the viewport.
            clippedTop = (viewport.y - y) / size + 1;
            if (clippedTop < 0)
            {
                clippedTop = 0;
            }

            // Also need to crop empty lines below non-empty lines that have been clipped.
            unsigned int emptyIndex = clippedTop;
            while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
            {
                y += size;
                height -= size;
                emptyIndex++;
            }

            if (vAlign == ALIGN_VCENTER)
            {
                // In this case lines may be clipped off the bottom as well.
                clippedBottom = (height - viewport.height + vWhitespace/2 + 0.01) / size + 1;
                if (clippedBottom > 0)
                {
                    emptyIndex = emptyLines.size() - clippedBottom;
                    while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
                    {
                        height -= size;
                        emptyIndex++;
                    }

                    height -= size * clippedBottom;
                }
                else
                {
                    clippedBottom = 0;
                }
            }

            y = y + size * clippedTop;
            height = height - size * clippedTop;
        }
    }

    // Determine left-most x coordinate and largest width out of lines that have not been clipped.
    for (int i = clippedTop; i < (int)lines.size() - clippedBottom; ++i)
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

    if (!ignoreClip)
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

unsigned int Font::getTokenWidth(const char* token, unsigned int length, unsigned int size, float scale)
{
    // Calculate width of word or line.
    unsigned int tokenWidth = 0;
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
            int glyphIndex = c - 32;
            if (glyphIndex >= 0 && glyphIndex < (int)_glyphCount)
            {
                Glyph& g = _glyphs[glyphIndex];
                tokenWidth += g.width * scale + (size>>3);
            }
            break;
        }
    }

    return tokenWidth;
}

unsigned int Font::getReversedTokenLength(const char* token, const char* bufStart)
{
    char* cursor = const_cast<char*>(token);
    char c = cursor[0];
    unsigned int length = 0;

    while (cursor != bufStart && c != ' ' && c != '\r' && c != '\n' && c != '\t')
    {
        length++;
        cursor--;
        c = cursor[0];
    }

    if (cursor == bufStart)
    {
        length++;
    }

    return length;
}

bool Font::handleDelimiters(char** token, const unsigned int size, const int iteration, const int areaX, int* xPos, int* yPos, unsigned int* lineLength,
                      std::vector<int>::const_iterator* xPositionsIt, std::vector<int>::const_iterator xPositionsEnd)
{
    char delimiter = *token[0];
    bool nextLine = true;
    while (delimiter == ' ' ||
            delimiter == '\t' ||
            delimiter == '\r' ||
            delimiter == '\n' ||
            delimiter == 0)
    {
        switch (delimiter)
        {
            case ' ':
                *xPos += size>>1;
                (*lineLength)++;
                break;
            case '\r':
            case '\n':
                *yPos += size;

                // Only use next xPos for first newline character (in case of multiple consecutive newlines).
                if (nextLine)
                {
                    if (*xPositionsIt != xPositionsEnd)
                    {
                        *xPos = **xPositionsIt;
                        (*xPositionsIt)++;
                    }
                    else
                    {
                        *xPos = areaX;
                    }
                    nextLine = false;
                    *lineLength = 0;
                }
                break;
            case '\t':
                *xPos += (size>>1)*4;
                (*lineLength)++;
                break;
            case 0:
                return false;
        }

        *token += iteration;
        delimiter = *token[0];
    }

    return true;
}

void Font::addLineInfo(const Rectangle& area, int lineWidth, int lineLength, Justify hAlign,
                       std::vector<int>* xPositions, std::vector<unsigned int>* lineLengths, bool rightToLeft)
{
    int hWhitespace = area.width - lineWidth;
    if (hAlign == ALIGN_HCENTER)
    {
        (*xPositions).push_back(area.x + hWhitespace / 2);
    }
    else if (hAlign == ALIGN_RIGHT)
    {
        (*xPositions).push_back(area.x + hWhitespace);
    }

    if (rightToLeft)
    {
        (*lineLengths).push_back(lineLength);
    }
}

SpriteBatch* Font::getSpriteBatch()
{
    return _batch;
}

Font::Justify Font::getJustifyFromString(const char* justify)
{
    if (strcmp(justify, "ALIGN_LEFT") == 0)
    {
        return Font::ALIGN_LEFT;
    }
    else if (strcmp(justify, "ALIGN_HCENTER") == 0)
    {
        return Font::ALIGN_HCENTER;
    }
    else if (strcmp(justify, "ALIGN_RIGHT") == 0)
    {
        return Font::ALIGN_RIGHT;
    }
    else if (strcmp(justify, "ALIGN_TOP") == 0)
    {
        return Font::ALIGN_TOP;
    }
    else if (strcmp(justify, "ALIGN_VCENTER") == 0)
    {
        return Font::ALIGN_VCENTER;
    }
    else if (strcmp(justify, "ALIGN_BOTTOM") == 0)
    {
        return Font::ALIGN_BOTTOM;
    }
    else if (strcmp(justify, "ALIGN_TOP_LEFT") == 0)
    {
        return Font::ALIGN_TOP_LEFT;
    }
    else if (strcmp(justify, "ALIGN_VCENTER_LEFT") == 0)
    {
        return Font::ALIGN_VCENTER_LEFT;
    }
    else if (strcmp(justify, "ALIGN_BOTTOM_LEFT") == 0)
    {
        return Font::ALIGN_BOTTOM_LEFT;
    }
    else if (strcmp(justify, "ALIGN_TOP_HCENTER") == 0)
    {
        return Font::ALIGN_TOP_HCENTER;
    }
    else if (strcmp(justify, "ALIGN_VCENTER_HCENTER") == 0)
    {
        return Font::ALIGN_VCENTER_HCENTER;
    }
    else if (strcmp(justify, "ALIGN_BOTTOM_HCENTER") == 0)
    {
        return Font::ALIGN_BOTTOM_HCENTER;
    }
    else if (strcmp(justify, "ALIGN_TOP_RIGHT") == 0)
    {
        return Font::ALIGN_TOP_RIGHT;
    }
    else if (strcmp(justify, "ALIGN_VCENTER_RIGHT") == 0)
    {
        return Font::ALIGN_VCENTER_RIGHT;
    }
    else if (strcmp(justify, "ALIGN_BOTTOM_RIGHT") == 0)
    {
        return Font::ALIGN_BOTTOM_RIGHT;
    }

    // Default.
    return Font::ALIGN_TOP_LEFT;
}

}
