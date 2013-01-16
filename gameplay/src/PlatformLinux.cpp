#ifdef __linux__

#include "Base.h"
#include "Platform.h"
#include "FileSystem.h"
#include "Game.h"
#include "Form.h"
#include "ScriptController.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <sys/time.h>
#include <GL/glxew.h>
#include <poll.h>

#define TOUCH_COUNT_MAX     4

using namespace std;

struct timespec __timespec;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static float __pitch;
static float __roll;
static bool __mouseCaptured = false;
static float __mouseCapturePointX = 0;
static float __mouseCapturePointY = 0;
static bool __cursorVisible = true;
static Display* __display;
static Window   __window;
static int __windowSize[2];
static GLXContext __context;
static Window __attachToWindow;
static Atom __atomWmDeleteWindow;

namespace gameplay
{

// Gets the Keyboard::Key enumeration constant that corresponds to the given X11 key symbol.
static Keyboard::Key getKey(KeySym sym)
{
    switch (sym)
    {
    case XK_Sys_Req:
        return Keyboard::KEY_SYSREQ;
    case XK_Break:
        return Keyboard::KEY_BREAK;
    case XK_Menu :
        return Keyboard::KEY_MENU;
    case XK_KP_Enter:
        return Keyboard::KEY_KP_ENTER;
    case XK_Pause:
        return Keyboard::KEY_PAUSE;
    case XK_Scroll_Lock:
        return Keyboard::KEY_SCROLL_LOCK;
    case XK_Print:
        return Keyboard::KEY_PRINT;
    case XK_Escape:
        return Keyboard::KEY_ESCAPE;
    case XK_BackSpace:
        return Keyboard::KEY_BACKSPACE;
    case XK_Tab:
        return Keyboard::KEY_TAB;
    case XK_Return:
        return Keyboard::KEY_RETURN;
    case XK_Caps_Lock:
        return Keyboard::KEY_CAPS_LOCK;
    case XK_Shift_L:
    case XK_Shift_R:
        return Keyboard::KEY_SHIFT;
    case XK_Control_L:
    case XK_Control_R:
        return Keyboard::KEY_CTRL;
    case XK_Alt_L:
    case XK_Alt_R:
        return Keyboard::KEY_ALT;
    case XK_Hyper_L:
    case XK_Hyper_R:
        return Keyboard::KEY_HYPER;
    case XK_Insert:
        return Keyboard::KEY_INSERT;
    case XK_Home:
        return Keyboard::KEY_HOME;
    case XK_Page_Up:
        return Keyboard::KEY_PG_UP;
    case XK_Delete:
        return Keyboard::KEY_DELETE;
    case XK_End:
        return Keyboard::KEY_END;
    case XK_Page_Down:
        return Keyboard::KEY_PG_DOWN;
    case XK_Left:
        return Keyboard::KEY_LEFT_ARROW;
    case XK_Right:
        return Keyboard::KEY_RIGHT_ARROW;
    case XK_Up:
        return Keyboard::KEY_UP_ARROW;
    case XK_Down:
        return Keyboard::KEY_DOWN_ARROW;
    case XK_Num_Lock:
        return Keyboard::KEY_NUM_LOCK;
    case XK_KP_Add:
        return Keyboard::KEY_KP_PLUS;
    case XK_KP_Subtract:
        return Keyboard::KEY_KP_MINUS;
    case XK_KP_Multiply:
        return Keyboard::KEY_KP_MULTIPLY;
    case XK_KP_Divide:
        return Keyboard::KEY_KP_DIVIDE;
    case XK_KP_Home:
        return Keyboard::KEY_KP_HOME;
    case XK_KP_Up:
        return Keyboard::KEY_KP_UP;
    case XK_KP_Page_Up:
        return Keyboard::KEY_KP_PG_UP;
    case XK_KP_Left:
        return Keyboard::KEY_KP_LEFT;
    case XK_KP_5:
        return Keyboard::KEY_KP_FIVE;
    case XK_KP_Right:
        return Keyboard::KEY_KP_RIGHT;
    case XK_KP_End:
        return Keyboard::KEY_KP_END;
    case XK_KP_Down:
        return Keyboard::KEY_KP_DOWN;
    case XK_KP_Page_Down:
        return Keyboard::KEY_KP_PG_DOWN;
    case XK_KP_Insert:
        return Keyboard::KEY_KP_INSERT;
    case XK_KP_Delete:
        return Keyboard::KEY_KP_DELETE;
    case XK_F1:
        return Keyboard::KEY_F1;
    case XK_F2:
        return Keyboard::KEY_F2;
    case XK_F3:
        return Keyboard::KEY_F3;
    case XK_F4:
        return Keyboard::KEY_F4;
    case XK_F5:
        return Keyboard::KEY_F5;
    case XK_F6:
        return Keyboard::KEY_F6;
    case XK_F7:
        return Keyboard::KEY_F7;
    case XK_F8:
        return Keyboard::KEY_F8;
    case XK_F9:
        return Keyboard::KEY_F9;
    case XK_F10:
        return Keyboard::KEY_F10;
    case XK_F11:
        return Keyboard::KEY_F11;
    case XK_F12:
        return Keyboard::KEY_F12;
    case XK_KP_Space:
    case XK_space:
        return Keyboard::KEY_SPACE;
    case XK_parenright:
        return Keyboard::KEY_RIGHT_PARENTHESIS;
    case XK_0:
        return Keyboard::KEY_ZERO;
    case XK_exclam:
        return Keyboard::KEY_EXCLAM;
    case XK_1:
        return Keyboard::KEY_ONE;
    case XK_at:
        return Keyboard::KEY_AT;
    case XK_2:
        return Keyboard::KEY_TWO;
    case XK_numbersign:
        return Keyboard::KEY_NUMBER;
    case XK_3:
        return Keyboard::KEY_THREE;
    case XK_dollar:
        return Keyboard::KEY_DOLLAR;
    case XK_4:
        return Keyboard::KEY_FOUR;
    case XK_percent:
    case XK_asciicircum :
        return Keyboard::KEY_CIRCUMFLEX;
        return Keyboard::KEY_PERCENT;
    case XK_5:
        return Keyboard::KEY_FIVE;
    case XK_6:
        return Keyboard::KEY_SIX;
    case XK_ampersand:
        return Keyboard::KEY_AMPERSAND;
    case XK_7:
        return Keyboard::KEY_SEVEN;
    case XK_asterisk:
        return Keyboard::KEY_ASTERISK;
    case XK_8:
        return Keyboard::KEY_EIGHT;
    case XK_parenleft:
        return Keyboard::KEY_LEFT_PARENTHESIS;
    case XK_9:
        return Keyboard::KEY_NINE;
    case XK_equal:
        return Keyboard::KEY_EQUAL;
    case XK_plus:
        return Keyboard::KEY_PLUS;
    case XK_less:
        return Keyboard::KEY_LESS_THAN;
    case XK_comma:
        return Keyboard::KEY_COMMA;
    case XK_underscore:
        return Keyboard::KEY_UNDERSCORE;
    case XK_minus:
        return Keyboard::KEY_MINUS;
    case XK_greater:
        return Keyboard::KEY_GREATER_THAN;
    case XK_period:
        return Keyboard::KEY_PERIOD;
    case XK_colon:
        return Keyboard::KEY_COLON;
    case XK_semicolon:
        return Keyboard::KEY_SEMICOLON;
    case XK_question:
        return Keyboard::KEY_QUESTION;
    case XK_slash:
        return Keyboard::KEY_SLASH;
    case XK_grave:
        return Keyboard::KEY_GRAVE;
    case XK_asciitilde:
        return Keyboard::KEY_TILDE;
    case XK_braceleft:
        return Keyboard::KEY_LEFT_BRACE;
    case XK_bracketleft:
        return Keyboard::KEY_LEFT_BRACKET;
    case XK_bar:
        return Keyboard::KEY_BAR;
    case XK_backslash:
        return Keyboard::KEY_BACK_SLASH;
    case XK_braceright:
        return Keyboard::KEY_RIGHT_BRACE;
    case XK_bracketright:
        return Keyboard::KEY_RIGHT_BRACKET;
    case XK_quotedbl:
        return Keyboard::KEY_QUOTE;
    case XK_apostrophe:
        return Keyboard::KEY_APOSTROPHE;
    case XK_EuroSign:
        return Keyboard::KEY_EURO;
    case XK_sterling:
        return Keyboard::KEY_POUND;
    case XK_yen:
        return Keyboard::KEY_YEN;
    case XK_periodcentered:
        return Keyboard::KEY_MIDDLE_DOT;
    case XK_A:
        return Keyboard::KEY_CAPITAL_A;
    case XK_a:
        return Keyboard::KEY_A;
    case XK_B:
        return Keyboard::KEY_CAPITAL_B;
    case XK_b:
        return Keyboard::KEY_B;
    case XK_C:
        return Keyboard::KEY_CAPITAL_C;
    case XK_c:
        return Keyboard::KEY_C;
    case XK_D:
        return Keyboard::KEY_CAPITAL_D;
    case XK_d:
        return Keyboard::KEY_D;
    case XK_E:
        return Keyboard::KEY_CAPITAL_E;
    case XK_e:
        return Keyboard::KEY_E;
    case XK_F:
        return Keyboard::KEY_CAPITAL_F;
    case XK_f:
        return Keyboard::KEY_F;
    case XK_G:
        return Keyboard::KEY_CAPITAL_G;
    case XK_g:
        return Keyboard::KEY_G;
    case XK_H:
        return Keyboard::KEY_CAPITAL_H;
    case XK_h:
        return Keyboard::KEY_H;
    case XK_I:
        return Keyboard::KEY_CAPITAL_I;
    case XK_i:
        return Keyboard::KEY_I;
    case XK_J:
        return Keyboard::KEY_CAPITAL_J;
    case XK_j:
        return Keyboard::KEY_J;
    case XK_K:
        return Keyboard::KEY_CAPITAL_K;
    case XK_k:
        return Keyboard::KEY_K;
    case XK_L:
        return Keyboard::KEY_CAPITAL_L;
    case XK_l:
        return Keyboard::KEY_L;
    case XK_M:
        return Keyboard::KEY_CAPITAL_M;
    case XK_m:
        return Keyboard::KEY_M;
    case XK_N:
        return Keyboard::KEY_CAPITAL_N;
    case XK_n:
        return Keyboard::KEY_N;
    case XK_O:
        return Keyboard::KEY_CAPITAL_O;
    case XK_o:
        return Keyboard::KEY_O;
    case XK_P:
        return Keyboard::KEY_CAPITAL_P;
    case XK_p:
        return Keyboard::KEY_P;
    case XK_Q:
        return Keyboard::KEY_CAPITAL_Q;
    case XK_q:
        return Keyboard::KEY_Q;
    case XK_R:
        return Keyboard::KEY_CAPITAL_R;
    case XK_r:
        return Keyboard::KEY_R;
    case XK_S:
        return Keyboard::KEY_CAPITAL_S;
    case XK_s:
        return Keyboard::KEY_S;
    case XK_T:
        return Keyboard::KEY_CAPITAL_T;
    case XK_t:
        return Keyboard::KEY_T;
    case XK_U:
        return Keyboard::KEY_CAPITAL_U;
    case XK_u:
        return Keyboard::KEY_U;
    case XK_V:
        return Keyboard::KEY_CAPITAL_V;
    case XK_v:
        return Keyboard::KEY_V;
    case XK_W:
        return Keyboard::KEY_CAPITAL_W;
    case XK_w:
        return Keyboard::KEY_W;
    case XK_X:
        return Keyboard::KEY_CAPITAL_X;
    case XK_x:
        return Keyboard::KEY_X;
    case XK_Y:
        return Keyboard::KEY_CAPITAL_Y;
    case XK_y:
        return Keyboard::KEY_Y;
    case XK_Z:
        return Keyboard::KEY_CAPITAL_Z;
    case XK_z:
        return Keyboard::KEY_Z;
    default:
        return Keyboard::KEY_NONE;
    }
}

/**
 * Returns the unicode value for the given keycode or zero if the key is not a valid printable character.
 */
static int getUnicode(Keyboard::Key key)
{

    switch (key)
    {
    case Keyboard::KEY_BACKSPACE:
        return 0x0008;
    case Keyboard::KEY_TAB:
        return 0x0009;
    case Keyboard::KEY_RETURN:
    case Keyboard::KEY_KP_ENTER:
        return 0x000A;
    case Keyboard::KEY_ESCAPE:
        return 0x001B;
    case Keyboard::KEY_SPACE:
    case Keyboard::KEY_EXCLAM:
    case Keyboard::KEY_QUOTE:
    case Keyboard::KEY_NUMBER:
    case Keyboard::KEY_DOLLAR:
    case Keyboard::KEY_PERCENT:
    case Keyboard::KEY_CIRCUMFLEX:
    case Keyboard::KEY_AMPERSAND:
    case Keyboard::KEY_APOSTROPHE:
    case Keyboard::KEY_LEFT_PARENTHESIS:
    case Keyboard::KEY_RIGHT_PARENTHESIS:
    case Keyboard::KEY_ASTERISK:
    case Keyboard::KEY_PLUS:
    case Keyboard::KEY_COMMA:
    case Keyboard::KEY_MINUS:
    case Keyboard::KEY_PERIOD:
    case Keyboard::KEY_SLASH:
    case Keyboard::KEY_ZERO:
    case Keyboard::KEY_ONE:
    case Keyboard::KEY_TWO:
    case Keyboard::KEY_THREE:
    case Keyboard::KEY_FOUR:
    case Keyboard::KEY_FIVE:
    case Keyboard::KEY_SIX:
    case Keyboard::KEY_SEVEN:
    case Keyboard::KEY_EIGHT:
    case Keyboard::KEY_NINE:
    case Keyboard::KEY_COLON:
    case Keyboard::KEY_SEMICOLON:
    case Keyboard::KEY_LESS_THAN:
    case Keyboard::KEY_EQUAL:
    case Keyboard::KEY_GREATER_THAN:
    case Keyboard::KEY_QUESTION:
    case Keyboard::KEY_AT:
    case Keyboard::KEY_CAPITAL_A:
    case Keyboard::KEY_CAPITAL_B:
    case Keyboard::KEY_CAPITAL_C:
    case Keyboard::KEY_CAPITAL_D:
    case Keyboard::KEY_CAPITAL_E:
    case Keyboard::KEY_CAPITAL_F:
    case Keyboard::KEY_CAPITAL_G:
    case Keyboard::KEY_CAPITAL_H:
    case Keyboard::KEY_CAPITAL_I:
    case Keyboard::KEY_CAPITAL_J:
    case Keyboard::KEY_CAPITAL_K:
    case Keyboard::KEY_CAPITAL_L:
    case Keyboard::KEY_CAPITAL_M:
    case Keyboard::KEY_CAPITAL_N:
    case Keyboard::KEY_CAPITAL_O:
    case Keyboard::KEY_CAPITAL_P:
    case Keyboard::KEY_CAPITAL_Q:
    case Keyboard::KEY_CAPITAL_R:
    case Keyboard::KEY_CAPITAL_S:
    case Keyboard::KEY_CAPITAL_T:
    case Keyboard::KEY_CAPITAL_U:
    case Keyboard::KEY_CAPITAL_V:
    case Keyboard::KEY_CAPITAL_W:
    case Keyboard::KEY_CAPITAL_X:
    case Keyboard::KEY_CAPITAL_Y:
    case Keyboard::KEY_CAPITAL_Z:
    case Keyboard::KEY_LEFT_BRACKET:
    case Keyboard::KEY_BACK_SLASH:
    case Keyboard::KEY_RIGHT_BRACKET:
    case Keyboard::KEY_UNDERSCORE:
    case Keyboard::KEY_GRAVE:
    case Keyboard::KEY_A:
    case Keyboard::KEY_B:
    case Keyboard::KEY_C:
    case Keyboard::KEY_D:
    case Keyboard::KEY_E:
    case Keyboard::KEY_F:
    case Keyboard::KEY_G:
    case Keyboard::KEY_H:
    case Keyboard::KEY_I:
    case Keyboard::KEY_J:
    case Keyboard::KEY_K:
    case Keyboard::KEY_L:
    case Keyboard::KEY_M:
    case Keyboard::KEY_N:
    case Keyboard::KEY_O:
    case Keyboard::KEY_P:
    case Keyboard::KEY_Q:
    case Keyboard::KEY_R:
    case Keyboard::KEY_S:
    case Keyboard::KEY_T:
    case Keyboard::KEY_U:
    case Keyboard::KEY_V:
    case Keyboard::KEY_W:
    case Keyboard::KEY_X:
    case Keyboard::KEY_Y:
    case Keyboard::KEY_Z:
    case Keyboard::KEY_LEFT_BRACE:
    case Keyboard::KEY_BAR:
    case Keyboard::KEY_RIGHT_BRACE:
    case Keyboard::KEY_TILDE:
        return key;
    default:
        return 0;
    }
}

extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

Platform::Platform(Game* game) : _game(game)
{
}

Platform::~Platform()
{
}

Platform* Platform::create(Game* game, void* attachToWindow)
{

    GP_ASSERT(game);
    
    __attachToWindow = (Window)attachToWindow;
    FileSystem::setResourcePath("./");
    Platform* platform = new Platform(game);
    
    // Get the display and initialize
    __display = XOpenDisplay(NULL);
    if (__display == NULL)
    {
        perror("XOpenDisplay");
        return NULL;
    }
     
    // Get the window configuration values
    const char *title = NULL;
    int __x = 0, __y = 0, __width = 1280, __height = 800;
    bool fullscreen = false;
    if (game->getConfig())
    {
        Properties* config = game->getConfig()->getNamespace("window", true);
        if (config)
        {
            // Read window title.
            title = config->getString("title");

            // Read window rect.
            int x = config->getInt("x");
            int y = config->getInt("y");
            int width = config->getInt("width");
            int height = config->getInt("height");
            fullscreen = config->getBool("fullscreen");

            if (fullscreen && width == 0 && height == 0)
            {
                // Use the screen resolution if fullscreen is true but width and height were not set in the config
                int screen_num = DefaultScreen(__display);
                width = DisplayWidth(__display, screen_num);
                height = DisplayHeight(__display, screen_num);
            }
            if (x != 0) __x = x;
            if (y != 0) __y = y;
            if (width != 0) __width = width;
            if (height != 0) __height = height;
        }
    }

    // GLX version
    GLint majorGLX, minorGLX = 0;
    glXQueryVersion(__display, &majorGLX, &minorGLX);
    if (majorGLX == 1 && minorGLX < 2)
    {
        perror("GLX 1.2 or greater is required.");
        XCloseDisplay(__display);
        return NULL;
    }
    else
    {
        printf( "GLX version: %d.%d\n", majorGLX , minorGLX);
    }
    
    // Get the GLX Functions
    glXCreateContextAttribsARB = (GLXContext(*)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct,  const int *attrib_list))glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
    glXChooseFBConfig = (GLXFBConfig*(*)(Display *dpy, int screen, const int *attrib_list, int *nelements))glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");
    glXGetVisualFromFBConfig = (XVisualInfo*(*)(Display *dpy, GLXFBConfig config))glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
    glXGetFBConfigAttrib = (int(*)(Display *dpy, GLXFBConfig config, int attribute, int *value))glXGetProcAddressARB((GLubyte*)"glXGetFBConfigAttrib");
    glXSwapIntervalEXT = (void(*)(Display* dpy, GLXDrawable drawable, int interval))glXGetProcAddressARB((GLubyte*)"glXSwapIntervalEXT");
    glXSwapIntervalMESA = (int(*)(unsigned int interval))glXGetProcAddressARB((GLubyte*)"glXSwapIntervalMESA");

    // Get the configs
    int configAttribs[] = 
    {
        GLX_RENDER_TYPE,    GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
        GLX_X_RENDERABLE,   True,
        GLX_DEPTH_SIZE,     24,
        GLX_STENCIL_SIZE,   8,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_DOUBLEBUFFER,   True,
        0
    };
    GLXFBConfig* configs;
    int configCount = 0;
    configs = glXChooseFBConfig(__display, DefaultScreen(__display), configAttribs, &configCount);
    if ( configCount == 0 || configs == 0 )
    {
        perror( "glXChooseFBConfig" );
        return NULL;
    }

    // Create the windows
    XVisualInfo* visualInfo;
    visualInfo = glXGetVisualFromFBConfig(__display, configs[0]);
    
    XSetWindowAttributes winAttribs;
    long eventMask;
    eventMask = ExposureMask | VisibilityChangeMask | StructureNotifyMask |
                KeyPressMask | KeyReleaseMask | PointerMotionMask |
                ButtonPressMask | ButtonReleaseMask |
                EnterWindowMask | LeaveWindowMask;
    winAttribs.event_mask = eventMask;
    winAttribs.border_pixel = 0;
    winAttribs.bit_gravity = StaticGravity;
    winAttribs.colormap = XCreateColormap(__display, RootWindow(__display, visualInfo->screen), visualInfo->visual, AllocNone);

    GLint winMask;
    winMask = CWBorderPixel | CWBitGravity | CWEventMask| CWColormap;
   
    __window = XCreateWindow(__display, DefaultRootWindow(__display), __x, __y, __width, __height, 0,
                            visualInfo->depth, InputOutput, visualInfo->visual, winMask,
                            &winAttribs);

    // Tell the window manager that it should send the delete window notification through ClientMessage
    __atomWmDeleteWindow = XInternAtom(__display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(__display, __window, &__atomWmDeleteWindow, 1);

    XMapWindow(__display, __window);

    // Send fullscreen atom message to the window; most window managers respect WM_STATE messages
    // Note: fullscreen mode will use native desktop resolution and won't care about width/height specified
    if (fullscreen)
    {
        XEvent xev;
        Atom atomWm_state = XInternAtom(__display, "_NET_WM_STATE", False);
        Atom atomFullscreen = XInternAtom(__display, "_NET_WM_STATE_FULLSCREEN", False);

        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.window = __window;
        xev.xclient.message_type = atomWm_state;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 1;
        xev.xclient.data.l[1] = atomFullscreen;
        xev.xclient.data.l[2] = 0;

        XSendEvent(__display, DefaultRootWindow(__display), false, SubstructureNotifyMask | SubstructureRedirectMask, &xev);
    }
    
    XStoreName(__display, __window, title ? title : "");

    __context = glXCreateContext(__display, visualInfo, NULL, True);
    if (!__context)
    {
        perror("glXCreateContext");
        return NULL;
    }
    glXMakeCurrent(__display, __window, __context);

    // Use OpenGL 2.x with GLEW
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        perror("glewInit");
        return NULL;
    }

    // GL Version
    int versionGL[2] = {-1, -1};
    glGetIntegerv(GL_MAJOR_VERSION, versionGL);
    glGetIntegerv(GL_MINOR_VERSION, versionGL + 1);
    printf("GL version: %d.%d\n", versionGL[0], versionGL[1]);

    // TODO: Get this workings
    if (glXSwapIntervalEXT)
        glXSwapIntervalEXT(__display, __window, __vsync ? 1 : 0);
    else if(glXSwapIntervalMESA)
        glXSwapIntervalMESA(__vsync ? 1 : 0);
 
    return platform;
}

void cleanupX11()
{
    if (__display)
    {
        glXMakeCurrent(__display, None, NULL);

        if (__context)
             glXDestroyContext(__display, __context);
        if (__window)
             XDestroyWindow(__display, __window);

         XCloseDisplay(__display);
    }
}

double timespec2millis(struct timespec *a)
{
    GP_ASSERT(a);
    return (1000.0 * a->tv_sec) + (0.000001 * a->tv_nsec);
}

void updateWindowSize()
{
    GP_ASSERT(__display);
    GP_ASSERT(__window);
    XWindowAttributes windowAttrs;
    XGetWindowAttributes(__display, __window, &windowAttrs);
    __windowSize[0] = windowAttrs.width;
    __windowSize[1] = windowAttrs.height;
}

int Platform::enterMessagePump()
{
    GP_ASSERT(_game);

    updateWindowSize();
 
    static const float ACCELEROMETER_X_FACTOR = 90.0f / __windowSize[0];
    static const float ACCELEROMETER_Y_FACTOR = 90.0f / __windowSize[1];
    static int lx = 0;
    static int ly = 0;
    static bool shiftDown = false;
    static bool capsOn = false;
    static XEvent evt;

    // Get the initial time.
    clock_gettime(CLOCK_REALTIME, &__timespec);
    __timeStart = timespec2millis(&__timespec);
    __timeAbsolute = 0L;

    // Run the game.
    _game->run();

    // Setup select for message handling (to allow non-blocking)
    int x11_fd = ConnectionNumber(__display);
    
    pollfd xpolls[1];
    xpolls[0].fd = x11_fd;
    xpolls[0].events = POLLIN|POLLPRI;
  
    // Message loop.
    while (true)
    {
        poll( xpolls, 1, 16 );
        // handle all pending events in one block
        while (XPending(__display))
        {
           XNextEvent(__display, &evt);
        
            switch (evt.type)
            {
            case ClientMessage:
                {
                    // Handle destroy window message correctly
                    if (evt.xclient.data.l[0] == __atomWmDeleteWindow)
                    {
                        _game->exit();
                    }
                }
                break;
            case DestroyNotify :
                {
                    cleanupX11();
                    exit(0);
                }
                break;

            case Expose:
                {
                    updateWindowSize();
                }
                break;

            case KeyPress:
                {
                    KeySym sym = XLookupKeysym(&evt.xkey, (evt.xkey.state & shiftDown) ? 1 : 0);


                    //TempSym needed because XConvertCase operates on two keysyms: One lower and the other upper, we are only interested in the upper case
                    KeySym tempSym;
                    if (capsOn && !shiftDown)
                        XConvertCase(sym,  &tempSym, &sym);

                    Keyboard::Key key = getKey(sym);
                    gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_PRESS, key);

                    if (key == Keyboard::KEY_CAPS_LOCK)
                        capsOn = !capsOn;
                    if (key == Keyboard::KEY_SHIFT)
                        shiftDown = true;

                    if (int character = getUnicode(key))
                        gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_CHAR, character);

                }
                break;

            case KeyRelease:
                {
                    //detect and drop repeating keystrokes (no other way to do this using the event interface)
                    XEvent next;
                    if ( XPending(__display) )
                    {
                        XPeekEvent(__display,&next);
                        if ( next.type == KeyPress
                            && next.xkey.time == evt.xkey.time
                            && next.xkey.keycode == evt.xkey.keycode )
                        {
                            XNextEvent(__display,&next);
                            continue;
                        }
                    }
                    
                    KeySym sym = XLookupKeysym(&evt.xkey, 0);
                    Keyboard::Key key = getKey(sym);
                    gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_RELEASE, key);

                    if (key == Keyboard::KEY_SHIFT)
                        shiftDown = false;
                }
                break;

            case ButtonPress:
                {
                    gameplay::Mouse::MouseEvent mouseEvt;
                    switch (evt.xbutton.button)
                    {
                        case 1:
                            mouseEvt = gameplay::Mouse::MOUSE_PRESS_LEFT_BUTTON;
                            break;
                        case 2:
                            mouseEvt = gameplay::Mouse::MOUSE_PRESS_MIDDLE_BUTTON;
                            break;
                        case 3:
                            mouseEvt = gameplay::Mouse::MOUSE_PRESS_RIGHT_BUTTON;
                            break;
                        case 4:
                        case 5:
                            gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_WHEEL,
                                                                   evt.xbutton.x, evt.xbutton.y,
                                                                   evt.xbutton.button == Button4 ? 1 : -1);
                            break;
                        default:
                            break;
                    }
                    if (!gameplay::Platform::mouseEventInternal(mouseEvt, evt.xbutton.x, evt.xbutton.y, 0))
                    {
                        gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_PRESS, evt.xbutton.x, evt.xbutton.y, 0);
                    }
                }
                break;

            case ButtonRelease:
                {
                    gameplay::Mouse::MouseEvent mouseEvt;
                    switch (evt.xbutton.button)
                    {
                        case 1:
                            mouseEvt = gameplay::Mouse::MOUSE_RELEASE_LEFT_BUTTON;
                            break;
                        case 2:
                            mouseEvt = gameplay::Mouse::MOUSE_RELEASE_MIDDLE_BUTTON;
                            break;
                        case 3:
                            mouseEvt = gameplay::Mouse::MOUSE_RELEASE_RIGHT_BUTTON;
                            break;
                        default:
                            break;
                    }
                    if (!gameplay::Platform::mouseEventInternal(mouseEvt, evt.xbutton.x, evt.xbutton.y, 0))
                    {
                        gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, evt.xbutton.x, evt.xbutton.y, 0);
                    }
                }
                break;
        
            case MotionNotify:
                {
                    int x = evt.xmotion.x;
                    int y = evt.xmotion.y;

                    if (__mouseCaptured)
                    {
                        if (x == __mouseCapturePointX && y == __mouseCapturePointY)
                        {
                            // Discard the first MotionNotify following capture
                            // since it contains bogus x,y data.
                            break;
                        }

                        // Convert to deltas
                        x -= __mouseCapturePointX;
                        y -= __mouseCapturePointY;

                        // Warp mouse back to center of screen.
                        XWarpPointer(__display, None, __window, 0, 0, 0, 0, __mouseCapturePointX, __mouseCapturePointY);
                    }

                    if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_MOVE, x, y, 0))
                    {
                        if (evt.xmotion.state & Button1Mask)
                        {
                            gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_MOVE, x, y, 0);
                        }
                        else if (evt.xmotion.state & Button3Mask)
                        {
                            // Update the pitch and roll by adding the scaled deltas.
                            __roll += (float)(x - lx) * ACCELEROMETER_X_FACTOR;
                            __pitch += -(float)(y - ly) * ACCELEROMETER_Y_FACTOR;

                            // Clamp the values to the valid range.
                            __roll = max(min(__roll, 90.0f), -90.0f);
                            __pitch = max(min(__pitch, 90.0f), -90.0f);

                            // Update the last X/Y values.
                            lx = x;
                            ly = y;
                        }
                    }
                }
                break;
            
            default:
                break;
            }
        }

        if (_game)
        {
            // Game state will be uninitialized if game was closed through Game::exit()
            if (_game->getState() == Game::UNINITIALIZED)
                break;
            
            _game->frame();
        }

        glXSwapBuffers(__display, __window);
    }

    cleanupX11();

    return 0;
}
    
void Platform::signalShutdown()
{
    // nothing to do  
}

bool Platform::canExit()
{
    return true;
}
    
unsigned int Platform::getDisplayWidth()
{
    return __windowSize[0];
}

unsigned int Platform::getDisplayHeight()
{
    return __windowSize[1];
}

double Platform::getAbsoluteTime()
{

    clock_gettime(CLOCK_REALTIME, &__timespec);
    double now = timespec2millis(&__timespec);
    __timeAbsolute = now - __timeStart;

    return __timeAbsolute;
}

void Platform::setAbsoluteTime(double time)
{
    __timeAbsolute = time;
}

bool Platform::isVsync()
{
    return __vsync;
}

void Platform::setVsync(bool enable)
{
    if (glXSwapIntervalEXT)
        glXSwapIntervalEXT(__display, __window, __vsync ? 1 : 0);
    else if(glXSwapIntervalMESA)
        glXSwapIntervalMESA(__vsync ? 1 : 0);

    __vsync = enable;
}

void Platform::swapBuffers()
{
    glXSwapBuffers(__display, __window);
}

void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

void Platform::setMultiTouch(bool enabled)
{
    // not supported
}

bool Platform::isMultiTouch()
{
    false;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = __pitch;
    *roll = __roll;
}

bool Platform::hasMouse()
{
    return true;
}

void Platform::setMouseCaptured(bool captured)
{
    if (captured != __mouseCaptured)
    {
        if (captured)
        {
            // Hide the cursor and warp it to the center of the screen
            __mouseCapturePointX = getDisplayWidth() / 2;
            __mouseCapturePointY = getDisplayHeight() / 2;

            setCursorVisible(false);
            XWarpPointer(__display, None, __window, 0, 0, 0, 0, __mouseCapturePointX, __mouseCapturePointY);
        }
        else
        {
            // Restore cursor
            XWarpPointer(__display, None, __window, 0, 0, 0, 0, __mouseCapturePointX, __mouseCapturePointY);
            setCursorVisible(true);
        }

        __mouseCaptured = captured;
    }
}

bool Platform::isMouseCaptured()
{
    return __mouseCaptured;
}

void Platform::setCursorVisible(bool visible)
{
    if (visible != __cursorVisible)
    {
        if (visible)
        {
            Cursor invisibleCursor;
            Pixmap bitmapNoData;
            XColor black;
            static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
            black.red = black.green = black.blue = 0;
            bitmapNoData = XCreateBitmapFromData(__display, __window, noData, 8, 8);
            invisibleCursor = XCreatePixmapCursor(__display, bitmapNoData, bitmapNoData, &black, &black, 0, 0);

            XDefineCursor(__display, __window, invisibleCursor);
            XFreeCursor(__display, invisibleCursor);
            XFreePixmap(__display, bitmapNoData);
        }
        else
        {
            XUndefineCursor(__display, __window);
        }
        XFlush(__display);
        __cursorVisible = visible;
    }
}

bool Platform::isCursorVisible()
{
    return __cursorVisible;
}

void Platform::displayKeyboard(bool display)
{
    // not supported
}

void Platform::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (!Form::touchEventInternal(evt, x, y, contactIndex))
    {
        Game::getInstance()->touchEvent(evt, x, y, contactIndex);
        Game::getInstance()->getScriptController()->touchEvent(evt, x, y, contactIndex);
    }
}

void Platform::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (!Form::keyEventInternal(evt, key))
    {
        Game::getInstance()->keyEvent(evt, key);
        Game::getInstance()->getScriptController()->keyEvent(evt, key);
    }
}

bool Platform::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (Form::mouseEventInternal(evt, x, y, wheelDelta))
    {
        return true;
    }
    else if (Game::getInstance()->mouseEvent(evt, x, y, wheelDelta))
    {
        return true;
    }
    else
    {
        return Game::getInstance()->getScriptController()->mouseEvent(evt, x, y, wheelDelta);
    }
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
}
    
bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
}

bool Platform::launchURL(const char* url)
{
    if (url == NULL || *url == '\0')
        return false;

    int len = strlen(url);
    
    char* cmd = new char[11 + len];
    sprintf(cmd, "xdg-open %s", url);
    int r = system(cmd);
    SAFE_DELETE_ARRAY(cmd);
    
    return (r == 0);
}

}

#endif
