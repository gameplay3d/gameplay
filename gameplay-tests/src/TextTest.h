#ifndef TextTest_H_
#define TextTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class TextTest : public Test
{
public:
    TextTest();
    virtual ~TextTest();

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);
    void touchEvent(Touch::TouchEvent event, int x, int y, unsigned int contactIndex);
    void keyEvent(Keyboard::KeyEvent keyEvent, int key);

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
    Font::Text* _legendText;
    float _scale;
    bool _wrap;
    bool _ignoreClip;
    bool _useViewport;
    bool _rightToLeft;
    bool _simple;
    unsigned int _fontsCount;
};

#endif
