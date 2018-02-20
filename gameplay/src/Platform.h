#pragma once

#include "Input.h"

#include <bx/bx.h>
#include <bx/os.h>
#include <bx/file.h>
#include <bx/spscqueue.h>
#include <bgfx/platform.h>

namespace gameplay
{

/**
 * Defines a platform abstraction layer.
 */
class Platform
{
public:

	/**
	 * Constructor.
	 */
	Platform();

	/**
	 * Destructor.
	 */
	~Platform();

	/**
	 * Defines an opaque window  handle.
	 */
	struct WindowHandle  
	{ 
		unsigned short idx; 
	};
	
	/**
	 * Checks if a window is valid.
	 *
	 * @return true if the window is valid, false if invalid.
	 */
	static bool isWindowValid(WindowHandle handle);


	/**
	 * Defines the suspend state of suspend events.
	 */
	enum SuspendMode
	{
		SUSPEND_MODE_WILL_SUSPEND,
		SUSPEND_MODE_DID_SUSPEND,
		SUSPEND_MODE_WILL_RESUME,
		SUSPEND_MODE_DID_RESUME,
		SUSPEND_MODE_COUNt
	};

	/**
	 * Defines a platform event.
	 */
	struct Event
	{
		/**
		 * Type of platform event.
		 */
		enum Type
		{
			TYPE_AXIS_EVENT,
			TYPE_CHAR_EVENT,
			TYPE_EXIT_EVENT,
			TYPE_GAMEPAD_EVENT,
			TYPE_KEY_EVENT,
			TYPE_MOUSE_EVENT,
			TYPE_SIZE_EVENT,
			TYPE_WINDOW_EVENT,
			TYPE_SUSPEND_EVENT,
			TYPE_DROP_FILE_EVENT
		};

		Event(Type type);

		Event(Type type, WindowHandle window);

		Event::Type type;
		WindowHandle window;
	};

	/**
	 * Defines an event queue for processing platform events.
	 */
	class EventQueue
	{
	public:

		EventQueue();

		~EventQueue();

		void postAxisEvent(WindowHandle window, Input::GamepadHandle gamepad, Input::GamepadAxis gamepadAxis, int32_t value);

		void postCharEvent(WindowHandle window, uint8_t len, const uint8_t chr[4]);
		
		void postExitEvent();
		
		void postGamepadEvent(WindowHandle window, Input::GamepadHandle gamepad, bool connected);
		
		void postKeyEvent(WindowHandle window, Input::Key key, uint8_t keyModifiers, bool down);
		
		void postMouseEvent(WindowHandle window, int32_t x, int32_t y, int32_t z);
		
		void postMouseEvent(WindowHandle window, int32_t x, int32_t y, int32_t z, Input::MouseButton button, bool down);
		
		void postSizeEvent(WindowHandle window, uint32_t width, uint32_t height);
		
		void postWindowEvent(WindowHandle window, void* nwh = nullptr);
		
		void postSuspendEvent(WindowHandle window, SuspendMode mode);

		void postDropFileEvent(WindowHandle window, const bx::FilePath& dropFile);

		const Platform::Event* poll();

		const Platform::Event* poll(Platform::WindowHandle window);

		void release(const Platform::Event* evt) const;

	private:
		bx::SpScUnboundedQueueT<Event> _queue;
	};
};

void setCurrentDir(const char* dir);
bx::FileReaderI* getFileReader();
bx::FileWriterI* getFileWriter();
bx::AllocatorI* getAllocator();
const Platform::Event* poll();
const Platform::Event* poll(Platform::WindowHandle window);
void release(const Platform::Event* evt);

/**
 * Defines the platform mouse state
 */
struct MouseState
{
	MouseState() 
		: mx(0), 
		  my(0), 
		  mz(0)
	{
		for (uint32_t i = 0; i < Input::MouseButton::MOUSE_BUTTON_COUNT; ++i)
		{
			buttons[i] = Input::MouseButton::MOUSE_BUTTON_NONE;
		}
	}

	int32_t mx;
	int32_t my;
	int32_t mz;
	uint8_t buttons[Input::MouseButton::MOUSE_BUTTON_COUNT];
};

/**
 * Defines the platform gamepad state
 */
struct GamepadState
{
	GamepadState()
	{
		bx::memSet(axis, 0, sizeof(axis) );
	}

	int32_t axis[Input::GamepadAxis::GAMEPAD_AXIS_COUNT];
};

/**
 * Defines the platform window state.
 */
struct WindowState
{
	WindowState()
		: width(0), 
		  height(0), 
		  nwh(nullptr)
	{
		window.idx = UINT16_MAX;
	}

	Platform::WindowHandle window;
	size_t width;
	size_t height;
	MouseState mouse;
	void* nwh;
	bx::FilePath dropFile;
};

/**
 * Defines a platform file reader.
 */
class FileReader : public bx::FileReader
{
	typedef bx::FileReader super;
public:
	virtual bool open(const bx::FilePath& _filePath, bx::Error* _err);
};

/**
 * Defines a platform file writer.
 */
class FileWriter : public bx::FileWriter
{
	typedef bx::FileWriter super;
public:
	virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err);
};

bool processEvents(size_t& width, size_t& height, uint32_t& debug, uint32_t& reset, MouseState* mouse = nullptr);

bool processWindowEvents(WindowState& state, uint32_t& debug, uint32_t& reset);

int main(int argc, const char* const* argv);


// Platform specific
gameplay::Platform::WindowHandle createWindow(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t flags = GP_WINDOW_FLAG_NONE, const char* title = "");
void destroyWindow(gameplay::Platform::WindowHandle handle);
void setWindowPos(gameplay::Platform::WindowHandle handle, int32_t x, int32_t y);
void setWindowSize(gameplay::Platform::WindowHandle handle, uint32_t width, uint32_t height);
void setWindowTitle(gameplay::Platform::WindowHandle handle, const char* title);
void setWindowFlags(gameplay::Platform::WindowHandle handle, uint32_t flags, bool enabled);
void toggleFullscreen(gameplay::Platform::WindowHandle handle);
void setMouseLock(gameplay::Platform::WindowHandle handle, bool lock);

}


// Main entry point
extern "C" int _main_(int _argc, char** _argv);

#ifndef GP_CONFIG_IMPLEMENT_MAIN
#	define GP_CONFIG_IMPLEMENT_MAIN 1
#endif
#if GP_CONFIG_IMPLEMENT_MAIN
#define GP_IMPLEMENT_MAIN(_app) \
	int _main_(int _argc, char** _argv)             \
	{                                               \
		_app app;									\
		return gameplay::runGame(&app, _argc, _argv);   \
	}
#else
#define GP_IMPLEMENT_MAIN(_app) \
	_app app
#endif 





