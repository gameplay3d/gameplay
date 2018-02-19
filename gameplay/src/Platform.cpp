#include "Base.h"
#include "Platform.h"
#include "Input.h"
#include "Command.h"
#include "Game.h"

#include <bx/bx.h>
#include <bx/os.h>
#include <bx/mutex.h>
#include <bx/thread.h>
#include <bx/handlealloc.h>
#include <bx/filepath.h>
#include <bx/readerwriter.h>
#include <bx/sort.h>

namespace gameplay
{

static bool __exit = false;
static uint32_t __debug = BGFX_DEBUG_TEXT;
static uint32_t __reset = BGFX_RESET_NONE;
static uint32_t __width = GP_GRAPHICS_WIDTH;
static uint32_t __height = GP_GRAPHICS_HEIGHT;
static std::string __currentDir = "";
static bx::FileReaderI* __fileReader = nullptr;
static bx::FileWriterI* __fileWriter = nullptr;
static Game* __currentGame = nullptr;
static Game*  __games = nullptr;
static uint32_t __gameCount = 0;
static char __restartArgs[1024] = { '\0' };
static WindowState __windowStates[GP_WINDOWS_MAX];


Platform::Platform()
{
}

Platform::~Platform()
{
}

bool Platform::isWindowValid(WindowHandle window)
{
	return (window.idx != UINT16_MAX);
}

Platform::Event::Event(Event::Type type)
	: type(type)
{
	window.idx = UINT16_MAX;
}

Platform::Event::Event(Event::Type type, WindowHandle window)
	: type(type), 
	  window(window)
{
}

struct AxisEvent : public Platform::Event
{
	AxisEvent(Platform::WindowHandle window) 
		: Event(TYPE_AXIS_EVENT, window) 
	{
	} 
	Input::GamepadHandle gamepad;
	Input::GamepadAxis axis;
	int32_t value;
};

struct CharEvent : public Platform::Event
{
	CharEvent(Platform::WindowHandle window) 
		: Event(TYPE_CHAR_EVENT, window) 
	{
	}
	uint8_t len;
	uint8_t chr[4];
};

struct GamepadEvent : public Platform::Event
{
	GamepadEvent(Platform::WindowHandle window) 
		: Event(TYPE_GAMEPAD_EVENT, window) 
	{
	}
	Input::GamepadHandle gamepad;
	bool connected;
};

struct KeyEvent : public Platform::Event
{
	KeyEvent(Platform::WindowHandle window) 
		: Event(TYPE_KEY_EVENT, window) 
	{
	}
	Input::Key key;
	uint8_t keyModifiers;
	bool down;
};

struct MouseEvent : public Platform::Event
{
	MouseEvent(Platform::WindowHandle window) 
		: Event(TYPE_MOUSE_EVENT, window) 
	{
	}
	int32_t mx;
	int32_t my;
	int32_t mz;
	Input::MouseButton button;
	bool down;
	bool move;
};

struct SizeEvent : public Platform::Event
{
	SizeEvent(Platform::WindowHandle window) 
		: Event(TYPE_SIZE_EVENT, window) 
	{
	}
	size_t width;
	size_t height;
};

struct WindowEvent : public Platform::Event
{
	WindowEvent(Platform::WindowHandle window) 
		: Event(TYPE_WINDOW_EVENT, window) 
	{
	}
	void* nwh;
};

struct SuspendEvent : public Platform::Event
{
	SuspendEvent(Platform::WindowHandle window) 
		: Event(TYPE_SUSPEND_EVENT, window) 
	{
	}
	Platform::SuspendMode mode;
};

struct DropFileEvent : public  Platform::Event
{
	DropFileEvent(Platform::WindowHandle window) 
		: Event(TYPE_DROP_FILE_EVENT, window) 
	{
	}
	bx::FilePath dropFile;
};


Platform::EventQueue::EventQueue()
	: _queue(getAllocator())
{
}

Platform::EventQueue::~EventQueue()
{
	for (const Event* evt = poll(); evt != nullptr; evt = poll())
	{
		release(evt);
	}
}

void Platform::EventQueue::postAxisEvent(WindowHandle window, Input::GamepadHandle gamepad, Input::GamepadAxis gamepadAxis, int32_t value)
{
	AxisEvent* evt = new AxisEvent(window);
	evt->gamepad = gamepad;
	evt->axis    = gamepadAxis;
	evt->value   = value;
	_queue.push(evt);
}

void Platform::EventQueue::postCharEvent(WindowHandle window, uint8_t len, const uint8_t chr[4])
{
	CharEvent* evt = new CharEvent(window);
	evt->len = len;
	bx::memCopy(evt->chr, chr, 4);
	_queue.push(evt);
}

void Platform::EventQueue::postExitEvent()
{
	Event* evt = new Event(Event::TYPE_EXIT_EVENT);
	_queue.push(evt);
}

void Platform::EventQueue::postGamepadEvent(WindowHandle window, Input::GamepadHandle gamepad, bool connected)
{
	GamepadEvent* evt = new GamepadEvent(window);
	evt->gamepad   = gamepad;
	evt->connected = connected;
	_queue.push(evt);
}

void Platform::EventQueue::postKeyEvent(WindowHandle window, Input::Key key, uint8_t keyModifiers, bool down)
{
	KeyEvent* evt = new KeyEvent(window);
	evt->key = key;
	evt->keyModifiers = keyModifiers;
	evt->down = down;
	_queue.push(evt);
}

void Platform::EventQueue::postMouseEvent(WindowHandle window, int32_t mx, int32_t my, int32_t mz)
{
	MouseEvent* evt = new MouseEvent(window);
	evt->mx = mx;
	evt->my = my;
	evt->mz = mz;
	evt->button = Input::MouseButton::MOUSE_BUTTON_NONE;
	evt->down = false;
	evt->move = true;
	_queue.push(evt);
}

void Platform::EventQueue::postMouseEvent(WindowHandle window, int32_t mx, int32_t my, int32_t mz, Input::MouseButton button, bool down)
{
	MouseEvent* evt = new MouseEvent(window);
	evt->mx = mx;
	evt->my = my;
	evt->mz = mz;
	evt->button = button;
	evt->down = down;
	evt->move = false;
	_queue.push(evt);
}

void Platform::EventQueue::postSizeEvent(WindowHandle window, uint32_t width, uint32_t height)
{
	SizeEvent* evt = new SizeEvent(window);
	evt->width  = width;
	evt->height = height;
	_queue.push(evt);
}

void Platform::EventQueue::postWindowEvent(WindowHandle window, void* nativeWindowHandle)
{
	WindowEvent* evt = new WindowEvent(window);
	evt->nwh = nativeWindowHandle;
	_queue.push(evt);
}

void Platform::EventQueue::postSuspendEvent(WindowHandle window, SuspendMode mode)
{
	SuspendEvent* evt = new SuspendEvent(window);
	evt->mode = mode;
	_queue.push(evt);
}

void Platform::EventQueue::postDropFileEvent(WindowHandle window, const bx::FilePath& dropFile)
{
	DropFileEvent* evt = new DropFileEvent(window);
	evt->dropFile = dropFile;
	_queue.push(evt);
}

void setCurrentDir(const char* _dir)
{
	__currentDir = std::string(_dir);
}

bx::FileReaderI* getFileReader()
{
	return __fileReader;
}

bx::FileWriterI* getFileWriter()
{
	return __fileWriter;
}

bx::AllocatorI* getAllocator()
{
	BX_PRAGMA_DIAGNOSTIC_PUSH();
	BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459);
	BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
	static bx::DefaultAllocator allocator;
	return &allocator;
}

const Platform::Event* Platform::EventQueue::poll()
{
	return _queue.pop();
}

const Platform::Event* Platform::EventQueue::poll(Platform::WindowHandle window)
{
	if (isWindowValid(window) )
	{
		Platform::Event* evt = _queue.peek();
		if (evt != nullptr ||  evt->window.idx != window.idx)
		{
			return nullptr;
		}
	}
	return poll();
}

void Platform::EventQueue::release(const Event* evt) const
{
	delete const_cast<Event*>(evt);
}

bool FileReader::open(const bx::FilePath& _filePath, bx::Error* _err)
{
	std::string filePath(__currentDir);
	filePath.append(_filePath.get());
	return super::open(filePath.c_str(), _err);
}

bool FileWriter::open(const bx::FilePath& _filePath, bool _append, bx::Error* _err)
{
	std::string filePath(__currentDir);
	filePath.append(_filePath.get() );
	return super::open(filePath.c_str(), _append, _err);
}

bool processEvents(size_t& width, size_t& height, uint32_t& debug, uint32_t& reset, MouseState* mouseState)
{
	__debug = debug;
	__reset = reset;

	gameplay::Platform::WindowHandle window = { UINT16_MAX };

	bool mouseLock = Input::isMouseLocked();

	const Platform::Event* evt;
	do
	{
		struct SE { const Platform::Event* _evt; SE() : _evt(poll() ) {} ~SE() { if (_evt != nullptr) { release(_evt); } } } scopeEvent;
		evt = scopeEvent._evt;

		if (evt != nullptr)
		{
			switch (evt->type)
			{
			case  Platform::Event::TYPE_AXIS_EVENT:
				{	
					const AxisEvent* axisEvt = static_cast<const AxisEvent*>(evt);
					Input::gamepads[window.idx].setAxis(axisEvt->axis, axisEvt->value);
				}
				break;

			case Platform::Event::TYPE_CHAR_EVENT:
				{
					const CharEvent* charEvt = static_cast<const CharEvent*>(evt);
					Input::keyboard.pushChar(charEvt->len, charEvt->chr);
				}
				break;

			case Platform::Event::TYPE_EXIT_EVENT:
				return true;

			case Platform::Event::TYPE_GAMEPAD_EVENT:
				{
					const GamepadEvent* gamepadEvt = static_cast<const GamepadEvent*>(evt);
					GP_INFO("gamepad %d, %d", gamepadEvt->gamepad.idx, gamepadEvt->connected);
				}
				break;

			case Platform::Event::TYPE_MOUSE_EVENT:
				{
					const MouseEvent* mouseEvt = static_cast<const MouseEvent*>(evt);
					window = mouseEvt->window;

					Input::mouse.setPosition(mouseEvt->mx, mouseEvt->my, mouseEvt->mz);
					
					if (!mouseEvt->move)
					{
						Input::mouse.setButtonState(mouseEvt->button, mouseEvt->down);
					}

					if (mouseState != nullptr &&  !mouseLock)
					{
						mouseState->mx = mouseEvt->mx;
						mouseState->my = mouseEvt->my;
						mouseState->mz = mouseEvt->mz;
						if (!mouseEvt->move)
						{
							mouseState->buttons[mouseEvt->button] = mouseEvt->down;
						}
					}
				}
				break;

			case Platform::Event::TYPE_KEY_EVENT:
				{
					const KeyEvent* keyEvt = static_cast<const KeyEvent*>(evt);
					window = keyEvt->window;
					Input::keyboard.setKeyState(keyEvt->key, keyEvt->keyModifiers, keyEvt->down);
				}
				break;

			case Platform::Event::TYPE_SIZE_EVENT:
				{
					const SizeEvent* sizeEvt = static_cast<const SizeEvent*>(evt);
					WindowState& windowState = __windowStates[0];
					windowState.window = sizeEvt->window;
					windowState.width  = sizeEvt->width;
					windowState.height = sizeEvt->height;

					window  = sizeEvt->window;
					width  = sizeEvt->width;
					height = sizeEvt->height;
					reset  = !__reset;
				}
				break;

			case Platform::Event::TYPE_WINDOW_EVENT:
				break;

			case Platform::Event::TYPE_SUSPEND_EVENT:
				break;

			case Platform::Event::TYPE_DROP_FILE_EVENT:
				{
					const DropFileEvent* dropEvt = static_cast<const DropFileEvent*>(evt);
					GP_INFO("Drag file:%s", dropEvt->dropFile.get() );
				}
				break;

			default:
				break;
			}
		}

		Input::process();

	} while (evt != nullptr);

	if (window.idx == 0 &&  reset != __reset)
	{
		reset = __reset;
		bgfx::reset(width, height, reset);
		Input::mouse.setResolution(width, height);
	}

	debug = __debug;
	__width = width;
	__height = height;

	return __exit;
}

bool processWindowEvents(WindowState& state, uint32_t& debug, uint32_t& reset)
{
	__debug = debug;
	__reset = reset;

	Platform::WindowHandle window = { UINT16_MAX };

	bool mouseLock = Input::isMouseLocked();
	bool clearDropFile = true;

	const Platform::Event* evt;
	do
	{
		struct SE
		{
			SE(Platform::WindowHandle handle)
				: _evt(poll(handle) )
			{
			}

			~SE()
			{
				if (_evt != nullptr)
				{
					release(_evt);
				}
			}

			const Platform::Event* _evt;

		} scopeEvent(window);
		evt = scopeEvent._evt;

		if (evt != nullptr)
		{
			window = evt->window;
			WindowState& windowState = __windowStates[window.idx];

			switch (evt->type)
			{
			case Platform::Event::TYPE_AXIS_EVENT:
				{	
					const AxisEvent* axisEvt = static_cast<const AxisEvent*>(evt);
					Input::gamepads[window.idx].setAxis(axisEvt->axis, axisEvt->value);
				}
				break;

			case Platform::Event::TYPE_CHAR_EVENT:
				{
					const CharEvent* charEvt = static_cast<const CharEvent*>(evt);
					windowState.window = charEvt->window;
					Input::keyboard.pushChar(charEvt->len, charEvt->chr);
				}
				break;

			case Platform::Event::TYPE_EXIT_EVENT:
				return true;

			case Platform::Event::TYPE_GAMEPAD_EVENT:
				{
					const GamepadEvent* gamepadEvt = static_cast<const GamepadEvent*>(evt);
					GP_INFO("gamepad %d, %d", gamepadEvt->gamepad.idx, gamepadEvt->connected);
				}
				break;

			case Platform::Event::TYPE_MOUSE_EVENT:
				{
					const MouseEvent* mouseEvt = static_cast<const MouseEvent*>(evt);
					windowState.window = mouseEvt->window;

					if (mouseEvt->move)
					{
						Input::mouse.setPosition(mouseEvt->mx, mouseEvt->my, mouseEvt->mz);
					}
					else
					{
						Input::mouse.setButtonState(mouseEvt->button, mouseEvt->down);
					}

					if (!mouseLock)
					{
						if (mouseEvt->move)
						{
							windowState.mouse.mx = mouseEvt->mx;
							windowState.mouse.my = mouseEvt->my;
							windowState.mouse.mz = mouseEvt->mz;
						}
						else
						{
							windowState.mouse.buttons[mouseEvt->button] = mouseEvt->down;
						}
					}
				}
				break;

			case Platform::Event::TYPE_KEY_EVENT:
				{
					const KeyEvent* keyEvt = static_cast<const KeyEvent*>(evt);
					windowState.window = keyEvt->window;
					Input::keyboard.setKeyState(keyEvt->key, keyEvt->keyModifiers, keyEvt->down);
				}
				break;

			case Platform::Event::TYPE_SIZE_EVENT:
				{
					const SizeEvent* sizeEvt = static_cast<const SizeEvent*>(evt);
					windowState.window = sizeEvt->window;
					windowState.width  = sizeEvt->width;
					windowState.height = sizeEvt->height;
					__reset  = windowState.window.idx == 0 ? !__reset : reset;
				}
				break;

			case Platform::Event::TYPE_WINDOW_EVENT:
				{
					const WindowEvent* windowEvt = static_cast<const WindowEvent*>(evt);
					windowState.window = windowEvt->window;
					windowState.nwh    = windowEvt->nwh;
					evt = nullptr;
				}
				break;

			case Platform::Event::TYPE_SUSPEND_EVENT:
				break;

			case Platform::Event::TYPE_DROP_FILE_EVENT:
				{
					const DropFileEvent* dropEvt = static_cast<const DropFileEvent*>(evt);
					windowState.dropFile = dropEvt->dropFile;
					clearDropFile = false;
				}
				break;

			default:
				break;
			}
		}
		Input::process();

	} while (evt != nullptr);

	if (Platform::isWindowValid(window))
	{
		WindowState& windowState = __windowStates[window.idx];
		if (clearDropFile)
		{
			windowState.dropFile.clear();
		}
		state = windowState;

		if (window.idx == 0)
		{
			Input::mouse.setResolution(windowState.width, windowState.height);
		}
	}

	if (reset != __reset)
	{
		reset = __reset;
		bgfx::reset(__windowStates[0].width, __windowStates[0].height, reset);
		Input::mouse.setResolution(__windowStates[0].width, __windowStates[0].height);
	}
	debug = __debug;

	return __exit;
}

Game* getFirstGame()
{
	return __games;
}

uint32_t getGameCount()
{
	return __gameCount;
}

static Game* getCurrentGame(Game* set = nullptr)
{
	if (set != nullptr )
	{
		__currentGame = set;
	}
	else if (__currentGame == nullptr)
	{
		__currentGame = getFirstGame();
	}
	return __currentGame;
}

static Game* getNextGame(Game* game)
{
	Game* next = game->next;
	if (next != nullptr)
	{
		return next;
	}
	return getFirstGame();
}

int runGame(Game* game, int argc, const char* const* argv)
{
	game->onInitialize(argc, argv);
	bgfx::frame();

	Platform::WindowHandle defaultWindow = { 0 };
	setWindowSize(defaultWindow, game->getWidth(), game->getHeight());

	while (game->frame())
	{
		if (bx::strLen(__restartArgs) != 0)
		{
			break;
		}
	}
	return game->exit();
}

static int32_t sortGame(const void* a, const void* b)
{
	Game* lhs = *(Game**)a;
	Game* rhs = *(Game**)b;
	return bx::strCmpI(lhs->getConfig()->title.c_str(), rhs->getConfig()->title.c_str() );
}

static void sortGames()
{
	if (__gameCount < 2)
		return;
	
	Game** games = (Game**)::malloc(__gameCount * sizeof(Game*) );

	uint32_t i = 0;
	for (Game* game = getFirstGame(); game != nullptr; game = game->next )
	{
		games[i++] = game;
	}
	bx::quickSort(games, __gameCount, sizeof(Game*), sortGame);

	__games = games[0];
	for (i = 1; i < __gameCount; ++i)
	{
		Game* game = games[i - 1];
		game->next = games[i];
	}
	games[__gameCount - 1]->next = nullptr;

	::free(games);
}

bool setOrToggle(uint32_t& flags, const char* name, uint32_t bit, int first, int argc, char const* const* argv)
{
	if (bx::strCmp(argv[first], name) == 0)
	{
		int arg = first+1;
		if (argc > arg)
		{
			flags &= ~bit;
			bool set = false;
			bx::fromString(&set, argv[arg]);
			flags |= set ? bit : 0;
		}
		else
		{
			flags ^= bit;
		}
		return true;
	}
	return false;
}

int cmdGame(CommandContext* context, void* userData, int argc, char const* const* argv)
{
	if (bx::strCmp(argv[1], "restart") == 0)
	{
		if (argc == 2)
		{
			bx::strCopy(__restartArgs, BX_COUNTOF(__restartArgs), getCurrentGame()->getConfig()->title.c_str() );
			return bx::kExitSuccess;
		}

		if (bx::strCmp(argv[2], "next") == 0)
		{
			Game* next = getNextGame(getCurrentGame() );
			bx::strCopy(__restartArgs, BX_COUNTOF(__restartArgs), next->getConfig()->title.c_str() );
			return bx::kExitSuccess;
		}
		else if (bx::strCmp(argv[2], "prev") == 0)
		{
			Game* prev = getCurrentGame();
			for (Game* game = getNextGame(prev); game != getCurrentGame(); game = getNextGame(game) )
			{
				prev = game;
			}

			bx::strCopy(__restartArgs, BX_COUNTOF(__restartArgs), prev->getConfig()->title.c_str() );
			return bx::kExitSuccess;
		}

		for (Game* game = getFirstGame(); game != nullptr; game = game->next )
		{
			if (bx::strCmp(argv[2], game->getConfig()->title.c_str()) == 0 )
			{
				bx::strCopy(__restartArgs, BX_COUNTOF(__restartArgs), game->getConfig()->title.c_str() );
				return bx::kExitSuccess;
			}
		}
	}
	return bx::kExitFailure;
}


int cmdExit(CommandContext* context, void* userData, int argc, char const* const* argv)
{
	__exit = true;
	return bx::kExitSuccess;
}

int cmdGraphics(CommandContext* context, void* userData, int argc, char const* const* argv)
{
	if (argc > 1)
	{
		if (setOrToggle(__reset, "vsync",       BGFX_RESET_VSYNC,              1, argc, argv) ||  
			setOrToggle(__reset, "maxaniso",    BGFX_RESET_MAXANISOTROPY,      1, argc, argv) ||  
			setOrToggle(__reset, "hmd",         BGFX_RESET_HMD,                1, argc, argv) || 
			setOrToggle(__reset, "hmddbg",      BGFX_RESET_HMD_DEBUG,          1, argc, argv) ||  
			setOrToggle(__reset, "hmdrecenter", BGFX_RESET_HMD_RECENTER,       1, argc, argv) || 
			setOrToggle(__reset, "msaa",        BGFX_RESET_MSAA_X16,           1, argc, argv) ||
			setOrToggle(__reset, "flush",       BGFX_RESET_FLUSH_AFTER_RENDER, 1, argc, argv) || 
			setOrToggle(__reset, "flip",        BGFX_RESET_FLIP_AFTER_RENDER,  1, argc, argv) ||  
			setOrToggle(__reset, "hidpi",       BGFX_RESET_HIDPI,              1, argc, argv) || 
			setOrToggle(__reset, "depthclamp",  BGFX_RESET_DEPTH_CLAMP,        1, argc, argv))
		{
			return bx::kExitSuccess;
		}
		else if (setOrToggle(__debug, "stats",     BGFX_DEBUG_STATS,     1, argc, argv) ||  
				 setOrToggle(__debug, "ifh",       BGFX_DEBUG_IFH,       1, argc, argv) ||  
				 setOrToggle(__debug, "text",      BGFX_DEBUG_TEXT,      1, argc, argv) ||  
				 setOrToggle(__debug, "wireframe", BGFX_DEBUG_WIREFRAME, 1, argc, argv) ||  
				 setOrToggle(__debug, "profiler",  BGFX_DEBUG_PROFILER,  1, argc, argv))
		{
			bgfx::setDebug(__debug);
			return bx::kExitSuccess;
		}
		else if (bx::strCmp(argv[1], "screenshot") == 0)
		{
			bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;

			if (argc > 2)
			{
				bgfx::requestScreenShot(fbh, argv[2]);
			}
			else
			{
				time_t tt;
				time(&tt);

				char filePath[256];
				bx::snprintf(filePath, sizeof(filePath), "temp/screenshot-%d", tt);
				bgfx::requestScreenShot(fbh, filePath);
			}

			return bx::kExitSuccess;
		}
		else if (bx::strCmp(argv[1], "fullscreen") == 0 )
		{
			Platform::WindowHandle window = { 0 };
			toggleFullscreen(window);
			return bx::kExitSuccess;
		}
	}
	return bx::kExitFailure;
}

int cmdMouseLock(CommandContext* context, void* userData, int argc, char const* const* argv)
{
	if (argc > 1)
	{
		bool lock = false;
		if (argc > 2)
		{
			bx::fromString(&lock, argv[1]);
			Input::setMouseLocked(lock);
		}
		else
		{
			Input::setMouseLocked(!Input::isMouseLocked());
		}
		return bx::kExitSuccess;
	}
	return bx::kExitFailure;
}


int main(int argc, const char* const* argv)
{
	__fileReader = new FileReader();
	__fileWriter = new FileWriter();

	cmdInit();
	cmdAdd("game",      cmdGame     );
	cmdAdd("exit",      cmdExit     );
	cmdAdd("graphics",  cmdGraphics );
	cmdAdd("mouselock", cmdMouseLock);
	
	Platform::WindowHandle defaultWindow = { 0 };

	bx::FilePath fp(argv[0]);
	char title[bx::kMaxFilePath];
	bx::strCopy(title, BX_COUNTOF(title), fp.getBaseName() );
	setWindowTitle(defaultWindow, title);
	setWindowSize(defaultWindow, GP_GRAPHICS_WIDTH, GP_GRAPHICS_HEIGHT);

	sortGames();

	const char* find = "";
	if (argc > 1)
	{
		find = argv[argc - 1];
	}

restart:
	Game* selected = nullptr;
	for (Game* game = getFirstGame(); game != nullptr; game = game->next )
	{
		if (selected == nullptr && bx::strFindI(game->getConfig()->title.c_str(), find) )
		{
			selected = game;
		}
	}

	int32_t result = bx::kExitSuccess;
	__restartArgs[0] = '\0';
	if (__gameCount == 0)
	{
		result = ::_main_(argc, (char**)argv);
	}
	else
	{
		result = runGame(getCurrentGame(selected), argc, argv);
	}

	if (bx::strLen(__restartArgs) != 0)
	{
		find = __restartArgs;
		goto restart;
	}

	setCurrentDir("");

	cmdShutdown();

	GP_SAFE_DELETE(__fileReader);
	GP_SAFE_DELETE(__fileWriter);

	return result;
}

}
