#include "TestsGame.h"

using std::string;
using std::pair;

std::vector<std::string>* TestsGame::_categories = NULL;
std::vector<TestsGame::TestRecordList>* TestsGame::_tests = NULL;

// Declare our game instance
TestsGame game;

TestsGame::TestsGame()
    : _font(NULL), _activeTest(NULL), _testSelectForm(NULL)
{
}

void TestsGame::initialize()
{
    _font = Font::create("res/common/arial18.gpb");

    for (size_t i = 0; i < _categories->size(); ++i)
    {
        std::sort((*_tests)[i].begin(), (*_tests)[i].end());
    }

    // Construct a form for selecting which test to run.
    Theme* theme = Theme::create("res/common/default.theme");
    Theme::Style* formStyle = theme->getStyle("basic");
    Theme::Style* buttonStyle = theme->getStyle("buttonStyle");
    Theme::Style* titleStyle = theme->getStyle("title");

    // Note: this calls addRef() on formStyle's Theme, which we created above.
    _testSelectForm = Form::create("testSelect", formStyle, Layout::LAYOUT_VERTICAL);
    theme->release();   // So we can release it once we're done creating forms with it.

    _testSelectForm->setAutoHeight(true);
    _testSelectForm->setWidth(200.0f);
    _testSelectForm->setScroll(Container::SCROLL_VERTICAL);

    const size_t size = _tests->size();
    for (size_t i = 0; i < size; ++i)
    {
        Label* categoryLabel = Label::create((*_categories)[i].c_str(), titleStyle);
        categoryLabel->setAutoWidth(true);
        categoryLabel->setTextAlignment(Font::ALIGN_BOTTOM_LEFT);
        categoryLabel->setHeight(40);
        categoryLabel->setText((*_categories)[i].c_str());
        categoryLabel->setConsumeInputEvents(false);
        _testSelectForm->addControl(categoryLabel);
        categoryLabel->release();

        TestRecordList list = (*_tests)[i];
        const size_t listSize = list.size();
        for (size_t j = 0; j < listSize; ++j)
        {
            TestRecord testRecord = list[j];
            Button* testButton = Button::create(testRecord.title.c_str(), buttonStyle);
            testButton->setText(testRecord.title.c_str());
            testButton->setAutoWidth(true);
            testButton->setHeight(60);      // Tall enough to touch easily on a BB10 device.
            testButton->setConsumeInputEvents(false);   // This lets the user scroll the container if they swipe starting from a button.
            testButton->addListener(this, Control::Listener::CLICK);
            _testSelectForm->addControl(testButton);
            testButton->release();
        }
    }
}

void TestsGame::finalize()
{
    SAFE_RELEASE(_font);
    if (_activeTest)
        _activeTest->finalize();
    SAFE_DELETE(_activeTest);
    SAFE_DELETE(_categories);
    SAFE_DELETE(_tests);
    SAFE_RELEASE(_testSelectForm);
}

void TestsGame::update(float elapsedTime)
{
    if (_activeTest)
    {
        _activeTest->update(elapsedTime);
        return;
    }

    _testSelectForm->update(elapsedTime);
}

void TestsGame::render(float elapsedTime)
{
    if (_activeTest)
    {
        _activeTest->render(elapsedTime);
        
        // Draw back arrow
        _font->start();
        _font->drawText("<<", getWidth() - 40, 20, Vector4::one(), 28);
        _font->finish();
        return;
    }
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    _testSelectForm->draw();
}

void TestsGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (_activeTest)
    {
        if (evt == Touch::TOUCH_PRESS && x >= ((int)getWidth() - 80) && y <= 80)
        {
            exitActiveTest();
        }
        else
        {
            _activeTest->touchEvent(evt, x, y, contactIndex);
        }
        return;
    }
}

void TestsGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (_activeTest)
    {
        if (key == Keyboard::KEY_MENU || (evt == Keyboard::KEY_PRESS && (key == Keyboard::KEY_ESCAPE)))
        {
            // Pressing escape exits the active test
            exitActiveTest();
        }
        else
        {
            _activeTest->keyEvent(evt, key);
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

bool TestsGame::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (_activeTest)
    {
        return _activeTest->mouseEvent(evt, x, y, wheelDelta);
    }
    return false;
}

void TestsGame::menuEvent()
{
    exitActiveTest();
}

void TestsGame::gestureSwipeEvent(int x, int y, int direction)
{
    if (_activeTest)
        _activeTest->gestureSwipeEvent(x, y, direction);
}

void TestsGame::gesturePinchEvent(int x, int y, float scale)
{
    if (_activeTest)
        _activeTest->gesturePinchEvent(x, y, scale);
}
    
void TestsGame::gestureTapEvent(int x, int y)
{
    if (_activeTest)
        _activeTest->gestureTapEvent(x, y);
}

void TestsGame::controlEvent(Control* control, EventType evt)
{
    const size_t size = _tests->size();
    for (size_t i = 0; i < size; ++i)
    {
        TestRecordList list = (*_tests)[i];
        const size_t listSize = list.size();
        for (size_t j = 0; j < listSize; ++j)
        {
            TestRecord testRecord = list[j];
            if (testRecord.title.compare(control->getId()) == 0)
            {
                _testSelectForm->setEnabled(false);
                runTest(testRecord.funcPtr);
                return;
            }
        }
    }
}

void TestsGame::runTest(void* func)
{
    exitActiveTest();
    
    TestGameCreatePtr p = (TestGameCreatePtr)func;

    _activeTest = reinterpret_cast<Test*>(p());
    _activeTest->initialize();
    resume();
}

void TestsGame::exitActiveTest()
{
    if (_activeTest)
    {
        _activeTest->finalize();
        SAFE_DELETE(_activeTest);

        _testSelectForm->setEnabled(true);
    }
    // Reset some game options
    setMultiTouch(false);
}

void TestsGame::addTest(const char* category, const char* title, void* func, unsigned int order)
{
    if (_tests == NULL)
        _tests = new std::vector<TestRecordList>();
    if (_categories == NULL)
    {
        _categories = new std::vector<std::string>();
        _categories->push_back("Graphics");
        _categories->push_back("Scene");
        _categories->push_back("Input");
        _categories->push_back("Physics");
        _categories->push_back("Audio");
        _tests->resize(_categories->size());
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

    if (index <= (int)_tests->size())
    {
        _tests->resize(_categories->size());
    }
    (*_tests)[index].push_back(TestRecord(titleString, func, order));
}

TestsGame* TestsGame::getInstance()
{
    return &game;
}