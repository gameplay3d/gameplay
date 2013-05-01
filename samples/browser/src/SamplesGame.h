#ifndef SAMPLESGAME_H_
#define SAMPLESGAME_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Macro for adding a sample. The purpose is to put the code that adds the sample in the class's cpp file.
 */
#define ADD_SAMPLE(cateogry, title, className, order) \
    static className* _createSample() \
    { \
        return new className(); \
    } \
    struct _foo ## className \
    { \
        _foo ## className() \
        { \
            SamplesGame::addSample((cateogry), (title), (void*)&_createSample, order); \
        } \
    }; \
    static _foo ## className _f;

/**
 * Main game class.
 */
class SamplesGame : public Game, Control::Listener
{
public:

    /**
     * Constructor.
     */
    SamplesGame();

	void keyEvent(Keyboard::KeyEvent evt, int key);

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    void menuEvent();

    void gestureSwipeEvent(int x, int y, int direction);
    
    void gesturePinchEvent(int x, int y, float scale);
    
    void gestureTapEvent(int x, int y);

    void controlEvent(Control* control, EventType evt);

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex = 0);

    /**
     * Adds a sample.
     * 
     * @param category The cateogry title to group the sample in.
     * @param title The title string of the sample.
     * @param func The function pointer that is used to create the sample.
     * @param order The order of the sample. Samples are sorted lowest order to highest.
     */
    static void addSample(const char* category, const char* title, void* func, unsigned int order);

    static SamplesGame* getInstance();
    
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
     * Function pointer type that is used to create a SampleGame.
     */
    typedef void*(*SampleGameCreatePtr)();

    /**
     * Run the given sample.
     * 
     * @param func The function pointer that creates the SampleGame.
     */
    void runSample(void* func);

    /**
     * Handles the activate when the user touches or clicks on a part of the screen.
     * 
     * @param x The x-coordinate in screen space where the user clicked.
     * @param y The y-coordinate in screen space where the user clicked.
     */
    void activate(int x, int y);

    /**
     * Exits the active sample and returns to the main menu.
     */
    void exitActiveSample();
    
    /**
     * Draws the main menu, which is a list of all the available samples.
     */
    void drawTextMenu();

private:

    struct SampleRecord
    {
        std::string title;
        void* funcPtr;
        unsigned int order;

        SampleRecord() : funcPtr(NULL), order(0) { }
        SampleRecord(std::string title, void* funcPtr, unsigned int order) : title(title), funcPtr(funcPtr), order(order) { }

        SampleRecord& operator = (const SampleRecord& copy)
        {
            title = copy.title;
            funcPtr = copy.funcPtr;
            order = copy.order;
            return *this;
        }

        bool operator<(const SampleRecord& v) const
        {
            return order < v.order;
        }
    };


    /**
     * The list of category title strings.
     */
    static std::vector<std::string>* _categories;

    /**
     * The collection of sample titles and the function pointers that create the samples.
     * The pair represents the string title of the sample and the function pointer that is used to create the sample.
     * The inner vector is a list of those pairs in the order that they were added.
     * The outer vector represents the list of categories that contain the list of samples in the category.
     * The index of _categories maps to the index of the outer vector. (Therefore their size should always be the same).
     */
    typedef std::vector<SampleRecord> SampleRecordList;
    static std::vector<SampleRecordList>* _samples;

    Sample* _activeSample;
    Font* _font;
    Form* _sampleSelectForm;
};

#endif
