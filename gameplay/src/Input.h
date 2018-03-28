#pragma once

namespace gameplay
{

/**
 * Defines user input api.
 */
class Input
{
public:

    static const uint32_t kGamepadsMax = 4;

    /**
     * Defines input keys.
     */
    enum class Key
    {
        eNone = 0,
        eEsc,
        eReturn,
        eTab,
        eSpace,
        eBackspace,
        eUp,
        eDown,
        eLeft,
        eRight,
        eInsert,
        eDelete,
        eHome,
        eEnd,
        ePageUp,
        ePageDown,
        ePrint,
        ePlus,
        eMinus,
        eLeftBracket,
        eRightBracket,
        eSemiColon,
        eQuote,
        eComma,
        ePeriod,
        eSlash,
        eBackslash,
        eTilde,
        eF1,
        eF2,
        eF3,
        eF4,
        eF5,
        eF6,
        eF7,
        eF8,
        eF9,
        eF10,
        eF11,
        eF12,
        eNumpad0,
        eNumpad1,
        eNumpad2,
        eNumpad3,
        eNumpad4,
        eNumpad5,
        eNumpad6,
        eNumpad7,
        eNumpad8,
        eNumpad9,
        e0,
        e1,
        e2,
        e3,
        e4,
        e5,
        e6,
        e7,
        e8,
        e9,
        eA,
        eB,
        eC,
        eD,
        eE,
        eF,
        eG,
        eH,
        eI,
        eJ,
        eK,
        eL,
        eM,
        eN,
        eO,
        eP,
        eQ,
        eR,
        eS,
        eT,
        eU,
        eV,
        eW,
        eX,
        eY,
        eZ,
        eGamepadA,
        eGamepadB,
        eGamepadX,
        eGamepadY,
        eGamepadThumbLeft,
        eGamepadThumbRight,
        eGamepadShoulderLeft,
        eGamepadShoulderRight,
        eGamepadUp,
        eGamepadDown,
        eGamepadLeft,
        eGamepadRight,
        eGamepadBack,
        eGamepadStart,
        eGamepadGuide,
        eCount
    };

    /**
     * Defines key modifiers
     */
    struct KeyModifiers
    {
        static const uint32_t
        kNone       = 0,
        kLeftAlt    = 0x01,
        kRightAlt   = 0x02,
        kLeftCtrl   = 0x04,
        kRightCtrl  = 0x08,
        kLeftShift  = 0x10,
        kRightShift = 0x20,
        kLeftMeta   = 0x40,
        kRightMeta  = 0x80;
    };

    /**
     * Defines input mouse buttons.
     */
    enum class MouseButton
    {
        eNone   = 0,
        eLeft,
        eRight,
        eMiddle
    };

    /**
     * Defines gamepad axis
     */
    enum class GamepadAxis
    {
        eLeftX,
        eLeftY,
        eLeftZ,
        eRightX,
        eRightY,
        eRightZ,
        eCount
    };

    /**
     * Gets the string name for a key.
     *
     * @param key The key to get the string name for.
     * @return The string name for a key.
     */
    static std::string getKeyName(Input::Key key);

    /**
     * Gets the ascii keycode for a key.
     *
     * @param key The key to get the ascii keycode for.
     * @param keyModifiers The key modifiers applied.
     * @return The ascii keycode for a key.
     */
    static char getKeyCode(Input::Key key, uint8_t keyModifiers);

    static bool isKeyHeld(Input::Key key);

    static bool isKeyDown(Input::Key key);

    static bool isKeyUp(Input::Key key);

    static bool isButtonHeld(std::string buttonName);

    static bool isButtonDown(std::string buttonName);

    static bool isButtonUp(std::string buttonName);

    static bool getAxis(std::string axisName);

    static bool isMouseButtonHeld(MouseButton button);

    static bool isMouseButtonDown(MouseButton button);

    static bool isMouseButtonUp(MouseButton button);

    static bool isMouseLocked();

    static void setMouseLocked(bool lock);

    static float getMouseScroll();

    ////////////////////////////////////////////////////////////////////////////////////////
    // Used only by input providers (Platform/Editor)
    static Input* getInput();
    void initialize();
    void update();
    void postGamepadConnectionEvent(uint32_t controllerIndex, bool connected);
    void postGamepadAxisEvent(uint32_t controllerIndex, GamepadAxis axis, int value);
    void postMouseMotionEvent(int mx, int my, int mz);
    void postMousePressEvent(int mx, int my, int mz, MouseButton button, bool down);
    void postKeyCharEvent(char chr);
    void postKeyPressEvent(Key key, uint8_t keyModifiers, bool down);

private:

    Input();

    ~Input();
};

}
