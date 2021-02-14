#pragma once

#include "App.h"
#include "Input.h"
#include "Signal.h"

namespace gameplay
{
struct WindowHandle;
struct Cursor;


/**
 * Defines a platform window.
 *
 * @see App::get_window() to access main application window.
 * @see App::create_window() to create additional windows.
 */
class GP_API Window
{
public:
    /**
     * Constructor.
     *
     * @see Windowing::create_window() instead.
     */
    Window();

    /**
     * Destructor.
     *
     * @see Windowing::destroy_window() instead.
     */
    ~Window();

    /**
     * Sets the title for a window.
     *
     * @param The title to be applied to the window.
     */
    void set_title(const char* title);

    /**
     * Sets the size of a window.
     *
     * @param size The new size to set for the window.
     */
    void set_size(const Int2& size);

    /**
     * Gets the size of the window.
     *
     * @return The size of the window.
     */
    Int2 get_size() const;

    /**
     * Signals that the window size has changed.
     *
     * emits:
     * <Window*> The window being resized
     * <Int2> The new size of the window
     */
    Signal<Window*, Int2> on_resize;

    /**
     * Sets the position of a window.
     *
     * @param pos The position to set the window to.
     */
    void set_pos(const Int2& pos);

    /**
     * Gets the position of a window.
     *
     * @returm The position of a window.
     */
    Int2 get_pos() const;

    /**
     * Signals that the window position has been moved.
     *
     * emits:
     * <Window*> The window being resized
     * <Int2> The new position of the window.
     */
    Signal<Window*, Int2> on_move;

    /**
     * Sets the window into fullscreen mode.
     *
     * @param fullscreen true to set to fullscreen, false for windowed.
     */
    void set_fullscreen(bool fullscreen);

    /**
     * Checks if the window is in fullscreen mode.
     *
     * @return true if the winow is in fullscreen, false if window is windowed.
     */
    bool is_fullscreen() const;

    /**
     * Gets the frame size for each edge of the frame of the window. 
     *
     * The frame size includes the title bar, if the window has one.
     * The size of the frame may vary depending on the window-related hints used to create it.
     *
     * @param left The size of the left edge of the frame.
     * @param top The size of the top of the frame including title bar.
     * @param right The size of the right edge of the frame.
     * @param bottom The size of the bottom of the frame.
     */
    void get_frame_size(int* left, int* top, int* right, int* bottom) const;

    /**
     * Sets the opacity of the window, including any decorations.
     *
     * The opacity (or alpha) value is a positive finite number
     * between zero and one, where zero is fully transparent and
     * one is fully opaque.
     *
     * @param opacity The opacity to set for the window between 0.0 - 1.0
     */
    void set_opacity(float opacity);

    /**
     * Gets the opacity of the window, including any decorations.
     *
     * The opacity (or alpha) value is a positive finite number
     * between zero and one, where zero is fully transparent and
     * one is fully opaque.
     *
     * @return The opacity to set for the window between 0.0 - 1.0
     */
    float get_opacity() const;

    /**
     * Sets the icon of the window.
     * 
     * If passed an array of candidate images, those of or closest
     * to the sizes desired by the system are selected.

     * If no images are specified, the window reverts to its default icon.
     *
     * @param images The images.
     * @param imageCount The number of images passed. 
     */
    void set_icon(const Pixmap* images, size_t imageCount);

    /**
     * Checks the value of the close flag of the window.
     *
     * @return The value of the close flag of the window.
     */
    bool should_close();

    /**
     * Closes the window.
     */
    void close();

    /**
     * Signals that the window has been flagged to close.
     *
     * emits:
     * <Window*> The window being closed
     */
    Signal<Window*> on_close;

    /**
     * Shows the window.
     */
    void show();

    /**
     * Hides the window.
     */
    void hide();

    /**
     * Checks if the window is visible (shown).
     *
     * @return true if the window is visible, false if the window is hidden.
     */
    bool is_visible() const;

    /**
     * Brings the window to front and sets input focus.
     *
     * The window should already be visible and not minimized.
     * 
     * Note: Do not use this function to steal focus from other applications
     * unless you are certain that is what the user wants.
     * Focus stealing can be extremely disruptive.
     */
    void focus();

    /**
     * Checks if the window is in focus.
     *
     * @return true if the window is in focus, false if not in focus.
     */
    bool is_focused() const;

    /**
     * Signals when the window gains or loses input focus.
     *
     * emits:
     * <Window*> The window losing or gaining focus.
     * <bool> true if gaining focused, false if loosing focus.
     */
    Signal<Window*, bool> on_focus;

    /**
     * Minimizes (iconifies) the window.
     *
     * If the window is already minimized, this function does nothing.
     * 
     * @see restore() to restore the window back.
     */
    void minimize();

    /**
     * Checks if the window is currently minimized(inonified).
     *
     * @returns true if the window is minimized(icoinified), false if not minimized.
     */
    bool is_minimized() const;

    /**
     * Signals when the window is minimized(iconified) or restored.
     *
     * emits:
     * <Window*> The window being minimized or restored.
     * <bool> true if the window is being minimized(iconified) or false if being restored.
     */
    Signal<Window*, bool> on_minimize;

    /**
     * Maximizes the window if it was previously not maximized.
     *
     * If the window is already maximized, this function does nothing.
     *
     * @see restore() to restore the window back.
     */
    void maximize();

    /**
     * Checks if the window is currently maximized.
     *
     * @returns true if the window is maximized, false if not maximized.
     */
    bool is_maximized() const;

    /**
     * Signals when the window is maximized or restored.
     *
     * emits:
     * <Window*> The window being maximized or restored.
     * <bool> true if the window is being maximized or false if being restored.
     */
    Signal<Window*, bool> on_maximize;

    /**
     * Restores the window if it was previously minimized(iconified) or maximized.
     *
     * If the window is already restored, this function does nothing.
     * If the  window is a full screen window, the resolution
     * chosen for the window is restored on the selected monitor.
     */
    void restore();

    /**
     * Gets the content scale for the window.
     *
     * The content scale is the ratio between the current DPI
     * and the platform's default DPI. This is especially important
     * for text and any UI elements. If the pixel dimensions of
     * your UI scaled by this look appropriate on your machine then
     * it should appear at a reasonable size on other machines regardless
     * of their DPI and scaling settings. This relies on the system DPI and
     * scaling settings being somewhat correct.
     *
     * On systems where each monitors can have its own content scale,
     * the window content scale will depend on which monitor the
     * system considers the window to be on.
     *
     * @return The content scale for the window.
     */
    Float2 get_content_scale() const;

    /**
     * Signals when the content scale of the specified window changes.
     *
     * emits:
     * <Window*> The window being content scaled.
     * <Float2> The update x and y content scale.
     */
    Signal<Window*, Float2> on_content_scale; 

    /**
     * Sets the window cursor to he 
     */
    void set_cursor(Cursor* cursor);

    /**
     * Sets the position of the coordinates of the cursor.
     *
     * The position is in screen coordinates, of the cursor relative
     * to the upper-left corner of the client area of the specified window.
     * The window must have input focus. If the window does not have input
     * focus when this function is called, it fails silently.
     *
     * Do not use this function to implement things like camera controls.
     * @see set_cursor_mode with `CursorMode::DISABLED`
     * to hide the cursor, transparently re-centers it and provides
     * unconstrained cursor motion.
     *
     * @param pos The coorinates which to set the cursor position to.
     */
    void set_cursor_pos(const Double2& pos);

    /**
     * Gets the position of the coordinates of the cursor.
     *
     * The position is in screen coordinates, of the cursor relative
     * to the upper-left corner of the client area of the specified window.
     * The window must have input focus. If the window does not have input
     * focus when this function is called, it fails silently.
     *
     * @return The cursor position coordinates
     */
    Double2 get_cursor_pos() const;

    /**
     * Sets the mode for the cursor.
     *
     * CursorMode::NORMAL makes the cursor visible and behaving normally.
     * CursorMode::HIDDEN makes the cursor invisible when it is over the content area
     *  of the window but does not restrict the cursor from leaving.
     * CursorMode::DISABLED makes the cursor invisible when it is over the
     * content area of the window but does not restrict the cursor from leaving.
     *
     * @param mode The cursor mode to set to.
     */
    void set_cursor_mode(CursorMode mode);

    /**
     * Gets the mode for the cursor.
     *
     * CursorMode::NORMAL makes the cursor visible and behaving normally.
     * CursorMode::HIDDEN makes the cursor invisible when it is over the content area
     *  of the window but does not restrict the cursor from leaving.
     * CursorMode::DISABLED makes the cursor invisible when it is over the
     * content area of the window but does not restrict the cursor from leaving.
     *
     * @return The current cursor mode.
     */
    CursorMode get_cursor_mode() const;

    /**
     * Signal when the cursor is moved.
     *
     * emits:
     * <Window*> The window being content scaled.
     * <Double2> The updated x and y screen coordinates.
     */
    Signal<Window*, Double2> on_cursor_move;

    /**
     * Signal when the cursor enters or leaves the client area of the window.
     *
     * emits:
     * <Window*> The window being content scaled.
     * <bool> true if the cursor enter the client area, false if it leaves it.
     */
    Signal<Window*, bool> on_cursor_enter;

    /**
     * Sets the input mode to be enabled.
     *
     * @param mode The input mode to be enabled.
     * @param enabled true if the input mode is enabled, false if disabled.
     */
    void set_input_mode_enabled(InputMode mode, bool enabled);

    /**
     * Checks if an input mode is enabled.
     *
     * @param mode The input mode to be enabled.
     * @return true if the input mode is enabled, false if disabled.
     */
    bool is_input_mode_enabled(InputMode mode) const;

    /**
     * Signaled when the user scrolls, whether with a mouse wheel or touchpad gesture.
     *
     * A normal mouse wheel, being vertical, provides offsets along the Y-axis.
     * 
     * emits:
     * <Window*> The window the mouse button is actioned in.
     * <Float2> The x,y scroll offset.
     */
    Signal<Window*, Float2> on_scroll;

    /**
     * Get the current mouse button action (state).
     *
     * @param button The button to get the action (state).
     * @return The current mouse button action (state)
     */
    ButtonAction get_mouse_button_action(MouseButton button);

    /**
     * Signaled when the mouse button action is fired (down) or released(up).
     *
     * emits:
     * <Window*> The window the mouse button is actioned in.
     * <int> The mouse button index. You can also use MouseButton::LEFT,RIGHT or MIDDLE.
     * <ButtonAction> The action of the mouse button (PRESS or RELEASE)
     * <Float2> The mouse window coordinates
     * <KeyModFlags> The flags that are active when the button is pressed/released.
     */
    Signal<Window*, int, ButtonAction, Float2, KeyModFlags> on_mouse_button;

    /**
     * Get the current key action (state) of a key.
     *
     * @param key The key to check.
     * @return The current key action (state) of a key.
     */
    KeyAction get_key_action(Key key);

    /**
     * Signaled when the key action is fired (PRESS, RELEASE or REPEAT).
     *
     * emits:
     * <Window*> The window the key is actioned from.
     * <Key> The keyboard key actioned
     * <KeyAction> The action of the key (PRESS, RELEASE or REPEAT)
     * <KeyModFlags> The flags that are active when the button is actioned.
     */
    Signal<Window*, Key, KeyAction, KeyModFlags> on_key;

    /**
     * Signaled when a unicode character is input.
     *
     * The character callback is intended for Unicode text input.
     * As it deals with characters, it is keyboard layout dependent,
     * whereas the key callback is not. Characters do not map 1:1 to physical keys,
     * as a key may produce zero, one or more characters. If you want to know whether
     * a specific physical key was pressed or released, see the key callback instead.
     *
     * The character callback behaves as system text input normally does and will not
     * be called if modifier keys are held down that would prevent normal text input
     * on that platform, for example a Super (Command) key on macOS or Alt key on Windows.
     * 
     * emits:
     * <Window*> The window the character is input into.
     * <uint32_t> The unicode point that is entered.
     */
    Signal<Window*, uint32_t> on_char;

    /**
     * Signaled when one or more dragged paths are dropped on the window.
     *
     * emits:
     * <Window*> The window the mouse button is actioned in.
     * <const char**> The paths dropped.
     * <size_t> The number of paths dropped.
     */
    Signal<Window*, const char**, size_t> on_drop;

    /**
     * Sets user pointer data associated with this window.
     *
     * @param userPtr The user pointer data to associate.
     */
    void set_user_ptr(void* userPtr);

    /**
     * Gets the user pointer data associate with this window.
     *
     * @return The user pointer data to associate.
     */
    void* get_user_ptr() const;

    std::unique_ptr<WindowHandle> handle;
};
}
