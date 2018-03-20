#include "Base.h"
#include "Platform.h"
#include "Game.h"

namespace gameplay
{

struct GamepadAxisDpadRemap
{
    Input::Key first;
    Input::Key second;
};

GamepadAxisDpadRemap __gamepadAxisDpad[] =
{
    { Input::Key::KEY_GAMEPAD_LEFT, Input::Key::KEY_GAMEPAD_RIGHT   },
    { Input::Key::KEY_GAMEPAD_UP,   Input::Key::KEY_GAMEPAD_DOWN    },
    { Input::Key::KEY_NONE,         Input::Key::KEY_NONE            },
    { Input::Key::KEY_GAMEPAD_LEFT, Input::Key::KEY_GAMEPAD_RIGHT   },
    { Input::Key::KEY_GAMEPAD_UP,   Input::Key::KEY_GAMEPAD_DOWN    },
    { Input::Key::KEY_NONE,         Input::Key::KEY_NONE            }
};

class Gamepad
{
public:
    Gamepad() : 
        joystick(nullptr),
        controller(nullptr), 
        jid(INT32_MAX)
    {
        std::memset(value, 0, sizeof(value));
        deadzone[Input::GAMEPAD_AXIS_LEFT_X] =
        deadzone[Input::GAMEPAD_AXIS_LEFT_Y] = 7849;
        deadzone[Input::GAMEPAD_AXIS_RIGHT_X] =
        deadzone[Input::GAMEPAD_AXIS_RIGHT_Y] = 8689;
        deadzone[Input::GAMEPAD_AXIS_LEFT_Z] =
        deadzone[Input::GAMEPAD_AXIS_RIGHT_Z] = 30;
    }

    void create(const SDL_JoyDeviceEvent& evt)
    {
        this->joystick = SDL_JoystickOpen(evt.which);
        SDL_Joystick* joystick = this->joystick;
        jid = SDL_JoystickInstanceID(joystick);
    }

    void create(const SDL_ControllerDeviceEvent& evt)
    {
        controller = SDL_GameControllerOpen(evt.which);
        SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
        jid = SDL_JoystickInstanceID(joystick);
    }

    void destroy()
    {
        if (controller != nullptr)
        {
            SDL_GameControllerClose(controller);
            controller = nullptr;
        }
        if (joystick != nullptr)
        {
            SDL_JoystickClose(joystick);
            joystick = nullptr;
        }
        jid = INT32_MAX;
    }

    bool filter(Input::GamepadAxis axis, int* value)
    {
        const int32_t oldValue = this->value[axis];
        const int32_t deadzone = this->deadzone[axis];
        int32_t newValue = *value;
        newValue = ((newValue > deadzone) || (newValue < -deadzone)) ? newValue : 0;
        this->value[axis] = newValue;
        *value = newValue;
        return oldValue != newValue;
    }

    void update(uint32_t controllerIndex, Input::GamepadAxis axis, int32_t value)
    {
        Input* input = Input::getInput();
        if (filter(axis, &value))
        {
            input->postGamepadAxisEvent(controllerIndex, axis, value);

            if (__gamepadAxisDpad[axis].first != Input::KEY_NONE)
            {
                if (value == 0)
                {
                    input->postKeyPressEvent(__gamepadAxisDpad[axis].first,  0, false);
                    input->postKeyPressEvent(__gamepadAxisDpad[axis].second, 0, false);
                }
                else
                {
                    input->postKeyPressEvent(value < 0 ? __gamepadAxisDpad[axis].first : __gamepadAxisDpad[axis].second, 0, true);
                }
            }
        }
    }

    SDL_Joystick* joystick;
    SDL_GameController* controller;
    SDL_JoystickID jid;
    int32_t value[Input::GAMEPAD_AXIS_COUNT];
    int32_t deadzone[Input::GAMEPAD_AXIS_COUNT];
};

Platform* Platform::getPlatform()
{
    static Platform platform;
    return &platform;
}

Platform::Platform() :
    _window(nullptr),
    _width(0),
    _height(0),
    _fullscreen(false),
    _nativeWindow(0),
    _nativeConnection(0),
    _running(false)
{
    std::memset(_translateKey, 0, sizeof(_translateKey));
    initTranslateKey(SDL_SCANCODE_ESCAPE,       Input::KEY_ESC);
    initTranslateKey(SDL_SCANCODE_RETURN,       Input::KEY_RETURN);
    initTranslateKey(SDL_SCANCODE_TAB,          Input::KEY_TAB);
    initTranslateKey(SDL_SCANCODE_BACKSPACE,    Input::KEY_BACKSPACE);
    initTranslateKey(SDL_SCANCODE_SPACE,        Input::KEY_SPACE);
    initTranslateKey(SDL_SCANCODE_UP,           Input::KEY_UP);
    initTranslateKey(SDL_SCANCODE_DOWN,         Input::KEY_DOWN);
    initTranslateKey(SDL_SCANCODE_LEFT,         Input::KEY_LEFT);
    initTranslateKey(SDL_SCANCODE_RIGHT,        Input::KEY_RIGHT);
    initTranslateKey(SDL_SCANCODE_PAGEUP,       Input::KEY_PAGE_UP);
    initTranslateKey(SDL_SCANCODE_PAGEDOWN,     Input::KEY_PAGE_DOWN);
    initTranslateKey(SDL_SCANCODE_HOME,         Input::KEY_HOME);
    initTranslateKey(SDL_SCANCODE_END,          Input::KEY_END);
    initTranslateKey(SDL_SCANCODE_PRINTSCREEN,  Input::KEY_PRINT);
    initTranslateKey(SDL_SCANCODE_KP_PLUS,      Input::KEY_PLUS);
    initTranslateKey(SDL_SCANCODE_EQUALS,       Input::KEY_PLUS);
    initTranslateKey(SDL_SCANCODE_KP_MINUS,     Input::KEY_MINUS);
    initTranslateKey(SDL_SCANCODE_MINUS,        Input::KEY_MINUS);
    initTranslateKey(SDL_SCANCODE_GRAVE,        Input::KEY_TILDE);
    initTranslateKey(SDL_SCANCODE_KP_COMMA,     Input::KEY_COMMA);
    initTranslateKey(SDL_SCANCODE_COMMA,        Input::KEY_COMMA);
    initTranslateKey(SDL_SCANCODE_KP_PERIOD,    Input::KEY_PERIOD);
    initTranslateKey(SDL_SCANCODE_PERIOD,       Input::KEY_PERIOD);
    initTranslateKey(SDL_SCANCODE_SLASH,        Input::KEY_SLASH);
    initTranslateKey(SDL_SCANCODE_F1,           Input::KEY_F1);
    initTranslateKey(SDL_SCANCODE_F2,           Input::KEY_F2);
    initTranslateKey(SDL_SCANCODE_F3,           Input::KEY_F3);
    initTranslateKey(SDL_SCANCODE_F4,           Input::KEY_F4);
    initTranslateKey(SDL_SCANCODE_F5,           Input::KEY_F5);
    initTranslateKey(SDL_SCANCODE_F6,           Input::KEY_F6);
    initTranslateKey(SDL_SCANCODE_F7,           Input::KEY_F7);
    initTranslateKey(SDL_SCANCODE_F8,           Input::KEY_F8);
    initTranslateKey(SDL_SCANCODE_F9,           Input::KEY_F9);
    initTranslateKey(SDL_SCANCODE_F10,          Input::KEY_F10);
    initTranslateKey(SDL_SCANCODE_F11,          Input::KEY_F11);
    initTranslateKey(SDL_SCANCODE_F12,          Input::KEY_F12);
    initTranslateKey(SDL_SCANCODE_KP_0,         Input::KEY_NUMPAD0);
    initTranslateKey(SDL_SCANCODE_KP_1,         Input::KEY_NUMPAD1);
    initTranslateKey(SDL_SCANCODE_KP_2,         Input::KEY_NUMPAD2);
    initTranslateKey(SDL_SCANCODE_KP_3,         Input::KEY_NUMPAD3);
    initTranslateKey(SDL_SCANCODE_KP_4,         Input::KEY_NUMPAD4);
    initTranslateKey(SDL_SCANCODE_KP_5,         Input::KEY_NUMPAD5);
    initTranslateKey(SDL_SCANCODE_KP_6,         Input::KEY_NUMPAD6);
    initTranslateKey(SDL_SCANCODE_KP_7,         Input::KEY_NUMPAD7);
    initTranslateKey(SDL_SCANCODE_KP_8,         Input::KEY_NUMPAD8);
    initTranslateKey(SDL_SCANCODE_KP_9,         Input::KEY_NUMPAD9);
    initTranslateKey(SDL_SCANCODE_0,            Input::KEY_0);
    initTranslateKey(SDL_SCANCODE_1,            Input::KEY_1);
    initTranslateKey(SDL_SCANCODE_2,            Input::KEY_2);
    initTranslateKey(SDL_SCANCODE_3,            Input::KEY_3);
    initTranslateKey(SDL_SCANCODE_4,            Input::KEY_4);
    initTranslateKey(SDL_SCANCODE_5,            Input::KEY_5);
    initTranslateKey(SDL_SCANCODE_6,            Input::KEY_6);
    initTranslateKey(SDL_SCANCODE_7,            Input::KEY_7);
    initTranslateKey(SDL_SCANCODE_8,            Input::KEY_8);
    initTranslateKey(SDL_SCANCODE_9,            Input::KEY_9);
    initTranslateKey(SDL_SCANCODE_A,            Input::KEY_A);
    initTranslateKey(SDL_SCANCODE_B,            Input::KEY_B);
    initTranslateKey(SDL_SCANCODE_C,            Input::KEY_C);
    initTranslateKey(SDL_SCANCODE_D,            Input::KEY_D);
    initTranslateKey(SDL_SCANCODE_E,            Input::KEY_E);
    initTranslateKey(SDL_SCANCODE_F,            Input::KEY_F);
    initTranslateKey(SDL_SCANCODE_G,            Input::KEY_G);
    initTranslateKey(SDL_SCANCODE_H,            Input::KEY_H);
    initTranslateKey(SDL_SCANCODE_I,            Input::KEY_I);
    initTranslateKey(SDL_SCANCODE_J,            Input::KEY_J);
    initTranslateKey(SDL_SCANCODE_K,            Input::KEY_K);
    initTranslateKey(SDL_SCANCODE_L,            Input::KEY_L);
    initTranslateKey(SDL_SCANCODE_M,            Input::KEY_M);
    initTranslateKey(SDL_SCANCODE_N,            Input::KEY_N);
    initTranslateKey(SDL_SCANCODE_O,            Input::KEY_O);
    initTranslateKey(SDL_SCANCODE_P,            Input::KEY_P);
    initTranslateKey(SDL_SCANCODE_Q,            Input::KEY_Q);
    initTranslateKey(SDL_SCANCODE_R,            Input::KEY_R);
    initTranslateKey(SDL_SCANCODE_S,            Input::KEY_S);
    initTranslateKey(SDL_SCANCODE_T,            Input::KEY_T);
    initTranslateKey(SDL_SCANCODE_U,            Input::KEY_U);
    initTranslateKey(SDL_SCANCODE_V,            Input::KEY_V);
    initTranslateKey(SDL_SCANCODE_W,            Input::KEY_W);
    initTranslateKey(SDL_SCANCODE_X,            Input::KEY_X);
    initTranslateKey(SDL_SCANCODE_Y,            Input::KEY_Y);
    initTranslateKey(SDL_SCANCODE_Z,            Input::KEY_Z);

    std::memset(_translateGamepad, uint8_t(Input::KEY_COUNT), sizeof(_translateGamepad) );
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_A,             Input::KEY_GAMEPAD_A);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_B,             Input::KEY_GAMEPAD_B);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_X,             Input::KEY_GAMEPAD_X);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_Y,             Input::KEY_GAMEPAD_Y);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSTICK,     Input::KEY_GAMEPAD_THUMB_LEFT);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSTICK,    Input::KEY_GAMEPAD_THUMB_RIGHT);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  Input::KEY_GAMEPAD_SHOULDER_LEFT);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Input::KEY_GAMEPAD_SHOULDER_RIGHT);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_UP,       Input::KEY_GAMEPAD_UP);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_DOWN,     Input::KEY_GAMEPAD_DOWN);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_LEFT,     Input::KEY_GAMEPAD_LEFT);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_RIGHT,    Input::KEY_GAMEPAD_RIGHT);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_BACK,          Input::KEY_GAMEPAD_BACK);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_START,         Input::KEY_GAMEPAD_START);
    initTranslateGamepad(SDL_CONTROLLER_BUTTON_GUIDE,         Input::KEY_GAMEPAD_GUIDE);

    std::memset(_translateGamepadAxis, uint8_t(Input::GAMEPAD_AXIS_COUNT), sizeof(_translateGamepadAxis) );
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTX,        Input::GAMEPAD_AXIS_LEFT_X);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTY,        Input::GAMEPAD_AXIS_LEFT_Y);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT,  Input::GAMEPAD_AXIS_LEFT_Z);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTX,       Input::GAMEPAD_AXIS_RIGHT_X);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTY,       Input::GAMEPAD_AXIS_RIGHT_Y);
    initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, Input::GAMEPAD_AXIS_RIGHT_Z);
}

Platform::~Platform()
{
}

int Platform::run()
{
    if (_running)
        return 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) < 0)
        GP_ERROR("Failed to initialize. SDL error=%s", SDL_GetError());

    Game* game = Game::getInstance();
    std::shared_ptr<Game::Config> config = game->getConfig();

    _width = config->width;
    _height = config->height;
    _fullscreen = config->fullscreen;

    // Create the SDL window based on game config
    _window = SDL_CreateWindow(config->title.c_str(), 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                _width, _height, 
                                _fullscreen ? 
                                (SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN) : 
                                (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

    // Get the native display,instance and windows
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (SDL_GetWindowWMInfo(_window, &wmi) )
    {
#if GP_PLATFORM_WINDOWS
        _nativeWindow = (uint64_t)wmi.info.win.window;
        _nativeConnection = (uint64_t)wmi.info.win.hinstance;
#elif GP_PLATFORM_LINUX
        _nativeWindow = (uint64_t)wmi.info.x11.window;
        _nativeConnection = (uint64_t)wmi.info.x11.display;
#elif GP_PLATFORM_MACOS
        _nativeWindow = (uint64_t)wmi.info.cocoa.window;
        _nativeConnection = 0;
#else
        GP_ERROR("Failed to initialize. Unsupported platform: %s");
#endif  
    }
    else
    {
        GP_WARN("Failed to get SDL window info. SDL error: %s", SDL_GetError());
    }

    // Enable file drops
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    game->onInitialize();

    Input* input = Input::getInput();
    input->initialize();
    Gamepad gamepads[GP_GAMEPADS_MAX];
    int mx, my, mz = 0.0f;
    bool exit = false;
    _running = true;

    // Start the event loop
    SDL_Event evt;
    while (!exit)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
                case SDL_QUIT:
                    {
                        game->exit();
                        exit = true;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    {
                        const SDL_MouseMotionEvent& motionEvt = evt.motion;
                        mx = motionEvt.x;
                        my = motionEvt.y;
                        input->postMouseMotionEvent(mx, my, mz);
                        game->onMouseMotion(mx, my, mz);
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    {
                        const SDL_MouseButtonEvent& mouseEvent = evt.button;
                        Input::MouseButton button;
                        switch (mouseEvent.button)
                        {
                        default:
                        case SDL_BUTTON_LEFT:
                            button = Input::MOUSE_BUTTON_LEFT;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            button = Input::MOUSE_BUTTON_MIDDLE;
                            break;
                        case SDL_BUTTON_RIGHT:
                            button = Input::MOUSE_BUTTON_RIGHT;
                            break;
                        }
                        input->postMousePressEvent(mouseEvent.x, mouseEvent.y, 0, button, 
                                                   mouseEvent.type == SDL_MOUSEBUTTONDOWN);
                        game->onMousePress(mouseEvent.x, mouseEvent.y, 0, button, 
                                           mouseEvent.type == SDL_MOUSEBUTTONDOWN);
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    {
                        const SDL_MouseWheelEvent& mouseEvent = evt.wheel;
                        mz += mouseEvent.y;
                        input->postMouseMotionEvent(mx, my, mz);
                        game->onMouseMotion(mx, my, mz);
                    }
                    break;

                case SDL_TEXTINPUT:
                    {
                        const SDL_TextInputEvent& textEvent = evt.text;
                        input->postKeyCharEvent(textEvent.text[0]);
                        game->onKeyChar(textEvent.text[0]);
                    }
                    break;

                case SDL_KEYDOWN:
                    {
                        const SDL_KeyboardEvent& keyEvent = evt.key;

                        uint8_t keyModifiers = translateKeyModifiers(keyEvent.keysym.mod);
                        Input::Key key = translateKey(keyEvent.keysym.scancode);
                        if (key == 0 && keyModifiers == 0)
                        {
                            keyModifiers = translateKeyModifierPress(keyEvent.keysym.scancode);
                        }

                        if (key == Input::KEY_ESC)
                        {
                            char pressedChar = 0x1b;
                            input->postKeyCharEvent(pressedChar);
                            game->onKeyChar(pressedChar);
                        }
                        else if (key == Input::KEY_RETURN)
                        {
                            char pressedChar = 0x0d;
                            input->postKeyCharEvent(pressedChar);
                            game->onKeyChar(pressedChar);
                        }
                        else if (key == Input::KEY_BACKSPACE)
                        {
                            uint8_t pressedChar = 0x08;
                            input->postKeyCharEvent(pressedChar);
                            game->onKeyChar(pressedChar);
                        }
                        input->postKeyPressEvent(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                        game->onKeyPress(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                    }
                    break;

                    case SDL_KEYUP:
                        {
                            const SDL_KeyboardEvent& keyEvent = evt.key;
                            uint8_t keyModifiers = translateKeyModifiers(keyEvent.keysym.mod);
                            Input::Key key = translateKey(keyEvent.keysym.scancode);
                            input->postKeyPressEvent(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                            game->onKeyPress(key, keyModifiers, keyEvent.state == SDL_PRESSED);
                        }
                        break;

                    case SDL_JOYDEVICEADDED:
                        {
                            const SDL_JoyDeviceEvent& joystickEvent = evt.jdevice;
                            gamepads[joystickEvent.which].create(joystickEvent);
                            input->postGamepadConnectionEvent(joystickEvent.which, true);
                            game->onGamepadConnection(joystickEvent.which, true);
                        }
                        break;

                    case SDL_JOYDEVICEREMOVED:
                        {
                            const SDL_JoyDeviceEvent& joystickEvent = evt.jdevice;
                            gamepads[joystickEvent.which].destroy();
                            input->postGamepadConnectionEvent(joystickEvent.which, false);
                            game->onGamepadConnection(joystickEvent.which, false);
                        }
                        break;

                    case SDL_CONTROLLERDEVICEADDED:
                        {
                            const SDL_ControllerDeviceEvent& controllerEvent = evt.cdevice;
                            gamepads[controllerEvent.which].create(controllerEvent);
                            input->postGamepadConnectionEvent(controllerEvent.which, true);
                            game->onGamepadConnection(controllerEvent.which, true);
                        }
                        break;

                    case SDL_CONTROLLERDEVICEREMAPPED:
                        break;

                    case SDL_CONTROLLERDEVICEREMOVED:
                        {
                            const SDL_ControllerDeviceEvent& controllerEvent = evt.cdevice;
                            gamepads[controllerEvent.which].destroy();
                            input->postGamepadConnectionEvent(controllerEvent.which, false);
                            game->onGamepadConnection(controllerEvent.which, false);
                        }
                        break;

                    case SDL_JOYAXISMOTION:
                        {
                            const SDL_JoyAxisEvent& joystickEvent = evt.jaxis;
                            Input::GamepadAxis axis = translateGamepadAxis(joystickEvent.axis);
                            gamepads[joystickEvent.which].update(joystickEvent.which, axis, joystickEvent.value);
                        }
                        break;

                    case SDL_CONTROLLERAXISMOTION:
                        {
                            const SDL_ControllerAxisEvent& controllerEvent = evt.caxis;
                            Input::GamepadAxis axis = translateGamepadAxis(controllerEvent.axis);
                            gamepads[controllerEvent.which].update(controllerEvent.which, axis, controllerEvent.value);
                        }
                        break;

                    case SDL_JOYBUTTONDOWN:
                    case SDL_JOYBUTTONUP:
                        {
                            const SDL_JoyButtonEvent& buttonEvent = evt.jbutton;
                            Input::Key key = translateGamepad(buttonEvent.button);
                            if (key != Input::KEY_COUNT)
                            {
                                input->postKeyPressEvent(key, 0, evt.type == SDL_JOYBUTTONDOWN);
                                game->onKeyPress(key, 0, evt.type == SDL_JOYBUTTONDOWN);
                            }
                        }
                        break;

                    case SDL_CONTROLLERBUTTONDOWN:
                    case SDL_CONTROLLERBUTTONUP:
                        {
                            const SDL_ControllerButtonEvent& buttonEvent = evt.cbutton;
                            Input::Key key = translateGamepad(buttonEvent.button);
                            if (key != Input::KEY_COUNT)
                            {
                                input->postKeyPressEvent(key, 0, evt.type == SDL_CONTROLLERBUTTONDOWN);
                                game->onKeyPress(key, 0, evt.type == SDL_CONTROLLERBUTTONDOWN);
                            }
                        }
                        break;

                    case SDL_WINDOWEVENT:
                        {
                            const SDL_WindowEvent& windowEvent = evt.window;
                            switch (windowEvent.event)
                            {
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                game->onResize(windowEvent.data1, windowEvent.data2);
                                break;
                            case SDL_WINDOWEVENT_SHOWN:
                            case SDL_WINDOWEVENT_HIDDEN:
                            case SDL_WINDOWEVENT_EXPOSED:
                            case SDL_WINDOWEVENT_MOVED:
                            case SDL_WINDOWEVENT_MINIMIZED:
                            case SDL_WINDOWEVENT_MAXIMIZED:
                            case SDL_WINDOWEVENT_RESTORED:
                            case SDL_WINDOWEVENT_ENTER:
                            case SDL_WINDOWEVENT_LEAVE:
                            case SDL_WINDOWEVENT_FOCUS_GAINED:
                            case SDL_WINDOWEVENT_FOCUS_LOST:
                                break;
                            case SDL_WINDOWEVENT_CLOSE:
                                game->exit();
                                exit = true;
                                break;
                            }
                        }
                        break;


                    case SDL_DROPFILE:
                        {
                            const SDL_DropEvent& dropEvent = evt.drop;
                            game->onDropFile(std::string(dropEvent.file));
                            SDL_free(dropEvent.file);
                        }
                        break;
            }
        }
        game->frame();
    }

    SDL_DestroyWindow(_window);
    SDL_Quit();

    return 0;
}

uint64_t Platform::getNativeWindow() const
{
    return _nativeWindow;
}

uint64_t Platform::getNativeConnection() const
{
    return _nativeConnection;
}

size_t Platform::getWidth() const
{
    return _width;
}

size_t Platform::getHeight() const
{
    return _height;
}

bool Platform::isFullscreen() const
{
    return _fullscreen;
}

SDL_Window * Platform::getSDLWindow() const
{
    return _window;
}

void Platform::initTranslateKey(uint16_t sdl, Input::Key key)
{
    _translateKey[sdl & 0xff] = (uint8_t)key;
}

Input::Key Platform::translateKey(SDL_Scancode sdl)
{
    return (Input::Key)_translateKey[sdl & 0xff];
}

uint8_t Platform::translateKeyModifiers(uint16_t sdl)
{
    uint8_t keyModifiers = 0;
    keyModifiers |= sdl & KMOD_LALT   ? Input::KEY_MODIFIER_LEFT_ALT    : 0;
    keyModifiers |= sdl & KMOD_RALT   ? Input::KEY_MODIFIER_RIGHT_ALT   : 0;
    keyModifiers |= sdl & KMOD_LCTRL  ? Input::KEY_MODIFIER_LEFT_CTRL   : 0;
    keyModifiers |= sdl & KMOD_RCTRL  ? Input::KEY_MODIFIER_RIGHT_CTRL  : 0;
    keyModifiers |= sdl & KMOD_LSHIFT ? Input::KEY_MODIFIER_LEFT_SHIFT  : 0;
    keyModifiers |= sdl & KMOD_RSHIFT ? Input::KEY_MODIFIER_RIGHT_SHIFT : 0;
    keyModifiers |= sdl & KMOD_LGUI   ? Input::KEY_MODIFIER_LEFT_META   : 0;
    keyModifiers |= sdl & KMOD_RGUI   ? Input::KEY_MODIFIER_RIGHT_META  : 0;
    return keyModifiers;
}

uint8_t Platform::translateKeyModifierPress(uint16_t key)
{
    uint8_t keyModifier;
    switch (key)
    {
        case SDL_SCANCODE_LALT:
            keyModifier = Input::KEY_MODIFIER_LEFT_ALT;
            break;
        case SDL_SCANCODE_RALT:
            keyModifier = Input::KEY_MODIFIER_RIGHT_ALT;
            break;
        case SDL_SCANCODE_LCTRL:
            keyModifier = Input::KEY_MODIFIER_LEFT_CTRL;
            break;
        case SDL_SCANCODE_RCTRL:
            keyModifier = Input::KEY_MODIFIER_RIGHT_CTRL;
            break;
        case SDL_SCANCODE_LSHIFT:
            keyModifier = Input::KEY_MODIFIER_LEFT_SHIFT;
            break;
        case SDL_SCANCODE_RSHIFT:
            keyModifier = Input::KEY_MODIFIER_RIGHT_SHIFT;
            break;
        case SDL_SCANCODE_LGUI:
            keyModifier = Input::KEY_MODIFIER_LEFT_META;
            break;
        case SDL_SCANCODE_RGUI:
            keyModifier = Input::KEY_MODIFIER_RIGHT_META;
            break;
        default:
            keyModifier = 0;
            break;
    }

    return keyModifier;
}

void Platform::initTranslateGamepad(uint8_t sdl, Input::Key button)
{
    _translateGamepad[sdl] = button;
}

Input::Key Platform::translateGamepad(uint8_t sdl)
{
    return Input::Key(_translateGamepad[sdl]);
}

void Platform::initTranslateGamepadAxis(uint8_t sdl, Input::GamepadAxis axis)
{
    _translateGamepadAxis[sdl] = uint8_t(axis);
}

Input::GamepadAxis Platform::translateGamepadAxis(uint8_t sdl)
{
    return Input::GamepadAxis(_translateGamepadAxis[sdl]);
}


extern void print(const char* format, ...)
{
#ifdef GP_PLATFORM_WINDOWS
    va_list argptr;
    va_start(argptr, format);
    int sz = vfprintf(stderr, format, argptr);
    if (sz > 0)
    {
        char* buf = new char[sz + 1];
        vsprintf(buf, format, argptr);
        buf[sz] = 0;
        OutputDebugStringA(buf);
        GP_SAFE_DELETE_ARRAY(buf);
    }
    va_end(argptr);
#elif GP_PLATOFRM_LINUX || GP_PLATFORM_MACOS
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
#endif
}

}
