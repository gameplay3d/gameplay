#include "SamplesGame.h"

using std::string;
using std::pair;

std::vector<std::string>* SamplesGame::_categories = NULL;
std::vector<SamplesGame::SampleRecordList>* SamplesGame::_samples = NULL;

// Declare our game instance
SamplesGame game;

SamplesGame::SamplesGame()
    : _activeSample(NULL), _font(NULL),  _sampleSelectForm(NULL)
{
}

void SamplesGame::initialize()
{
    _font = Font::create("res/common/arial18.gpb");

    for (size_t i = 0; i < _categories->size(); ++i)
    {
        std::sort((*_samples)[i].begin(), (*_samples)[i].end());
    }

    // Load camera script
    getScriptController()->loadScript("res/common/camera.lua");

    // Construct a form for selecting which sample to run.
    Theme* theme = Theme::create("res/common/default.theme");
    Theme::Style* formStyle = theme->getStyle("basicContainer");
    Theme::Style* buttonStyle = theme->getStyle("buttonStyle");
    Theme::Style* titleStyle = theme->getStyle("title");

    // Note: this calls addRef() on formStyle's Theme, which we created above.
    _sampleSelectForm = Form::create("sampleSelect", formStyle, Layout::LAYOUT_VERTICAL);
    theme->release();   // So we can release it once we're done creating forms with it.

    _sampleSelectForm->setAutoHeight(true);
    _sampleSelectForm->setWidth(200.0f);
    _sampleSelectForm->setScroll(Container::SCROLL_VERTICAL);
    _sampleSelectForm->setConsumeInputEvents(true);

    const size_t size = _samples->size();
    for (size_t i = 0; i < size; ++i)
    {
        Label* categoryLabel = Label::create((*_categories)[i].c_str(), titleStyle);
        categoryLabel->setAutoWidth(true);
        categoryLabel->setTextAlignment(Font::ALIGN_BOTTOM_LEFT);
        categoryLabel->setHeight(40);
        categoryLabel->setText((*_categories)[i].c_str());
        categoryLabel->setConsumeInputEvents(false);
        _sampleSelectForm->addControl(categoryLabel);
        categoryLabel->release();

        SampleRecordList list = (*_samples)[i];
        const size_t listSize = list.size();
        for (size_t j = 0; j < listSize; ++j)
        {
            SampleRecord sampleRecord = list[j];
            Button* sampleButton = Button::create(sampleRecord.title.c_str(), buttonStyle);
            sampleButton->setText(sampleRecord.title.c_str());
            sampleButton->setAutoWidth(true);
            sampleButton->setHeight(60);      // Tall enough to touch easily on a BB10 device.
            sampleButton->setConsumeInputEvents(false);   // This lets the user scroll the container if they swipe starting from a button.
            sampleButton->addListener(this, Control::Listener::CLICK);
            _sampleSelectForm->addControl(sampleButton);
            sampleButton->release();
        }
    }
    _sampleSelectForm->setState(Control::FOCUS);

    // Disable virtual gamepads.
    unsigned int gamepadCount = getGamepadCount();

    for (unsigned int i = 0; i < gamepadCount; i++)
    {
        Gamepad* gamepad = getGamepad(i, false);
        if (gamepad->isVirtual())
        {
            gamepad->getForm()->setEnabled(false);
        }
    }
}

void SamplesGame::finalize()
{
    SAFE_RELEASE(_font);
    if (_activeSample)
        _activeSample->finalize();
    SAFE_DELETE(_activeSample);
    SAFE_DELETE(_categories);
    SAFE_DELETE(_samples);
    SAFE_RELEASE(_sampleSelectForm);
}

void SamplesGame::update(float elapsedTime)
{
    if (_activeSample)
    {
        Gamepad* gamepad = getGamepad(0);
        if (gamepad && gamepad->isButtonDown(Gamepad::BUTTON_MENU2))
        {
            exitActiveSample();
            return;
        }

        getScriptController()->executeFunction<void>("camera_update", "f", elapsedTime);
        _activeSample->update(elapsedTime);
        return;
    }

    _sampleSelectForm->update(elapsedTime);
}

void SamplesGame::render(float elapsedTime)
{
    if (_activeSample)
    {
        _activeSample->render(elapsedTime);
        
        // Draw back arrow
        _font->start();
        _font->drawText("<<", getWidth() - 40, 20, Vector4::one(), 28);
        _font->finish();
        return;
    }
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    _sampleSelectForm->draw();
}

void SamplesGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (_activeSample)
    {
        if (evt == Touch::TOUCH_PRESS && x >= ((int)getWidth() - 80) && y <= 80)
        {
            exitActiveSample();
        }
        else
        {
            getScriptController()->executeFunction<void>("camera_touchEvent", "[Touch::TouchEvent]iiui", evt, x, y, contactIndex);
            _activeSample->touchEvent(evt, x, y, contactIndex);
        }
        return;
    }
}

void SamplesGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (_activeSample)
    {
        if (key == Keyboard::KEY_MENU || (evt == Keyboard::KEY_PRESS && (key == Keyboard::KEY_ESCAPE)))
        {
            // Pressing escape exits the active sample
            exitActiveSample();
        }
        else
        {
            getScriptController()->executeFunction<void>("camera_keyEvent", "[Keyboard::KeyEvent][Keyboard::Key]", evt, key);
            _activeSample->keyEvent(evt, key);
        }
        return;
    }
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

bool SamplesGame::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (_activeSample)
    {
        return _activeSample->mouseEvent(evt, x, y, wheelDelta);
    }
    return false;
}

void SamplesGame::menuEvent()
{
    exitActiveSample();
}

void SamplesGame::gestureSwipeEvent(int x, int y, int direction)
{
    if (_activeSample)
        _activeSample->gestureSwipeEvent(x, y, direction);
}

void SamplesGame::gesturePinchEvent(int x, int y, float scale)
{
    if (_activeSample)
        _activeSample->gesturePinchEvent(x, y, scale);
}
    
void SamplesGame::gestureTapEvent(int x, int y)
{
    if (_activeSample)
        _activeSample->gestureTapEvent(x, y);
}

void SamplesGame::controlEvent(Control* control, EventType evt)
{
    const size_t size = _samples->size();
    for (size_t i = 0; i < size; ++i)
    {
        SampleRecordList list = (*_samples)[i];
        const size_t listSize = list.size();
        for (size_t j = 0; j < listSize; ++j)
        {
            SampleRecord sampleRecord = list[j];
            if (sampleRecord.title.compare(control->getId()) == 0)
            {
                _sampleSelectForm->setEnabled(false);
                runSample(sampleRecord.funcPtr);
                return;
            }
        }
    }
}

void SamplesGame::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    if (_activeSample)
        _activeSample->gamepadEvent(evt, gamepad);
}

void SamplesGame::runSample(void* func)
{
    exitActiveSample();
    
    SampleGameCreatePtr p = (SampleGameCreatePtr)func;

    _activeSample = reinterpret_cast<Sample*>(p());
    _activeSample->initialize();
    resume();
}

void SamplesGame::exitActiveSample()
{
    Gamepad* virtualGamepad = getGamepad(0, false);
    if (virtualGamepad && virtualGamepad->isVirtual())
    {
        virtualGamepad->getForm()->setEnabled(false);
    }

    if (_activeSample)
    {
        _activeSample->finalize();
        SAFE_DELETE(_activeSample);

        _sampleSelectForm->setEnabled(true);
        _sampleSelectForm->setState(Control::FOCUS);
    }

    // Reset some game options
    setMultiTouch(false);
}

void SamplesGame::addSample(const char* category, const char* title, void* func, unsigned int order)
{
    if (_samples == NULL)
        _samples = new std::vector<SampleRecordList>();
    if (_categories == NULL)
    {
        _categories = new std::vector<std::string>();
        _categories->push_back("Graphics");
        _categories->push_back("Scene");
        _categories->push_back("Input");
        _categories->push_back("Physics");
        _categories->push_back("Audio");
        _samples->resize(_categories->size());
    }

    string categoryString(category);
    string titleString(title);
    
    int index = -1;
    const int size = (int)_categories->size();
    for (int i = 0; i < size; ++i)
    {
        if ((*_categories)[i].compare(categoryString) == 0)
        {
            index = i;
        }
    }
    if (index < 0)
    {
        _categories->push_back(categoryString);
        index = (int)_categories->size() - 1;
    }

    if (index <= (int)_samples->size())
    {
        _samples->resize(_categories->size());
    }
    (*_samples)[index].push_back(SampleRecord(titleString, func, order));
}

SamplesGame* SamplesGame::getInstance()
{
    return &game;
}
