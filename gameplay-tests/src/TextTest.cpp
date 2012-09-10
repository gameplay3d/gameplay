#include "TextTest.h"
#include "TestsGame.h"

#if defined(ADD_TEST)
    ADD_TEST("Graphics", "Text", TextTest, 100);
#endif

const char* _legend =   "Keyboard input:\n\n" \
                        "1-9:  Alignments\n" \
                        "+/-: Scaling\n" \
                        "F: Change font\n" \
                        "W: Wrapping\n" \
                        "C: Ignore clip-rect\n" \
                        "R: Reverse\n" \
                        "V: Switch between viewport\n" \
                        "   and clip region.\n" \
                        "S: Simple / Advanced";

std::string _fontNames[] =
{
    "arial14",
    "arial18",
    "dynamic",
    "pirulen",
    "SFSquareHead",
    "BaroqueScript",
    "FOO",
};

TextTest::TextTest()
    : _font(NULL), _fontIndex(0), _stateBlock(NULL), _legendText(NULL), _viewport(250, 100, 512, 200), _alignment(Font::ALIGN_LEFT),  
    _scale(1.0f), _wrap(true), _ignoreClip(false), _useViewport(true), _simple(false), _rightToLeft(false), _fontsCount(7)
{
}

TextTest::~TextTest()
{
}

void TextTest::finalize()
{
    SAFE_RELEASE(_stateBlock);
    SAFE_DELETE(_legendText);

    for (unsigned int i = 0; i < _fonts.size(); i++)
    {
        SAFE_RELEASE(_fonts[i]);
    }
}

void TextTest::initialize()
{
    // Create our render state block that will be reused across all materials
    _stateBlock = RenderState::StateBlock::create();
    _stateBlock->setCullFace(true);
    _stateBlock->setDepthTest(true);
    _stateBlock->setBlend(true);
    _stateBlock->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    _stateBlock->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    for (unsigned int i = 0; i < _fontsCount; i++)
    {
        std::string s = "res/common/";
        s += _fontNames[i].c_str();
        s += ".gpb";
        Font* f = Font::create(s.c_str());
        _fonts.push_back(f);
    }
    _font = _fonts[0];

    _legendText = _fonts[0]->createText(_legend, Rectangle(5, 100, 1000, 500), Vector4(0, 1, 0, 1), _fonts[0]->getSize());
    
    _testString = std::string(
        "Some text for you to try wrapping.\n" \
        "Text text text!\n\n" \
        "Hello?\tGoodbye!");
}

void TextTest::update(float elapsedTime)
{
}

void TextTest::render(float elapsedTime)
{
    // Clear the screen.
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);

    // Draw the FPS, font info and instructions.
    char fps[5];
    sprintf(fps, "%u", getFrameRate());
    
    char fontSize[4];
    sprintf(fontSize, "%u", _fonts[_fontIndex]->getSize());
    char scale[9];
    sprintf(scale, "%f", _scale);

    std::string fontInfo = _fontNames[_fontIndex];
    fontInfo += "\tSize: ";
    fontInfo += fontSize;
    fontInfo += "\tScale: ";
    fontInfo += scale;

    _fonts[1]->start();
    _fonts[1]->drawText(fps, 5, 5, Vector4(1, 0, 0, 1), _fonts[1]->getSize());
    _fonts[1]->drawText(fontInfo.c_str(), 250, 5, Vector4(0, 1, 0, 1), _fonts[1]->getSize());
    //_fonts[1]->end();

    _fonts[0]->start();
    _fonts[0]->drawText(_legendText);
    //_fonts[0]->end();

    unsigned int size = (float)_font->getSize() * _scale;
    if (_font != _fonts[0] && _font != _fonts[1])
        _font->start();

    if (_simple)
    {
        // Test simple versions of measureText, drawText.
        unsigned int w, h;
        _font->measureText(_testString.c_str(), size, &w, &h);
        _font->drawText(_testString.c_str(), _viewport.x, _viewport.y, Vector4::fromColor(0xff0000ff), size, _rightToLeft);

        _font->drawText("'", _viewport.x, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x, _viewport.y + h, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText("'", _viewport.x + w, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x + w, _viewport.y + h, Vector4::fromColor(0x00ff00ff), size);
    }
    else
    {
        // Test viewport versions.
        gameplay::Rectangle area;
        _font->measureText(_testString.c_str(), _viewport, size, &area, _alignment, _wrap, _ignoreClip);
        _font->drawText(_testString.c_str(), _useViewport? _viewport : area, Vector4::fromColor(0xffffffff), size, _alignment, _wrap, _rightToLeft);
    
        _font->drawText("'", _viewport.x, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x, _viewport.y + _viewport.height, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText("'", _viewport.x + _viewport.width, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x + _viewport.width, _viewport.y + _viewport.height, Vector4::fromColor(0x00ff00ff), size);

        _font->drawText("'", area.x, area.y, Vector4::fromColor(0x0000ffff), size);
        _font->drawText(".", area.x, area.y + area.height, Vector4::fromColor(0x0000ffff), size);
        _font->drawText("'", area.x + area.width, area.y, Vector4::fromColor(0x0000ffff), size);
        _font->drawText(".", area.x + area.width, area.y + area.height, Vector4::fromColor(0x0000ffff), size);
    }

    if (_font != _fonts[0] && _font != _fonts[1])
        _font->finish();

    _fonts[0]->finish();
    _fonts[1]->finish();
}

void TextTest::touchEvent(Touch::TouchEvent event, int x, int y, unsigned int contactIndex)
{
    _viewport.width = x - _viewport.x;
    _viewport.height = y - _viewport.y;
}

void TextTest::keyEvent(Keyboard::KeyEvent keyEvent, int key)
{
    switch (keyEvent)
    {
    case Keyboard::KEY_PRESS:
        switch (key)
        {
            case Keyboard::KEY_ONE:
                _alignment = Font::ALIGN_TOP_LEFT;
                break;
            case Keyboard::KEY_TWO:
                _alignment = Font::ALIGN_TOP_HCENTER;
                break;
            case Keyboard::KEY_THREE:
                _alignment = Font::ALIGN_TOP_RIGHT;
                break;
            case Keyboard::KEY_FOUR:
                _alignment = Font::ALIGN_VCENTER_LEFT;
                break;
            case Keyboard::KEY_FIVE:
                _alignment = Font::ALIGN_VCENTER_HCENTER;
                break;
            case Keyboard::KEY_SIX:
                _alignment = Font::ALIGN_VCENTER_RIGHT;
                break;
            case Keyboard::KEY_SEVEN:
                _alignment = Font::ALIGN_BOTTOM_LEFT;
                break;
            case Keyboard::KEY_EIGHT:
                _alignment = Font::ALIGN_BOTTOM_HCENTER;
                break;
            case Keyboard::KEY_NINE:
                _alignment = Font::ALIGN_BOTTOM_RIGHT;
                break;
            case Keyboard::KEY_PLUS:
                _scale += 0.1f;
                break;
            case Keyboard::KEY_MINUS:
                _scale -= 0.1f;
                break;
            case Keyboard::KEY_C:
                _ignoreClip = !_ignoreClip;
                break;
            case Keyboard::KEY_F:
                _fontIndex++;
                if (_fontIndex >= _fontsCount)
                {
                    _fontIndex = 0;
                }
                _font = _fonts[_fontIndex];
                break;
            case Keyboard::KEY_R:
                _rightToLeft = !_rightToLeft;
                break;
            case Keyboard::KEY_S:
                _simple = !_simple;
                break;
            case Keyboard::KEY_V:
                _useViewport = !_useViewport;
                break;
            case Keyboard::KEY_W:
                _wrap = !_wrap;
                break;
        }
    }
}