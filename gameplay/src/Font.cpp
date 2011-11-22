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
        int index = c - 32; // HACK for ASCII
        if (index >= 0 && index < (int)_glyphCount)
        {
            Glyph& g = _glyphs[index];

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
                _batch->draw(xPos, yPos, g.width, _size, g.uvs[0], g.uvs[1], g.uvs[2], g.uvs[3], color);
                xPos += g.width + (_size>>3);
                break;
            }
        }
    }
}

void Font::end()
{
    _batch->end();
}

}
