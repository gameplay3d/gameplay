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

void Font::drawText(const char* text, int x, int y, const Vector4& color)
{
    const int length = strlen(text);
    int xPos = x, yPos = y;
    for (int i = 0; i < length; ++i)
    {
        char c = text[i];

        // Draw this character.
        switch (c)
        {
        case ' ':
            xPos += _size>>1;
            break;
        case '\r':
        case '\n':
            yPos += _size;
            xPos = x;
            break;
        case '\t':
            xPos += (_size>>1)+4;
            break;
        default:
            unsigned int index = c - 32; // HACK for ASCII
            if (index >= 0 && index < _glyphCount)
            {
                Glyph& g = _glyphs[index];
                _batch->draw(xPos, yPos, g.width, _size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                xPos += g.width + (_size>>3);
                break;
            }
        }
    }
}

void Font::drawText(const char* text, const Rectangle& area, const Vector4& color, Justify justify, bool wrap)
{
    const int length = strlen(text);
    int yPos = area.y;
    int fontSize = (int)_size;

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

    // For alignments other than top-left, need to calculate the y position to begin drawing from
    // and the starting x position of each line.
    std::vector<int> xPositions;
    if (vAlign != ALIGN_TOP || hAlign != ALIGN_LEFT)
    {
        int lineWidth = 0;
        int delimWidth = 0;
        char* token;

        token = const_cast<char*>(text);
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
                       //delimiter == '-' ||
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
                            delimWidth += (fontSize>>1)+4;
                            break;
                        case 0:
                            reachedEOF = true;
                            break;
                            /*
                        case '-':
                            unsigned int glyphIndex = delimiter - 32;
                            Glyph& g = _glyphs[glyphIndex];
                            tokenWidth += g.width + (_size>>3);
                            //lineWidth += g.width + (_size>>3);
                            //delimWidth += g.width + (_size>>3);
                            break;*/
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
                tokenWidth += getTokenWidth(token, tokenLength);

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

                int lineWidth = getTokenWidth(token, tokenLength);
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

    char* token = const_cast<char*>(text);
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
               //delimiter == '-' ||
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
                    xPos += (fontSize>>1)+4;
                    break;
                case 0:
                    reachedEOF = true;
                    break;
                /*case '-':
                    // Dash is special -- a delimiter that isn't whitespace.
                    unsigned int glyphIndex = delimiter - 32;
                    Glyph& g = _glyphs[glyphIndex];
                    _batch->draw(xPos, yPos, g.width, _size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                    xPos += g.width + (_size>>3);

                    // Get next xPos again (in case of a substring such as "\n-\n").
                    nextLine = true;
                    break;*/
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
        unsigned int tokenWidth = getTokenWidth(token, tokenLength);

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

                if (xPos + (int)g.width >= area.x + area.width)
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
                        _batch->draw(xPos, yPos, g.width, _size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                    }
                }
                xPos += g.width + (fontSize>>3);
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
                // Move x position to that of the next line.
                /*
                if (xPositionsIt != xPositions.end())
                {
                    xPos = *xPositionsIt++;
                }
                else
                {
                    xPos = area.x;
                }*/
                
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

void Font::measureText(const char* text, unsigned int* width, unsigned int* height)
{
    const int length = strlen(text);
    int index = 0;
    char* token;
    char* textCopy = new char[length+1];
    strncpy(textCopy, text, length);
    textCopy[length] = 0;

    // Measure each line.
    // if (lineWidth > maxWidth) maxWidth = lineWidth;
    // height += lineHeight

    *width = 0;
    *height = 0;

    token = strtok(textCopy, "\n");
    while (token != NULL)
    {
        while (text[index] == '\n')
        {
            *height += _size;
            ++index;
        }

        int tokenLength = strlen(token);
        unsigned int tokenWidth = getTokenWidth(token, tokenLength);
        if (tokenWidth > *width)
        {
            *width = tokenWidth;
        }

        *height += _size;
        token = strtok(NULL, "\n");
        index += tokenLength + 1;
    }
}

void Font::measureText(const char* text, const Rectangle& viewport, Justify justify, bool wrap, bool clipped, Rectangle* out)
{
    Justify vAlign = static_cast<Justify>(justify & 0xF0);
    Justify hAlign = static_cast<Justify>(justify & 0x0F);

    int fontSize = (int)_size;

    const int length = strlen(text);
    int index = 0;
    char* token = const_cast<char*>(text);

    unsigned int lineWidth = 0;
    unsigned int delimWidth = 0;
    int emptyLinesCount = 0;
    std::vector<bool> emptyLines;
    std::vector<Vector2> lines;

    int x = INT_MAX;
    int y = viewport.y;
    unsigned int width = 0;
    int yPos = viewport.y;

    if (wrap)
    {
        bool reachedEOF = false;
        while (token[0] != 0)
        {
            // Handle delimiters until next token.
            char delimiter = token[0];
            while (delimiter == ' ' ||
                    delimiter == '\t' ||
                    delimiter == '\r' ||
                    delimiter == '\n' ||
                    delimiter == '-' ||
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
                            emptyLines.push_back(false);

                            int hWhitespace = viewport.width - lineWidth;
                            int xPos = viewport.x;
                            if (hAlign == ALIGN_HCENTER)
                            {
                                xPos = viewport.x + hWhitespace / 2;
                            }
                            else if (hAlign == ALIGN_RIGHT)
                            {
                                xPos = viewport.x + hWhitespace;
                            }

                            lines.push_back(Vector2(xPos, lineWidth));
                            if (xPos < x)
                            {
                                x = xPos;
                            }
                            if (lineWidth > width)
                            {
                                width = lineWidth;
                            }
                        }
                        else
                        {
                            emptyLines.push_back(true);
                            lines.push_back(Vector2(FLT_MAX, 0));
                        }

                        lineWidth = 0;
                        delimWidth = 0;
                        break;
                    case '\t':
                        delimWidth += (fontSize>>1)+4;
                        break;
                    case 0:
                        reachedEOF = true;
                        break;
                    case '-':
                        unsigned int glyphIndex = delimiter - 32;
                        Glyph& g = _glyphs[glyphIndex];
                        lineWidth += g.width + (fontSize>>3);
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

            unsigned int tokenLength = strcspn(token, " -\r\n\t");
            unsigned int tokenWidth = getTokenWidth(token, tokenLength);

            // Wrap if necessary.
            if (lineWidth + tokenWidth + delimWidth > viewport.width)
            {
                emptyLines.push_back(false);

                yPos += fontSize;

                int hWhitespace = viewport.width - lineWidth;
                int xPos = viewport.x;
                if (hAlign == ALIGN_HCENTER)
                {
                    xPos = viewport.x + hWhitespace / 2;
                }
                else if (hAlign == ALIGN_RIGHT)
                {
                    xPos = viewport.x + hWhitespace;
                }

                lines.push_back(Vector2(xPos, lineWidth));
                if (xPos < x)
                {
                    x = xPos;
                }
                if (lineWidth > width)
                {
                    width = lineWidth;
                }

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
        unsigned int lineWidth = 0;
        while (token[0] != 0)
        {
            bool nextLine = true;
            while (token[0] == '\n')
            {
                if (nextLine)
                {
                    yPos += fontSize * (emptyLinesCount+1);
                    nextLine = false;
                    emptyLinesCount = 0;
                    emptyLines.push_back(false);
                }
                else
                {
                    ++emptyLinesCount;
                    emptyLines.push_back(true);
                    lines.push_back(Vector2(FLT_MAX, 0));
                }
                
                token++;
            }

            unsigned int tokenLength = strcspn(token, "\n");
            lineWidth = getTokenWidth(token, tokenLength);
            
            int xPos = viewport.x;
            int hWhitespace = viewport.width - lineWidth;
            if (hAlign == ALIGN_HCENTER)
            {
                xPos = viewport.x + hWhitespace / 2;
            }
            else if (hAlign == ALIGN_RIGHT)
            {
                xPos = viewport.x + hWhitespace;
            }

            lines.push_back(Vector2(xPos, lineWidth));
            if (xPos < x)
            {
                x = xPos;
            }
            if (lineWidth > width)
            {
                width = lineWidth;
            }

            token += tokenLength;
        }

        yPos += fontSize;
    }

    int hWhitespace = viewport.width - lineWidth;
    int xPos = viewport.x;
    if (hAlign == ALIGN_HCENTER)
    {
        xPos = viewport.x + hWhitespace / 2;
    }
    else if (hAlign == ALIGN_RIGHT)
    {
        xPos = viewport.x + hWhitespace;
    }

    if (wrap)
    {
        lines.push_back(Vector2(xPos, lineWidth));
    }

    if (xPos < x)
    {
        x = xPos;
    }
    if (lineWidth > width)
    {
        width = lineWidth;
    }

    int textHeight = yPos - viewport.y;
    int vWhitespace = viewport.height - textHeight;
    if (vAlign == ALIGN_VCENTER)
    {
        y = viewport.y + vWhitespace / 2;
    }
    else if (vAlign == ALIGN_BOTTOM)
    {
        y = viewport.y + vWhitespace;
    }
    else
    {
        y = viewport.y;
    }

    if (clipped)
    {
        if (y >= viewport.y)
        {
            out->y = y;

            int croppedLinesCount = (textHeight - viewport.height) / fontSize + 1;
            if (croppedLinesCount > 0)
            {
                unsigned int emptyIndex = emptyLines.size() - croppedLinesCount;
                while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
                {
                    y += fontSize;
                    textHeight -= fontSize;
                    emptyIndex++;
                }

                textHeight -= fontSize * croppedLinesCount;

                for (unsigned int i = croppedLinesCount; i < lines.size(); ++i)
                {
                    if (lines[i].x < x)
                    {
                        x = lines[i].x;
                        width = lines[i].y;
                    }
                }
            }

            out->height = textHeight;
        }
        else
        {
            int croppedLinesCount = (viewport.y - y) / fontSize + 1;
            unsigned int emptyIndex = croppedLinesCount;
            while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
            {
                y += fontSize;
                textHeight -= fontSize;
                emptyIndex++;
            }

            int croppedV = 0;
            if (vAlign == ALIGN_VCENTER)
            {
                croppedV = (textHeight - viewport.height + vWhitespace/2 + 0.01) / fontSize + 1;
                if (croppedV > 0)
                {
                    emptyIndex = emptyLines.size() - croppedV;
                    while (emptyIndex < emptyLines.size() && emptyLines[emptyIndex] == true)
                    {
                        textHeight -= fontSize;
                        emptyIndex++;
                    }

                    textHeight -= fontSize * croppedV;
                }
            }

            x = INT_MAX;
            width = 0;
            for (unsigned int i = croppedLinesCount; i < lines.size() - croppedV; ++i)
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

            out->y = y + fontSize * croppedLinesCount;
            out->height = textHeight - fontSize * croppedLinesCount;
        }

        out->x = (x >= viewport.x)? x : viewport.x;
        out->width = (width <= viewport.width)? width : viewport.width;
    }
    else
    {
        out->x = x;
        out->y = y;
        out->width = width;
        out->height = textHeight;
    }
}

unsigned int Font::getTokenWidth(const char* token, unsigned int length)
{
    // Calculate width of word or line.
    unsigned int tokenWidth = 0;
    for (unsigned int i = 0; i < length; ++i)
    {
        char c = token[i];
        switch (c)
        {
        case ' ':
            tokenWidth += _size>>1;
            break;
        case '\t':
            tokenWidth += (_size>>1)+4;
            break;
        default:
            unsigned int glyphIndex = c - 32;
            if (glyphIndex >= 0 && glyphIndex < _glyphCount)
            {
                Glyph& g = _glyphs[glyphIndex];
                tokenWidth += g.width + (_size>>3);
            }
            break;
        }
    }

    return tokenWidth;
}

}
