#include "TextSample.h"
#include "SamplesGame.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Text", TextSample, 9);
#endif

#define FONT_COUNT 6

std::string _fontNames[] =
{
    "arial18",
    "dynamic",
    "pirulen",
    "squarehead",
    "baroque",
    "custom",
};

TextSample::TextSample()
    : _form(NULL), _stateBlock(NULL), _scale(1.0f), _wrap(true), _ignoreClip(false), _useViewport(true), _rightToLeft(false), _simple(false), _alignment(Font::ALIGN_LEFT),
      _fontsCount(FONT_COUNT), _fontIndex(0), _font(NULL), _viewport(250, 100, 512, 200)
{
}

void TextSample::finalize()
{
    SAFE_RELEASE(_stateBlock);

    for (unsigned int i = 0; i < _fonts.size(); i++)
    {
        SAFE_RELEASE(_fonts[i]);
    }

    SAFE_RELEASE(_form);
}

void TextSample::initialize()
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
    
    _sampleString = std::string( "Lorem ipsum dolor sit amet, \n" \
                                "consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n" \
                                "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n" \
                                "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n" \
                                "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    // Create and listen to form.
    _form = Form::create("res/common/text.form");
    static_cast<Button*>(_form->getControl("fontButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("wrapButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("clipRectButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("reverseButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("switchClipRegionButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("simpleAdvancedButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("smallerButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("biggerButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("topLeftButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("topCenterButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("topRightButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("centerLeftButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("centerButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("centerRightButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("bottomLeftButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("bottomCenterButton"))->addListener(this, Control::Listener::CLICK);
    static_cast<Button*>(_form->getControl("bottomRightButton"))->addListener(this, Control::Listener::CLICK);
}

void TextSample::update(float elapsedTime)
{
}

void TextSample::render(float elapsedTime)
{
    // Clear the screen.
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);

    // Draw the frame rate.
    char fps[5];
    sprintf(fps, "%u", getFrameRate());

    _fonts[1]->start();

    _fonts[1]->drawText(fps, 245, 5, Vector4(0, 0.5f, 1, 1), _fonts[0]->getSize());
    
    _form->draw();

    unsigned int size = (float)_font->getSize() * _scale;
    if (_font != _fonts[1])
        _font->start();

    if (_simple)
    {
        // Sample simple versions of measureText, drawText.
        unsigned int w, h;
        _font->measureText(_sampleString.c_str(), size, &w, &h);
        _font->drawText(_sampleString.c_str(), _viewport.x, _viewport.y, Vector4::fromColor(0xff0000ff), size, _rightToLeft);

        _font->drawText("'", _viewport.x, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x, _viewport.y + h, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText("'", _viewport.x + w, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x + w, _viewport.y + h, Vector4::fromColor(0x00ff00ff), size);
    }
    else
    {
        // Sample viewport versions.
        gameplay::Rectangle area;
        _font->measureText(_sampleString.c_str(), _viewport, size, &area, _alignment, _wrap, _ignoreClip);
        _font->drawText(_sampleString.c_str(), _useViewport? _viewport : area, Vector4::fromColor(0xffffffff), size, _alignment, _wrap, _rightToLeft);
    
        _font->drawText("'", _viewport.x, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x, _viewport.y + _viewport.height, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText("'", _viewport.x + _viewport.width, _viewport.y, Vector4::fromColor(0x00ff00ff), size);
        _font->drawText(".", _viewport.x + _viewport.width, _viewport.y + _viewport.height, Vector4::fromColor(0x00ff00ff), size);

        _font->drawText("'", area.x, area.y, Vector4::fromColor(0x0000ffff), size);
        _font->drawText(".", area.x, area.y + area.height, Vector4::fromColor(0x0000ffff), size);
        _font->drawText("'", area.x + area.width, area.y, Vector4::fromColor(0x0000ffff), size);
        _font->drawText(".", area.x + area.width, area.y + area.height, Vector4::fromColor(0x0000ffff), size);
    }

    if (_font != _fonts[1])
    {
        _font->finish();
    }
    _fonts[1]->finish();
}

void TextSample::touchEvent(Touch::TouchEvent event, int x, int y, unsigned int contactIndex)
{
    _viewport.width = x - _viewport.x;
    _viewport.height = y - _viewport.y;
}

void TextSample::controlEvent(Control* control, EventType evt)
{
    const char* id = control->getId();

    if (strcmp(id, "fontButton") == 0)
    {
        _fontIndex++;
        if (_fontIndex >= _fontsCount)
        {
            _fontIndex = 0;
        }
        _font = _fonts[_fontIndex];
        std::string s = "Font (" + _fontNames[_fontIndex] + ")";
        static_cast<Button*>(control)->setText(s.c_str());
    }
    else if (strcmp(id, "wrapButton") == 0)
    {
        _wrap = !_wrap;
        Button* wrapButton = static_cast<Button*>(control);
        if (_wrap)
            wrapButton->setText("Word Wrap (On)");
        else
            wrapButton->setText("Word Wrap (Off)");
    }
    else if (strcmp(id, "clipRectButton") == 0)
    {
        _ignoreClip = !_ignoreClip;
        Button* clipRectButton = static_cast<Button*>(control);
        if (_ignoreClip)
            clipRectButton->setText("Clipping (Off)");
        else
            clipRectButton->setText("Clipping (On)");
    }
    else if (strcmp(id, "reverseButton") == 0)
    {
        _rightToLeft = !_rightToLeft;
        Button* reverseButton = static_cast<Button*>(control);
        if (_rightToLeft)
            reverseButton->setText("Reverse Text (On)");
        else
            reverseButton->setText("Reverse Text (Off)");
    }
    else if (strcmp(id, "switchClipRegionButton") == 0)
    {
        _useViewport = !_useViewport;
        Button* switchClipButton = static_cast<Button*>(control);
        if (_useViewport)
            switchClipButton->setText("Clip Regions (Viewport)");
        else
            switchClipButton->setText("Clip Regions (Text Area)");
    }
    else if (strcmp(id, "simpleAdvancedButton") == 0)
    {
        _simple = !_simple;
        Button* simpleAdvancedButton = static_cast<Button*>(control);
        if (_simple)
            simpleAdvancedButton->setText("Font API (Simple)");
        else
            simpleAdvancedButton->setText("Font API (Advanced)");
    }
    else if (strcmp(id, "smallerButton") == 0)
    {
        if (_scale > 0.11f)
        {
            _scale -= 0.1f;
            Label* scaleLabel = static_cast<Label*>(_form->getControl("scaleLabel"));
            char s[20];
            sprintf(s, "Font Scale (%.1f)", _scale);
            scaleLabel->setText(s);
        }
    }
    else if (strcmp(id, "biggerButton") == 0)
    {
        _scale += 0.1f;
        Label* scaleLabel = static_cast<Label*>(_form->getControl("scaleLabel"));
        char s[20];
        sprintf(s, "Scale (%.1f)", _scale);
        scaleLabel->setText(s);
    }
    else if (strcmp(id, "topLeftButton") == 0)
    {
        _alignment = Font::ALIGN_TOP_LEFT;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Top-Left)");
    }
    else if (strcmp(id, "topCenterButton") == 0)
    {
        _alignment = Font::ALIGN_TOP_HCENTER;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Top-Center)");
    }
    else if (strcmp(id, "topRightButton") == 0)
    {
        _alignment = Font::ALIGN_TOP_RIGHT;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Top-Right)");
    }
    else if (strcmp(id, "centerLeftButton") == 0)
    {
        _alignment = Font::ALIGN_VCENTER_LEFT;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Center-Left)");
    }
    else if (strcmp(id, "centerButton") == 0)
    {
        _alignment = Font::ALIGN_VCENTER_HCENTER;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Center)");
    }
    else if (strcmp(id, "centerRightButton") == 0)
    {
        _alignment = Font::ALIGN_VCENTER_RIGHT;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Center-Right)");
    }
    else if (strcmp(id, "bottomLeftButton") == 0)
    {
        _alignment = Font::ALIGN_BOTTOM_LEFT;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Bottom-Left)");
    }
    else if (strcmp(id, "bottomCenterButton") == 0)
    {
        _alignment = Font::ALIGN_BOTTOM_HCENTER;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Bottom-Center)");
    }
    else if (strcmp(id, "bottomRightButton") == 0)
    {
        _alignment = Font::ALIGN_BOTTOM_RIGHT;
        Label* alignmentLabel = static_cast<Label*>(_form->getControl("alignmentLabel"));
        alignmentLabel->setText("Align (Bottom-Right)");
    }
}
