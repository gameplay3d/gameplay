#include "TestsGame.h"

using std::string;
using std::pair;

std::vector<std::string>* TestsGame::_categories = NULL;
std::vector<TestsGame::TestRecordList>* TestsGame::_tests = NULL;

// Declare our game instance
TestsGame game;

TestsGame::TestsGame()
    : _font(NULL), _activeTest(NULL)
{
}

void TestsGame::initialize()
{
    _font = Font::create("res/common/arial18.gpb");

    for (size_t i = 0; i < _categories->size(); ++i)
    {
        std::sort((*_tests)[i].begin(), (*_tests)[i].end());
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
}

void TestsGame::update(float elapsedTime)
{
    if (_activeTest)
    {
        _activeTest->update(elapsedTime);
        return;
    }
}

void TestsGame::render(float elapsedTime)
{
    if (_activeTest)
    {
        _activeTest->render(elapsedTime);
        return;
    }
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
    drawTextMenu();
}

void TestsGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (_activeTest)
    {
        _activeTest->touchEvent(evt, x, y, contactIndex);
        return;
    }

    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        break;
    case Touch::TOUCH_RELEASE:
        activate(x, y);
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
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

void TestsGame::runTest(void* func)
{
    exitActiveTest();
    
    TestGameCreatePtr p = (TestGameCreatePtr)func;

    _activeTest = reinterpret_cast<Test*>(p());
    _activeTest->initialize();
}

void TestsGame::exitActiveTest()
{
    if (_activeTest)
    {
        _activeTest->finalize();
        SAFE_DELETE(_activeTest);
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
    const size_t size = _categories->size();
    for (size_t i = 0; i < size; ++i)
    {
        if ((*_categories)[i].compare(categoryString) == 0)
        {
            index = i;
        }
    }
    if (index < 0)
    {
        _categories->push_back(categoryString);
        index = _categories->size() - 1;
    }

    if (index <= (int)_tests->size())
    {
        _tests->resize(_categories->size());
    }
    (*_tests)[index].push_back(TestRecord(titleString, func, order));
}

void TestsGame::activate(int x, int y)
{
    if (_categories == NULL || _tests == NULL)
        return;
    int temp = y / _font->getSize();
    int itemIndex = 0;
    for (size_t i = 0; i < _categories->size(); ++i)
    {
        ++itemIndex;
        for (size_t testIndex = 0; testIndex < (*_tests)[i].size(); ++testIndex)
        {
            if (temp == itemIndex)
            {
                runTest((*_tests)[i][testIndex].funcPtr);
                return;
            }
            ++itemIndex;
        }
    }
}

void TestsGame::drawTextMenu()
{
    if (_categories == NULL)
        print("_categories null");
    if (_tests == NULL)
        print("_tests null");

    if (_categories == NULL || _tests == NULL)
        return;

    static const int INDENT = 40;
    int fontHeight = _font->getSize();
    int x = 10;
    int y = 10;
    _font->start();
    for (size_t i = 0; i < _categories->size(); ++i)
    {
        _font->drawText((*_categories)[i].c_str(), x, y, Vector4(0, 0.5f, 1, 1), fontHeight);
        y += fontHeight;
        for (TestRecordList::iterator it = (*_tests)[i].begin(); it != (*_tests)[i].end(); ++it)
        {
            _font->drawText(it->title.c_str(), x + INDENT, y, Vector4::one(), fontHeight);
            y += fontHeight;
        }
    }
    _font->finish();
}
