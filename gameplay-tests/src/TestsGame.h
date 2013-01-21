#ifndef TESTSGAME_H_
#define TESTSGAME_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Macro for adding a test. The purpose is to put the code that adds the test in the class's cpp file.
 */
#define ADD_TEST(cateogry, title, className, order) \
    static className* _createTest() \
    { \
        return new className(); \
    } \
    struct _foo ## className \
    { \
        _foo ## className() \
        { \
            TestsGame::addTest((cateogry), (title), (void*)&_createTest, order); \
        } \
    }; \
    static _foo ## className _f;

/**
 * Main game class.
 */
class TestsGame : public Game, Control::Listener
{
public:

    /**
     * Constructor.
     */
    TestsGame();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * @see Game::mouseEvent
     */
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * @see Game::menuEvent
     */
    void menuEvent();

    /**
     * @see Game::gestureSwipeEvent
     */
    void gestureSwipeEvent(int x, int y, int direction);
    
    void gesturePinchEvent(int x, int y, float scale);
    
    void gestureTapEvent(int x, int y);

    /**
     * @see Control::controlEvent
     */
    void controlEvent(Control* control, EventType evt);

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

    /**
     * Adds a test.
     * 
     * @param category The cateogry title to group the test in.
     * @param title The title string of the test.
     * @param func The function pointer that is used to create the test.
     * @param order The order of the test. Tests are sorted lowest order to highest.
     */
    static void addTest(const char* category, const char* title, void* func, unsigned int order);

    static TestsGame* getInstance();
    
protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    /**
     * Function pointer type that is used to create a TestGame.
     */
    typedef void*(*TestGameCreatePtr)();

    /**
     * Run the given test.
     * 
     * @param func The function pointer that creates the TestGame.
     */
    void runTest(void* func);

    /**
     * Handles the activate when the user touches or clicks on a part of the screen.
     * 
     * @param x The x-coordinate in screen space where the user clicked.
     * @param y The y-coordinate in screen space where the user clicked.
     */
    void activate(int x, int y);

    /**
     * Exits the active test and returns to the main menu.
     */
    void exitActiveTest();
    
    /**
     * Draws the main menu, which is a list of all the available tests.
     */
    void drawTextMenu();

private:

    struct TestRecord
    {
        std::string title;
        void* funcPtr;
        unsigned int order;

        TestRecord() : funcPtr(NULL), order(0) { }
        TestRecord(std::string title, void* funcPtr, unsigned int order) : title(title), funcPtr(funcPtr), order(order) { }

        TestRecord& operator = (const TestRecord& copy)
        {
            title = copy.title;
            funcPtr = copy.funcPtr;
            order = copy.order;
            return *this;
        }

        bool operator<(const TestRecord& v) const
        {
            return order < v.order;
        }
    };


    /**
     * The list of category title strings.
     */
    static std::vector<std::string>* _categories;

    /**
     * The collection of test titles and the function pointers that create the tests.
     * The pair represents the string title of the test and the function pointer that is used to create the test.
     * The inner vector is a list of those pairs in the order that they were added.
     * The outer vector represents the list of categories that contain the list of tests in the category.
     * The index of _categories maps to the index of the outer vector. (Therefore their size should always be the same).
     */
    typedef std::vector<TestRecord> TestRecordList;
    static std::vector<TestRecordList>* _tests;

    Test* _activeTest;
    Font* _font;
    Form* _testSelectForm;
};

#endif
