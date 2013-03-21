#ifndef TEXTSAMPLE_H_
#define TEXTSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample the text and font class with various text functionality.
 */
class TextSample : public Sample, public Control::Listener
{
public:

    TextSample();

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);
    void touchEvent(Touch::TouchEvent event, int x, int y, unsigned int contactIndex);
    void controlEvent(Control* control, EventType evt);

private:
    void renderToTexture();
    void buildQuad(Texture* texture);

    Form* _form;
    RenderState::StateBlock* _stateBlock;
    float _scale;
    bool _wrap;
    bool _ignoreClip;
    bool _useViewport;
    bool _rightToLeft;
    bool _simple;
    Font::Justify _alignment;
    std::vector<Font*> _fonts;
    unsigned int _fontsCount;
    unsigned int _fontIndex;
    Font* _font;
    Rectangle _viewport;
    std::string _sampleString;

};

#endif
