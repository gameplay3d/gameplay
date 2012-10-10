#ifndef TEXTTEST_H_
#define TEXTTEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Test the text and font class with various text functionality.
 */
class TextTest : public Test, public Control::Listener
{
public:

    TextTest();

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

    // Render state
    RenderState::StateBlock* _stateBlock;
    
    Font* _font;
    unsigned int _fontIndex;
    std::vector<Font*> _fonts;
    std::string _testString;
    gameplay::Rectangle _viewport;
    gameplay::Font::Justify _alignment;
    float _scale;
    bool _wrap;
    bool _ignoreClip;
    bool _useViewport;
    bool _rightToLeft;
    bool _simple;
    unsigned int _fontsCount;
    Form* _form;
};

#endif
