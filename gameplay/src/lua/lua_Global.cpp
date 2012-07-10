#include "ScriptController.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_lua_Global()
{
    ScriptController* sc = ScriptController::getInstance();

    sc->registerFunction("__al_error_code", lua____al_error_code);
    sc->registerFunction("__gl_error_code", lua____gl_error_code);
    sc->registerFunction("printError", lua__printError);

    std::map<std::string, std::vector<std::string> > hierarchy;
    hierarchy["AnimationClip::Listener"].push_back("classgameplay_1_1_script_listener");
    hierarchy["AnimationTarget"].push_back("Button");
    hierarchy["AnimationTarget"].push_back("CheckBox");
    hierarchy["AnimationTarget"].push_back("Container");
    hierarchy["AnimationTarget"].push_back("Control");
    hierarchy["AnimationTarget"].push_back("Form");
    hierarchy["AnimationTarget"].push_back("Joint");
    hierarchy["AnimationTarget"].push_back("Joystick");
    hierarchy["AnimationTarget"].push_back("Label");
    hierarchy["AnimationTarget"].push_back("MaterialParameter");
    hierarchy["AnimationTarget"].push_back("Node");
    hierarchy["AnimationTarget"].push_back("RadioButton");
    hierarchy["AnimationTarget"].push_back("Slider");
    hierarchy["AnimationTarget"].push_back("TextBox");
    hierarchy["AnimationTarget"].push_back("Transform");
    hierarchy["AnimationTarget"].push_back("classgameplay_1_1_theme_1_1_style_1_1_overlay");
    hierarchy["Button"].push_back("CheckBox");
    hierarchy["Button"].push_back("RadioButton");
    hierarchy["Container"].push_back("Form");
    hierarchy["Control"].push_back("Button");
    hierarchy["Control"].push_back("CheckBox");
    hierarchy["Control"].push_back("Container");
    hierarchy["Control"].push_back("Form");
    hierarchy["Control"].push_back("Joystick");
    hierarchy["Control"].push_back("Label");
    hierarchy["Control"].push_back("RadioButton");
    hierarchy["Control"].push_back("Slider");
    hierarchy["Control"].push_back("TextBox");
    hierarchy["Control::Listener"].push_back("classgameplay_1_1_script_listener");
    hierarchy["Label"].push_back("Button");
    hierarchy["Label"].push_back("CheckBox");
    hierarchy["Label"].push_back("RadioButton");
    hierarchy["Label"].push_back("Slider");
    hierarchy["Label"].push_back("TextBox");
    hierarchy["Layout"].push_back("AbsoluteLayout");
    hierarchy["Layout"].push_back("FlowLayout");
    hierarchy["Layout"].push_back("VerticalLayout");
    hierarchy["Node"].push_back("Joint");
    hierarchy["PhysicsCollisionObject"].push_back("PhysicsCharacter");
    hierarchy["PhysicsCollisionObject"].push_back("PhysicsGhostObject");
    hierarchy["PhysicsCollisionObject"].push_back("PhysicsRigidBody");
    hierarchy["PhysicsCollisionObject::CollisionListener"].push_back("classgameplay_1_1_script_listener");
    hierarchy["PhysicsConstraint"].push_back("PhysicsFixedConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsGenericConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsHingeConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsSocketConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsSpringConstraint");
    hierarchy["PhysicsController::Listener"].push_back("classgameplay_1_1_script_listener");
    hierarchy["PhysicsGenericConstraint"].push_back("PhysicsFixedConstraint");
    hierarchy["PhysicsGenericConstraint"].push_back("PhysicsSpringConstraint");
    hierarchy["PhysicsGhostObject"].push_back("PhysicsCharacter");
    hierarchy["Ref"].push_back("AbsoluteLayout");
    hierarchy["Ref"].push_back("Animation");
    hierarchy["Ref"].push_back("AnimationClip");
    hierarchy["Ref"].push_back("AudioBuffer");
    hierarchy["Ref"].push_back("AudioSource");
    hierarchy["Ref"].push_back("Bundle");
    hierarchy["Ref"].push_back("Button");
    hierarchy["Ref"].push_back("Camera");
    hierarchy["Ref"].push_back("CheckBox");
    hierarchy["Ref"].push_back("Container");
    hierarchy["Ref"].push_back("Control");
    hierarchy["Ref"].push_back("Curve");
    hierarchy["Ref"].push_back("DepthStencilTarget");
    hierarchy["Ref"].push_back("Effect");
    hierarchy["Ref"].push_back("FlowLayout");
    hierarchy["Ref"].push_back("Font");
    hierarchy["Ref"].push_back("Form");
    hierarchy["Ref"].push_back("FrameBuffer");
    hierarchy["Ref"].push_back("Image");
    hierarchy["Ref"].push_back("Joint");
    hierarchy["Ref"].push_back("Joystick");
    hierarchy["Ref"].push_back("Label");
    hierarchy["Ref"].push_back("Layout");
    hierarchy["Ref"].push_back("Light");
    hierarchy["Ref"].push_back("Material");
    hierarchy["Ref"].push_back("MaterialParameter");
    hierarchy["Ref"].push_back("Mesh");
    hierarchy["Ref"].push_back("Model");
    hierarchy["Ref"].push_back("Node");
    hierarchy["Ref"].push_back("ParticleEmitter");
    hierarchy["Ref"].push_back("Pass");
    hierarchy["Ref"].push_back("PhysicsCollisionShape");
    hierarchy["Ref"].push_back("RadioButton");
    hierarchy["Ref"].push_back("RenderState");
    hierarchy["Ref"].push_back("RenderState::StateBlock");
    hierarchy["Ref"].push_back("RenderTarget");
    hierarchy["Ref"].push_back("Scene");
    hierarchy["Ref"].push_back("Slider");
    hierarchy["Ref"].push_back("Technique");
    hierarchy["Ref"].push_back("TextBox");
    hierarchy["Ref"].push_back("Texture");
    hierarchy["Ref"].push_back("Texture::Sampler");
    hierarchy["Ref"].push_back("Theme");
    hierarchy["Ref"].push_back("Theme::ThemeImage");
    hierarchy["Ref"].push_back("VertexAttributeBinding");
    hierarchy["Ref"].push_back("VerticalLayout");
    hierarchy["Ref"].push_back("classgameplay_1_1_material_parameter_1_1_method_binding");
    hierarchy["Ref"].push_back("classgameplay_1_1_theme_1_1_image_list");
    hierarchy["Ref"].push_back("classgameplay_1_1_theme_1_1_skin");
    hierarchy["Ref"].push_back("classgameplay_1_1_theme_1_1_style_1_1_overlay");
    hierarchy["RenderState"].push_back("Material");
    hierarchy["RenderState"].push_back("Pass");
    hierarchy["RenderState"].push_back("Technique");
    hierarchy["Transform"].push_back("Joint");
    hierarchy["Transform"].push_back("Node");
    hierarchy["Transform::Listener"].push_back("AudioListener");
    hierarchy["Transform::Listener"].push_back("AudioSource");
    hierarchy["Transform::Listener"].push_back("Camera");
    hierarchy["Transform::Listener"].push_back("MeshSkin");
    hierarchy["Transform::Listener"].push_back("PhysicsCharacter");
    hierarchy["Transform::Listener"].push_back("PhysicsGhostObject");
    hierarchy["Transform::Listener"].push_back("PhysicsRigidBody");
    hierarchy["Transform::Listener"].push_back("classgameplay_1_1_script_listener");
    sc->setGlobalHierarchy(hierarchy);

    // Register enumeration AnimationClip::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AnimationClip");
        scopePath.push_back("Listener");
        sc->registerConstantString("DEFAULT", "DEFAULT", scopePath);
        sc->registerConstantString("BEGIN", "BEGIN", scopePath);
        sc->registerConstantString("END", "END", scopePath);
    }

    // Register enumeration AudioSource::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AudioSource");
        sc->registerConstantString("INITIAL", "INITIAL", scopePath);
        sc->registerConstantString("PLAYING", "PLAYING", scopePath);
        sc->registerConstantString("PAUSED", "PAUSED", scopePath);
        sc->registerConstantString("STOPPED", "STOPPED", scopePath);
    }

    // Register enumeration Camera::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Camera");
        sc->registerConstantString("PERSPECTIVE", "PERSPECTIVE", scopePath);
        sc->registerConstantString("ORTHOGRAPHIC", "ORTHOGRAPHIC", scopePath);
    }

    // Register enumeration Container::Scroll.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Container");
        sc->registerConstantString("SCROLL_NONE", "SCROLL_NONE", scopePath);
        sc->registerConstantString("SCROLL_HORIZONTAL", "SCROLL_HORIZONTAL", scopePath);
        sc->registerConstantString("SCROLL_VERTICAL", "SCROLL_VERTICAL", scopePath);
        sc->registerConstantString("SCROLL_BOTH", "SCROLL_BOTH", scopePath);
    }

    // Register enumeration Control::Alignment.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        sc->registerConstantString("ALIGN_LEFT", "ALIGN_LEFT", scopePath);
        sc->registerConstantString("ALIGN_HCENTER", "ALIGN_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_RIGHT", "ALIGN_RIGHT", scopePath);
        sc->registerConstantString("ALIGN_TOP", "ALIGN_TOP", scopePath);
        sc->registerConstantString("ALIGN_VCENTER", "ALIGN_VCENTER", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM", "ALIGN_BOTTOM", scopePath);
        sc->registerConstantString("ALIGN_TOP_LEFT", "ALIGN_TOP_LEFT", scopePath);
        sc->registerConstantString("ALIGN_VCENTER_LEFT", "ALIGN_VCENTER_LEFT", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM_LEFT", "ALIGN_BOTTOM_LEFT", scopePath);
        sc->registerConstantString("ALIGN_TOP_HCENTER", "ALIGN_TOP_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_VCENTER_HCENTER", "ALIGN_VCENTER_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM_HCENTER", "ALIGN_BOTTOM_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_TOP_RIGHT", "ALIGN_TOP_RIGHT", scopePath);
        sc->registerConstantString("ALIGN_VCENTER_RIGHT", "ALIGN_VCENTER_RIGHT", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM_RIGHT", "ALIGN_BOTTOM_RIGHT", scopePath);
    }

    // Register enumeration Control::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        scopePath.push_back("Listener");
        sc->registerConstantString("PRESS", "PRESS", scopePath);
        sc->registerConstantString("RELEASE", "RELEASE", scopePath);
        sc->registerConstantString("CLICK", "CLICK", scopePath);
        sc->registerConstantString("VALUE_CHANGED", "VALUE_CHANGED", scopePath);
        sc->registerConstantString("TEXT_CHANGED", "TEXT_CHANGED", scopePath);
        sc->registerConstantString("MIDDLE_CLICK", "MIDDLE_CLICK", scopePath);
        sc->registerConstantString("RIGHT_CLICK", "RIGHT_CLICK", scopePath);
    }

    // Register enumeration Control::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        sc->registerConstantString("NORMAL", "NORMAL", scopePath);
        sc->registerConstantString("FOCUS", "FOCUS", scopePath);
        sc->registerConstantString("ACTIVE", "ACTIVE", scopePath);
        sc->registerConstantString("DISABLED", "DISABLED", scopePath);
    }

    // Register enumeration Curve::InterpolationType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Curve");
        sc->registerConstantString("BEZIER", "BEZIER", scopePath);
        sc->registerConstantString("BSPLINE", "BSPLINE", scopePath);
        sc->registerConstantString("FLAT", "FLAT", scopePath);
        sc->registerConstantString("HERMITE", "HERMITE", scopePath);
        sc->registerConstantString("LINEAR", "LINEAR", scopePath);
        sc->registerConstantString("SMOOTH", "SMOOTH", scopePath);
        sc->registerConstantString("STEP", "STEP", scopePath);
        sc->registerConstantString("QUADRATIC_IN", "QUADRATIC_IN", scopePath);
        sc->registerConstantString("QUADRATIC_OUT", "QUADRATIC_OUT", scopePath);
        sc->registerConstantString("QUADRATIC_IN_OUT", "QUADRATIC_IN_OUT", scopePath);
        sc->registerConstantString("QUADRATIC_OUT_IN", "QUADRATIC_OUT_IN", scopePath);
        sc->registerConstantString("CUBIC_IN", "CUBIC_IN", scopePath);
        sc->registerConstantString("CUBIC_OUT", "CUBIC_OUT", scopePath);
        sc->registerConstantString("CUBIC_IN_OUT", "CUBIC_IN_OUT", scopePath);
        sc->registerConstantString("CUBIC_OUT_IN", "CUBIC_OUT_IN", scopePath);
        sc->registerConstantString("QUARTIC_IN", "QUARTIC_IN", scopePath);
        sc->registerConstantString("QUARTIC_OUT", "QUARTIC_OUT", scopePath);
        sc->registerConstantString("QUARTIC_IN_OUT", "QUARTIC_IN_OUT", scopePath);
        sc->registerConstantString("QUARTIC_OUT_IN", "QUARTIC_OUT_IN", scopePath);
        sc->registerConstantString("QUINTIC_IN", "QUINTIC_IN", scopePath);
        sc->registerConstantString("QUINTIC_OUT", "QUINTIC_OUT", scopePath);
        sc->registerConstantString("QUINTIC_IN_OUT", "QUINTIC_IN_OUT", scopePath);
        sc->registerConstantString("QUINTIC_OUT_IN", "QUINTIC_OUT_IN", scopePath);
        sc->registerConstantString("SINE_IN", "SINE_IN", scopePath);
        sc->registerConstantString("SINE_OUT", "SINE_OUT", scopePath);
        sc->registerConstantString("SINE_IN_OUT", "SINE_IN_OUT", scopePath);
        sc->registerConstantString("SINE_OUT_IN", "SINE_OUT_IN", scopePath);
        sc->registerConstantString("EXPONENTIAL_IN", "EXPONENTIAL_IN", scopePath);
        sc->registerConstantString("EXPONENTIAL_OUT", "EXPONENTIAL_OUT", scopePath);
        sc->registerConstantString("EXPONENTIAL_IN_OUT", "EXPONENTIAL_IN_OUT", scopePath);
        sc->registerConstantString("EXPONENTIAL_OUT_IN", "EXPONENTIAL_OUT_IN", scopePath);
        sc->registerConstantString("CIRCULAR_IN", "CIRCULAR_IN", scopePath);
        sc->registerConstantString("CIRCULAR_OUT", "CIRCULAR_OUT", scopePath);
        sc->registerConstantString("CIRCULAR_IN_OUT", "CIRCULAR_IN_OUT", scopePath);
        sc->registerConstantString("CIRCULAR_OUT_IN", "CIRCULAR_OUT_IN", scopePath);
        sc->registerConstantString("ELASTIC_IN", "ELASTIC_IN", scopePath);
        sc->registerConstantString("ELASTIC_OUT", "ELASTIC_OUT", scopePath);
        sc->registerConstantString("ELASTIC_IN_OUT", "ELASTIC_IN_OUT", scopePath);
        sc->registerConstantString("ELASTIC_OUT_IN", "ELASTIC_OUT_IN", scopePath);
        sc->registerConstantString("OVERSHOOT_IN", "OVERSHOOT_IN", scopePath);
        sc->registerConstantString("OVERSHOOT_OUT", "OVERSHOOT_OUT", scopePath);
        sc->registerConstantString("OVERSHOOT_IN_OUT", "OVERSHOOT_IN_OUT", scopePath);
        sc->registerConstantString("OVERSHOOT_OUT_IN", "OVERSHOOT_OUT_IN", scopePath);
        sc->registerConstantString("BOUNCE_IN", "BOUNCE_IN", scopePath);
        sc->registerConstantString("BOUNCE_OUT", "BOUNCE_OUT", scopePath);
        sc->registerConstantString("BOUNCE_IN_OUT", "BOUNCE_IN_OUT", scopePath);
        sc->registerConstantString("BOUNCE_OUT_IN", "BOUNCE_OUT_IN", scopePath);
    }

    // Register enumeration DepthStencilTarget::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("DepthStencilTarget");
        sc->registerConstantString("DEPTH", "DEPTH", scopePath);
        sc->registerConstantString("DEPTH_STENCIL", "DEPTH_STENCIL", scopePath);
    }

    // Register enumeration Font::Justify.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Font");
        sc->registerConstantString("ALIGN_LEFT", "ALIGN_LEFT", scopePath);
        sc->registerConstantString("ALIGN_HCENTER", "ALIGN_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_RIGHT", "ALIGN_RIGHT", scopePath);
        sc->registerConstantString("ALIGN_TOP", "ALIGN_TOP", scopePath);
        sc->registerConstantString("ALIGN_VCENTER", "ALIGN_VCENTER", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM", "ALIGN_BOTTOM", scopePath);
        sc->registerConstantString("ALIGN_TOP_LEFT", "ALIGN_TOP_LEFT", scopePath);
        sc->registerConstantString("ALIGN_VCENTER_LEFT", "ALIGN_VCENTER_LEFT", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM_LEFT", "ALIGN_BOTTOM_LEFT", scopePath);
        sc->registerConstantString("ALIGN_TOP_HCENTER", "ALIGN_TOP_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_VCENTER_HCENTER", "ALIGN_VCENTER_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM_HCENTER", "ALIGN_BOTTOM_HCENTER", scopePath);
        sc->registerConstantString("ALIGN_TOP_RIGHT", "ALIGN_TOP_RIGHT", scopePath);
        sc->registerConstantString("ALIGN_VCENTER_RIGHT", "ALIGN_VCENTER_RIGHT", scopePath);
        sc->registerConstantString("ALIGN_BOTTOM_RIGHT", "ALIGN_BOTTOM_RIGHT", scopePath);
    }

    // Register enumeration Font::Style.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Font");
        sc->registerConstantString("PLAIN", "PLAIN", scopePath);
        sc->registerConstantString("BOLD", "BOLD", scopePath);
        sc->registerConstantString("ITALIC", "ITALIC", scopePath);
        sc->registerConstantString("BOLD_ITALIC", "BOLD_ITALIC", scopePath);
    }

    // Register enumeration Game::ClearFlags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Game");
        sc->registerConstantString("CLEAR_COLOR", "CLEAR_COLOR", scopePath);
        sc->registerConstantString("CLEAR_DEPTH", "CLEAR_DEPTH", scopePath);
        sc->registerConstantString("CLEAR_STENCIL", "CLEAR_STENCIL", scopePath);
        sc->registerConstantString("CLEAR_COLOR_DEPTH", "CLEAR_COLOR_DEPTH", scopePath);
        sc->registerConstantString("CLEAR_COLOR_STENCIL", "CLEAR_COLOR_STENCIL", scopePath);
        sc->registerConstantString("CLEAR_DEPTH_STENCIL", "CLEAR_DEPTH_STENCIL", scopePath);
        sc->registerConstantString("CLEAR_COLOR_DEPTH_STENCIL", "CLEAR_COLOR_DEPTH_STENCIL", scopePath);
    }

    // Register enumeration Game::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Game");
        sc->registerConstantString("UNINITIALIZED", "UNINITIALIZED", scopePath);
        sc->registerConstantString("RUNNING", "RUNNING", scopePath);
        sc->registerConstantString("PAUSED", "PAUSED", scopePath);
    }

    // Register enumeration Gamepad::ButtonState.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gamepad");
        sc->registerConstantString("BUTTON_PRESSED", "BUTTON_PRESSED", scopePath);
        sc->registerConstantString("BUTTON_RELEASED", "BUTTON_RELEASED", scopePath);
    }

    // Register enumeration Gamepad::GamepadEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gamepad");
        sc->registerConstantString("ATTACHED_EVENT", "ATTACHED_EVENT", scopePath);
        sc->registerConstantString("DETACHED_EVENT", "DETACHED_EVENT", scopePath);
    }

    // Register enumeration Image::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Image");
        sc->registerConstantString("RGB", "RGB", scopePath);
        sc->registerConstantString("RGBA", "RGBA", scopePath);
    }

    // Register enumeration Keyboard::Key.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Keyboard");
        sc->registerConstantString("KEY_NONE", "KEY_NONE", scopePath);
        sc->registerConstantString("KEY_PAUSE", "KEY_PAUSE", scopePath);
        sc->registerConstantString("KEY_SCROLL_LOCK", "KEY_SCROLL_LOCK", scopePath);
        sc->registerConstantString("KEY_PRINT", "KEY_PRINT", scopePath);
        sc->registerConstantString("KEY_SYSREQ", "KEY_SYSREQ", scopePath);
        sc->registerConstantString("KEY_BREAK", "KEY_BREAK", scopePath);
        sc->registerConstantString("KEY_ESCAPE", "KEY_ESCAPE", scopePath);
        sc->registerConstantString("KEY_BACKSPACE", "KEY_BACKSPACE", scopePath);
        sc->registerConstantString("KEY_TAB", "KEY_TAB", scopePath);
        sc->registerConstantString("KEY_BACK_TAB", "KEY_BACK_TAB", scopePath);
        sc->registerConstantString("KEY_RETURN", "KEY_RETURN", scopePath);
        sc->registerConstantString("KEY_CAPS_LOCK", "KEY_CAPS_LOCK", scopePath);
        sc->registerConstantString("KEY_SHIFT", "KEY_SHIFT", scopePath);
        sc->registerConstantString("KEY_CTRL", "KEY_CTRL", scopePath);
        sc->registerConstantString("KEY_ALT", "KEY_ALT", scopePath);
        sc->registerConstantString("KEY_MENU", "KEY_MENU", scopePath);
        sc->registerConstantString("KEY_HYPER", "KEY_HYPER", scopePath);
        sc->registerConstantString("KEY_INSERT", "KEY_INSERT", scopePath);
        sc->registerConstantString("KEY_HOME", "KEY_HOME", scopePath);
        sc->registerConstantString("KEY_PG_UP", "KEY_PG_UP", scopePath);
        sc->registerConstantString("KEY_DELETE", "KEY_DELETE", scopePath);
        sc->registerConstantString("KEY_END", "KEY_END", scopePath);
        sc->registerConstantString("KEY_PG_DOWN", "KEY_PG_DOWN", scopePath);
        sc->registerConstantString("KEY_LEFT_ARROW", "KEY_LEFT_ARROW", scopePath);
        sc->registerConstantString("KEY_RIGHT_ARROW", "KEY_RIGHT_ARROW", scopePath);
        sc->registerConstantString("KEY_UP_ARROW", "KEY_UP_ARROW", scopePath);
        sc->registerConstantString("KEY_DOWN_ARROW", "KEY_DOWN_ARROW", scopePath);
        sc->registerConstantString("KEY_NUM_LOCK", "KEY_NUM_LOCK", scopePath);
        sc->registerConstantString("KEY_KP_PLUS", "KEY_KP_PLUS", scopePath);
        sc->registerConstantString("KEY_KP_MINUS", "KEY_KP_MINUS", scopePath);
        sc->registerConstantString("KEY_KP_MULTIPLY", "KEY_KP_MULTIPLY", scopePath);
        sc->registerConstantString("KEY_KP_DIVIDE", "KEY_KP_DIVIDE", scopePath);
        sc->registerConstantString("KEY_KP_ENTER", "KEY_KP_ENTER", scopePath);
        sc->registerConstantString("KEY_KP_HOME", "KEY_KP_HOME", scopePath);
        sc->registerConstantString("KEY_KP_UP", "KEY_KP_UP", scopePath);
        sc->registerConstantString("KEY_KP_PG_UP", "KEY_KP_PG_UP", scopePath);
        sc->registerConstantString("KEY_KP_LEFT", "KEY_KP_LEFT", scopePath);
        sc->registerConstantString("KEY_KP_FIVE", "KEY_KP_FIVE", scopePath);
        sc->registerConstantString("KEY_KP_RIGHT", "KEY_KP_RIGHT", scopePath);
        sc->registerConstantString("KEY_KP_END", "KEY_KP_END", scopePath);
        sc->registerConstantString("KEY_KP_DOWN", "KEY_KP_DOWN", scopePath);
        sc->registerConstantString("KEY_KP_PG_DOWN", "KEY_KP_PG_DOWN", scopePath);
        sc->registerConstantString("KEY_KP_INSERT", "KEY_KP_INSERT", scopePath);
        sc->registerConstantString("KEY_KP_DELETE", "KEY_KP_DELETE", scopePath);
        sc->registerConstantString("KEY_F1", "KEY_F1", scopePath);
        sc->registerConstantString("KEY_F2", "KEY_F2", scopePath);
        sc->registerConstantString("KEY_F3", "KEY_F3", scopePath);
        sc->registerConstantString("KEY_F4", "KEY_F4", scopePath);
        sc->registerConstantString("KEY_F5", "KEY_F5", scopePath);
        sc->registerConstantString("KEY_F6", "KEY_F6", scopePath);
        sc->registerConstantString("KEY_F7", "KEY_F7", scopePath);
        sc->registerConstantString("KEY_F8", "KEY_F8", scopePath);
        sc->registerConstantString("KEY_F9", "KEY_F9", scopePath);
        sc->registerConstantString("KEY_F10", "KEY_F10", scopePath);
        sc->registerConstantString("KEY_F11", "KEY_F11", scopePath);
        sc->registerConstantString("KEY_F12", "KEY_F12", scopePath);
        sc->registerConstantString("KEY_SPACE", "KEY_SPACE", scopePath);
        sc->registerConstantString("KEY_EXCLAM", "KEY_EXCLAM", scopePath);
        sc->registerConstantString("KEY_QUOTE", "KEY_QUOTE", scopePath);
        sc->registerConstantString("KEY_NUMBER", "KEY_NUMBER", scopePath);
        sc->registerConstantString("KEY_DOLLAR", "KEY_DOLLAR", scopePath);
        sc->registerConstantString("KEY_PERCENT", "KEY_PERCENT", scopePath);
        sc->registerConstantString("KEY_CIRCUMFLEX", "KEY_CIRCUMFLEX", scopePath);
        sc->registerConstantString("KEY_AMPERSAND", "KEY_AMPERSAND", scopePath);
        sc->registerConstantString("KEY_APOSTROPHE", "KEY_APOSTROPHE", scopePath);
        sc->registerConstantString("KEY_LEFT_PARENTHESIS", "KEY_LEFT_PARENTHESIS", scopePath);
        sc->registerConstantString("KEY_RIGHT_PARENTHESIS", "KEY_RIGHT_PARENTHESIS", scopePath);
        sc->registerConstantString("KEY_ASTERISK", "KEY_ASTERISK", scopePath);
        sc->registerConstantString("KEY_PLUS", "KEY_PLUS", scopePath);
        sc->registerConstantString("KEY_COMMA", "KEY_COMMA", scopePath);
        sc->registerConstantString("KEY_MINUS", "KEY_MINUS", scopePath);
        sc->registerConstantString("KEY_PERIOD", "KEY_PERIOD", scopePath);
        sc->registerConstantString("KEY_SLASH", "KEY_SLASH", scopePath);
        sc->registerConstantString("KEY_ZERO", "KEY_ZERO", scopePath);
        sc->registerConstantString("KEY_ONE", "KEY_ONE", scopePath);
        sc->registerConstantString("KEY_TWO", "KEY_TWO", scopePath);
        sc->registerConstantString("KEY_THREE", "KEY_THREE", scopePath);
        sc->registerConstantString("KEY_FOUR", "KEY_FOUR", scopePath);
        sc->registerConstantString("KEY_FIVE", "KEY_FIVE", scopePath);
        sc->registerConstantString("KEY_SIX", "KEY_SIX", scopePath);
        sc->registerConstantString("KEY_SEVEN", "KEY_SEVEN", scopePath);
        sc->registerConstantString("KEY_EIGHT", "KEY_EIGHT", scopePath);
        sc->registerConstantString("KEY_NINE", "KEY_NINE", scopePath);
        sc->registerConstantString("KEY_COLON", "KEY_COLON", scopePath);
        sc->registerConstantString("KEY_SEMICOLON", "KEY_SEMICOLON", scopePath);
        sc->registerConstantString("KEY_LESS_THAN", "KEY_LESS_THAN", scopePath);
        sc->registerConstantString("KEY_EQUAL", "KEY_EQUAL", scopePath);
        sc->registerConstantString("KEY_GREATER_THAN", "KEY_GREATER_THAN", scopePath);
        sc->registerConstantString("KEY_QUESTION", "KEY_QUESTION", scopePath);
        sc->registerConstantString("KEY_AT", "KEY_AT", scopePath);
        sc->registerConstantString("KEY_CAPITAL_A", "KEY_CAPITAL_A", scopePath);
        sc->registerConstantString("KEY_CAPITAL_B", "KEY_CAPITAL_B", scopePath);
        sc->registerConstantString("KEY_CAPITAL_C", "KEY_CAPITAL_C", scopePath);
        sc->registerConstantString("KEY_CAPITAL_D", "KEY_CAPITAL_D", scopePath);
        sc->registerConstantString("KEY_CAPITAL_E", "KEY_CAPITAL_E", scopePath);
        sc->registerConstantString("KEY_CAPITAL_F", "KEY_CAPITAL_F", scopePath);
        sc->registerConstantString("KEY_CAPITAL_G", "KEY_CAPITAL_G", scopePath);
        sc->registerConstantString("KEY_CAPITAL_H", "KEY_CAPITAL_H", scopePath);
        sc->registerConstantString("KEY_CAPITAL_I", "KEY_CAPITAL_I", scopePath);
        sc->registerConstantString("KEY_CAPITAL_J", "KEY_CAPITAL_J", scopePath);
        sc->registerConstantString("KEY_CAPITAL_K", "KEY_CAPITAL_K", scopePath);
        sc->registerConstantString("KEY_CAPITAL_L", "KEY_CAPITAL_L", scopePath);
        sc->registerConstantString("KEY_CAPITAL_M", "KEY_CAPITAL_M", scopePath);
        sc->registerConstantString("KEY_CAPITAL_N", "KEY_CAPITAL_N", scopePath);
        sc->registerConstantString("KEY_CAPITAL_O", "KEY_CAPITAL_O", scopePath);
        sc->registerConstantString("KEY_CAPITAL_P", "KEY_CAPITAL_P", scopePath);
        sc->registerConstantString("KEY_CAPITAL_Q", "KEY_CAPITAL_Q", scopePath);
        sc->registerConstantString("KEY_CAPITAL_R", "KEY_CAPITAL_R", scopePath);
        sc->registerConstantString("KEY_CAPITAL_S", "KEY_CAPITAL_S", scopePath);
        sc->registerConstantString("KEY_CAPITAL_T", "KEY_CAPITAL_T", scopePath);
        sc->registerConstantString("KEY_CAPITAL_U", "KEY_CAPITAL_U", scopePath);
        sc->registerConstantString("KEY_CAPITAL_V", "KEY_CAPITAL_V", scopePath);
        sc->registerConstantString("KEY_CAPITAL_W", "KEY_CAPITAL_W", scopePath);
        sc->registerConstantString("KEY_CAPITAL_X", "KEY_CAPITAL_X", scopePath);
        sc->registerConstantString("KEY_CAPITAL_Y", "KEY_CAPITAL_Y", scopePath);
        sc->registerConstantString("KEY_CAPITAL_Z", "KEY_CAPITAL_Z", scopePath);
        sc->registerConstantString("KEY_LEFT_BRACKET", "KEY_LEFT_BRACKET", scopePath);
        sc->registerConstantString("KEY_BACK_SLASH", "KEY_BACK_SLASH", scopePath);
        sc->registerConstantString("KEY_RIGHT_BRACKET", "KEY_RIGHT_BRACKET", scopePath);
        sc->registerConstantString("KEY_UNDERSCORE", "KEY_UNDERSCORE", scopePath);
        sc->registerConstantString("KEY_GRAVE", "KEY_GRAVE", scopePath);
        sc->registerConstantString("KEY_A", "KEY_A", scopePath);
        sc->registerConstantString("KEY_B", "KEY_B", scopePath);
        sc->registerConstantString("KEY_C", "KEY_C", scopePath);
        sc->registerConstantString("KEY_D", "KEY_D", scopePath);
        sc->registerConstantString("KEY_E", "KEY_E", scopePath);
        sc->registerConstantString("KEY_F", "KEY_F", scopePath);
        sc->registerConstantString("KEY_G", "KEY_G", scopePath);
        sc->registerConstantString("KEY_H", "KEY_H", scopePath);
        sc->registerConstantString("KEY_I", "KEY_I", scopePath);
        sc->registerConstantString("KEY_J", "KEY_J", scopePath);
        sc->registerConstantString("KEY_K", "KEY_K", scopePath);
        sc->registerConstantString("KEY_L", "KEY_L", scopePath);
        sc->registerConstantString("KEY_M", "KEY_M", scopePath);
        sc->registerConstantString("KEY_N", "KEY_N", scopePath);
        sc->registerConstantString("KEY_O", "KEY_O", scopePath);
        sc->registerConstantString("KEY_P", "KEY_P", scopePath);
        sc->registerConstantString("KEY_Q", "KEY_Q", scopePath);
        sc->registerConstantString("KEY_R", "KEY_R", scopePath);
        sc->registerConstantString("KEY_S", "KEY_S", scopePath);
        sc->registerConstantString("KEY_T", "KEY_T", scopePath);
        sc->registerConstantString("KEY_U", "KEY_U", scopePath);
        sc->registerConstantString("KEY_V", "KEY_V", scopePath);
        sc->registerConstantString("KEY_W", "KEY_W", scopePath);
        sc->registerConstantString("KEY_X", "KEY_X", scopePath);
        sc->registerConstantString("KEY_Y", "KEY_Y", scopePath);
        sc->registerConstantString("KEY_Z", "KEY_Z", scopePath);
        sc->registerConstantString("KEY_LEFT_BRACE", "KEY_LEFT_BRACE", scopePath);
        sc->registerConstantString("KEY_BAR", "KEY_BAR", scopePath);
        sc->registerConstantString("KEY_RIGHT_BRACE", "KEY_RIGHT_BRACE", scopePath);
        sc->registerConstantString("KEY_TILDE", "KEY_TILDE", scopePath);
        sc->registerConstantString("KEY_EURO", "KEY_EURO", scopePath);
        sc->registerConstantString("KEY_POUND", "KEY_POUND", scopePath);
        sc->registerConstantString("KEY_YEN", "KEY_YEN", scopePath);
        sc->registerConstantString("KEY_MIDDLE_DOT", "KEY_MIDDLE_DOT", scopePath);
        sc->registerConstantString("KEY_SEARCH", "KEY_SEARCH", scopePath);
    }

    // Register enumeration Keyboard::KeyEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Keyboard");
        sc->registerConstantString("KEY_PRESS", "KEY_PRESS", scopePath);
        sc->registerConstantString("KEY_RELEASE", "KEY_RELEASE", scopePath);
        sc->registerConstantString("KEY_CHAR", "KEY_CHAR", scopePath);
    }

    // Register enumeration Layout::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Layout");
        sc->registerConstantString("LAYOUT_FLOW", "LAYOUT_FLOW", scopePath);
        sc->registerConstantString("LAYOUT_VERTICAL", "LAYOUT_VERTICAL", scopePath);
        sc->registerConstantString("LAYOUT_ABSOLUTE", "LAYOUT_ABSOLUTE", scopePath);
        sc->registerConstantString("LAYOUT_SCROLL", "LAYOUT_SCROLL", scopePath);
    }

    // Register enumeration Light::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Light");
        sc->registerConstantString("DIRECTIONAL", "DIRECTIONAL", scopePath);
        sc->registerConstantString("POINT", "POINT", scopePath);
        sc->registerConstantString("SPOT", "SPOT", scopePath);
    }

    // Register enumeration Mesh::IndexFormat.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mesh");
        sc->registerConstantString("INDEX8", "INDEX8", scopePath);
        sc->registerConstantString("INDEX16", "INDEX16", scopePath);
        sc->registerConstantString("INDEX32", "INDEX32", scopePath);
    }

    // Register enumeration Mesh::PrimitiveType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mesh");
        sc->registerConstantString("TRIANGLES", "TRIANGLES", scopePath);
        sc->registerConstantString("TRIANGLE_STRIP", "TRIANGLE_STRIP", scopePath);
        sc->registerConstantString("LINES", "LINES", scopePath);
        sc->registerConstantString("LINE_STRIP", "LINE_STRIP", scopePath);
        sc->registerConstantString("POINTS", "POINTS", scopePath);
    }

    // Register enumeration Mouse::MouseEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mouse");
        sc->registerConstantString("MOUSE_PRESS_LEFT_BUTTON", "MOUSE_PRESS_LEFT_BUTTON", scopePath);
        sc->registerConstantString("MOUSE_RELEASE_LEFT_BUTTON", "MOUSE_RELEASE_LEFT_BUTTON", scopePath);
        sc->registerConstantString("MOUSE_PRESS_MIDDLE_BUTTON", "MOUSE_PRESS_MIDDLE_BUTTON", scopePath);
        sc->registerConstantString("MOUSE_RELEASE_MIDDLE_BUTTON", "MOUSE_RELEASE_MIDDLE_BUTTON", scopePath);
        sc->registerConstantString("MOUSE_PRESS_RIGHT_BUTTON", "MOUSE_PRESS_RIGHT_BUTTON", scopePath);
        sc->registerConstantString("MOUSE_RELEASE_RIGHT_BUTTON", "MOUSE_RELEASE_RIGHT_BUTTON", scopePath);
        sc->registerConstantString("MOUSE_MOVE", "MOUSE_MOVE", scopePath);
        sc->registerConstantString("MOUSE_WHEEL", "MOUSE_WHEEL", scopePath);
    }

    // Register enumeration Node::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Node");
        sc->registerConstantString("NODE", "NODE", scopePath);
        sc->registerConstantString("JOINT", "JOINT", scopePath);
    }

    // Register enumeration ParticleEmitter::TextureBlending.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("ParticleEmitter");
        sc->registerConstantString("BLEND_OPAQUE", "BLEND_OPAQUE", scopePath);
        sc->registerConstantString("BLEND_TRANSPARENT", "BLEND_TRANSPARENT", scopePath);
        sc->registerConstantString("BLEND_ADDITIVE", "BLEND_ADDITIVE", scopePath);
        sc->registerConstantString("BLEND_MULTIPLIED", "BLEND_MULTIPLIED", scopePath);
    }

    // Register enumeration PhysicsCollisionObject::CollisionListener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionObject");
        scopePath.push_back("CollisionListener");
        sc->registerConstantString("COLLIDING", "COLLIDING", scopePath);
        sc->registerConstantString("NOT_COLLIDING", "NOT_COLLIDING", scopePath);
    }

    // Register enumeration PhysicsCollisionObject::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionObject");
        sc->registerConstantString("RIGID_BODY", "RIGID_BODY", scopePath);
        sc->registerConstantString("CHARACTER", "CHARACTER", scopePath);
        sc->registerConstantString("GHOST_OBJECT", "GHOST_OBJECT", scopePath);
        sc->registerConstantString("NONE", "NONE", scopePath);
    }

    // Register enumeration PhysicsCollisionShape::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionShape");
        sc->registerConstantString("SHAPE_BOX", "SHAPE_BOX", scopePath);
        sc->registerConstantString("SHAPE_SPHERE", "SHAPE_SPHERE", scopePath);
        sc->registerConstantString("SHAPE_CAPSULE", "SHAPE_CAPSULE", scopePath);
        sc->registerConstantString("SHAPE_MESH", "SHAPE_MESH", scopePath);
        sc->registerConstantString("SHAPE_HEIGHTFIELD", "SHAPE_HEIGHTFIELD", scopePath);
    }

    // Register enumeration PhysicsController::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsController");
        scopePath.push_back("Listener");
        sc->registerConstantString("ACTIVATED", "ACTIVATED", scopePath);
        sc->registerConstantString("DEACTIVATED", "DEACTIVATED", scopePath);
    }

    // Register enumeration Properties::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Properties");
        sc->registerConstantString("NONE", "NONE", scopePath);
        sc->registerConstantString("STRING", "STRING", scopePath);
        sc->registerConstantString("NUMBER", "NUMBER", scopePath);
        sc->registerConstantString("VECTOR2", "VECTOR2", scopePath);
        sc->registerConstantString("VECTOR3", "VECTOR3", scopePath);
        sc->registerConstantString("VECTOR4", "VECTOR4", scopePath);
        sc->registerConstantString("MATRIX", "MATRIX", scopePath);
    }

    // Register enumeration RenderState::AutoBinding.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        sc->registerConstantString("NONE", "NONE", scopePath);
        sc->registerConstantString("WORLD_MATRIX", "WORLD_MATRIX", scopePath);
        sc->registerConstantString("VIEW_MATRIX", "VIEW_MATRIX", scopePath);
        sc->registerConstantString("PROJECTION_MATRIX", "PROJECTION_MATRIX", scopePath);
        sc->registerConstantString("WORLD_VIEW_MATRIX", "WORLD_VIEW_MATRIX", scopePath);
        sc->registerConstantString("VIEW_PROJECTION_MATRIX", "VIEW_PROJECTION_MATRIX", scopePath);
        sc->registerConstantString("WORLD_VIEW_PROJECTION_MATRIX", "WORLD_VIEW_PROJECTION_MATRIX", scopePath);
        sc->registerConstantString("INVERSE_TRANSPOSE_WORLD_MATRIX", "INVERSE_TRANSPOSE_WORLD_MATRIX", scopePath);
        sc->registerConstantString("INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX", scopePath);
        sc->registerConstantString("CAMERA_WORLD_POSITION", "CAMERA_WORLD_POSITION", scopePath);
        sc->registerConstantString("CAMERA_VIEW_POSITION", "CAMERA_VIEW_POSITION", scopePath);
        sc->registerConstantString("MATRIX_PALETTE", "MATRIX_PALETTE", scopePath);
    }

    // Register enumeration RenderState::Blend.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        sc->registerConstantString("BLEND_ZERO", "BLEND_ZERO", scopePath);
        sc->registerConstantString("BLEND_ONE", "BLEND_ONE", scopePath);
        sc->registerConstantString("BLEND_SRC_COLOR", "BLEND_SRC_COLOR", scopePath);
        sc->registerConstantString("BLEN_ONE_MINUS_SRC_COLOR", "BLEN_ONE_MINUS_SRC_COLOR", scopePath);
        sc->registerConstantString("BLEND_DST_COLOR", "BLEND_DST_COLOR", scopePath);
        sc->registerConstantString("BLEND_ONE_MINUS_DST_COLOR", "BLEND_ONE_MINUS_DST_COLOR", scopePath);
        sc->registerConstantString("BLEND_SRC_ALPHA", "BLEND_SRC_ALPHA", scopePath);
        sc->registerConstantString("BLEND_ONE_MINUS_SRC_ALPHA", "BLEND_ONE_MINUS_SRC_ALPHA", scopePath);
        sc->registerConstantString("BLEND_DST_ALPHA", "BLEND_DST_ALPHA", scopePath);
        sc->registerConstantString("BLEND_ONE_MINUS_DST_ALPHA", "BLEND_ONE_MINUS_DST_ALPHA", scopePath);
        sc->registerConstantString("BLEND_CONSTANT_ALPHA", "BLEND_CONSTANT_ALPHA", scopePath);
        sc->registerConstantString("BLEND_ONE_MINUS_CONSTANT_ALPHA", "BLEND_ONE_MINUS_CONSTANT_ALPHA", scopePath);
        sc->registerConstantString("BLEND_SRC_ALPHA_SATURATE", "BLEND_SRC_ALPHA_SATURATE", scopePath);
    }

    // Register enumeration Scene::DebugFlags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Scene");
        sc->registerConstantString("DEBUG_BOXES", "DEBUG_BOXES", scopePath);
        sc->registerConstantString("DEBUG_SPHERES", "DEBUG_SPHERES", scopePath);
    }

    // Register enumeration Texture::Filter.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        sc->registerConstantString("NEAREST", "NEAREST", scopePath);
        sc->registerConstantString("LINEAR", "LINEAR", scopePath);
        sc->registerConstantString("NEAREST_MIPMAP_NEAREST", "NEAREST_MIPMAP_NEAREST", scopePath);
        sc->registerConstantString("LINEAR_MIPMAP_NEAREST", "LINEAR_MIPMAP_NEAREST", scopePath);
        sc->registerConstantString("NEAREST_MIPMAP_LINEAR", "NEAREST_MIPMAP_LINEAR", scopePath);
        sc->registerConstantString("LINEAR_MIPMAP_LINEAR", "LINEAR_MIPMAP_LINEAR", scopePath);
    }

    // Register enumeration Texture::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        sc->registerConstantString("RGB", "RGB", scopePath);
        sc->registerConstantString("RGBA", "RGBA", scopePath);
        sc->registerConstantString("ALPHA", "ALPHA", scopePath);
    }

    // Register enumeration Texture::Wrap.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        sc->registerConstantString("REPEAT", "REPEAT", scopePath);
        sc->registerConstantString("CLAMP", "CLAMP", scopePath);
    }

    // Register enumeration Touch::TouchEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Touch");
        sc->registerConstantString("TOUCH_PRESS", "TOUCH_PRESS", scopePath);
        sc->registerConstantString("TOUCH_RELEASE", "TOUCH_RELEASE", scopePath);
        sc->registerConstantString("TOUCH_MOVE", "TOUCH_MOVE", scopePath);
    }

    // Register enumeration VertexFormat::Usage.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("VertexFormat");
        sc->registerConstantString("POSITION", "POSITION", scopePath);
        sc->registerConstantString("NORMAL", "NORMAL", scopePath);
        sc->registerConstantString("COLOR", "COLOR", scopePath);
        sc->registerConstantString("TANGENT", "TANGENT", scopePath);
        sc->registerConstantString("BINORMAL", "BINORMAL", scopePath);
        sc->registerConstantString("BLENDWEIGHTS", "BLENDWEIGHTS", scopePath);
        sc->registerConstantString("BLENDINDICES", "BLENDINDICES", scopePath);
        sc->registerConstantString("TEXCOORD0", "TEXCOORD0", scopePath);
        sc->registerConstantString("TEXCOORD1", "TEXCOORD1", scopePath);
        sc->registerConstantString("TEXCOORD2", "TEXCOORD2", scopePath);
        sc->registerConstantString("TEXCOORD3", "TEXCOORD3", scopePath);
        sc->registerConstantString("TEXCOORD4", "TEXCOORD4", scopePath);
        sc->registerConstantString("TEXCOORD5", "TEXCOORD5", scopePath);
        sc->registerConstantString("TEXCOORD6", "TEXCOORD6", scopePath);
        sc->registerConstantString("TEXCOORD7", "TEXCOORD7", scopePath);
    }
}

int lua____al_error_code(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 1)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0 or 1).");
        lua_error(state);
    }

    if (lua_gettop(state) == 1)
    {
        // Get parameter 1 off the stack.
        GP_WARN("Attempting to get parameter 1 with unrecognized type ALenum as an unsigned integer.");
        ALenum param1 = (ALenum)luaL_checkunsigned(state, 1);

        __al_error_code = param1;
        return 0;
    }
    else
    {
        ALenum result = __al_error_code;

        // Push the return value onto the stack.
        GP_WARN("Attempting to return value with unrecognized type ALenum as an unsigned integer.");
        lua_pushunsigned(state, result);

        return 1;
    }
}

int lua____gl_error_code(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 1)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0 or 1).");
        lua_error(state);
    }

    if (lua_gettop(state) == 1)
    {
        // Get parameter 1 off the stack.
        GP_WARN("Attempting to get parameter 1 with unrecognized type GLenum as an unsigned integer.");
        GLenum param1 = (GLenum)luaL_checkunsigned(state, 1);

        __gl_error_code = param1;
        return 0;
    }
    else
    {
        GLenum result = __gl_error_code;

        // Push the return value onto the stack.
        GP_WARN("Attempting to return value with unrecognized type GLenum as an unsigned integer.");
        lua_pushunsigned(state, result);

        return 1;
    }
}

int lua__printError(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = ScriptController::getInstance()->getString(1, false);

                printError(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua__printError - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

static const char* enumStringEmpty = "";

const char* lua_stringFromEnumGlobal(std::string& enumname, unsigned int value)
{
    if (enumname == "AnimationClip::Listener::EventType")
        return lua_stringFromEnum_AnimationClipListenerEventType((AnimationClip::Listener::EventType)value);
    if (enumname == "AudioSource::State")
        return lua_stringFromEnum_AudioSourceState((AudioSource::State)value);
    if (enumname == "Camera::Type")
        return lua_stringFromEnum_CameraType((Camera::Type)value);
    if (enumname == "Container::Scroll")
        return lua_stringFromEnum_ContainerScroll((Container::Scroll)value);
    if (enumname == "Control::Alignment")
        return lua_stringFromEnum_ControlAlignment((Control::Alignment)value);
    if (enumname == "Control::Listener::EventType")
        return lua_stringFromEnum_ControlListenerEventType((Control::Listener::EventType)value);
    if (enumname == "Control::State")
        return lua_stringFromEnum_ControlState((Control::State)value);
    if (enumname == "Curve::InterpolationType")
        return lua_stringFromEnum_CurveInterpolationType((Curve::InterpolationType)value);
    if (enumname == "DepthStencilTarget::Format")
        return lua_stringFromEnum_DepthStencilTargetFormat((DepthStencilTarget::Format)value);
    if (enumname == "Font::Justify")
        return lua_stringFromEnum_FontJustify((Font::Justify)value);
    if (enumname == "Font::Style")
        return lua_stringFromEnum_FontStyle((Font::Style)value);
    if (enumname == "Game::ClearFlags")
        return lua_stringFromEnum_GameClearFlags((Game::ClearFlags)value);
    if (enumname == "Game::State")
        return lua_stringFromEnum_GameState((Game::State)value);
    if (enumname == "Gamepad::ButtonState")
        return lua_stringFromEnum_GamepadButtonState((Gamepad::ButtonState)value);
    if (enumname == "Gamepad::GamepadEvent")
        return lua_stringFromEnum_GamepadGamepadEvent((Gamepad::GamepadEvent)value);
    if (enumname == "Image::Format")
        return lua_stringFromEnum_ImageFormat((Image::Format)value);
    if (enumname == "Keyboard::Key")
        return lua_stringFromEnum_KeyboardKey((Keyboard::Key)value);
    if (enumname == "Keyboard::KeyEvent")
        return lua_stringFromEnum_KeyboardKeyEvent((Keyboard::KeyEvent)value);
    if (enumname == "Layout::Type")
        return lua_stringFromEnum_LayoutType((Layout::Type)value);
    if (enumname == "Light::Type")
        return lua_stringFromEnum_LightType((Light::Type)value);
    if (enumname == "Mesh::IndexFormat")
        return lua_stringFromEnum_MeshIndexFormat((Mesh::IndexFormat)value);
    if (enumname == "Mesh::PrimitiveType")
        return lua_stringFromEnum_MeshPrimitiveType((Mesh::PrimitiveType)value);
    if (enumname == "Mouse::MouseEvent")
        return lua_stringFromEnum_MouseMouseEvent((Mouse::MouseEvent)value);
    if (enumname == "Node::Type")
        return lua_stringFromEnum_NodeType((Node::Type)value);
    if (enumname == "ParticleEmitter::TextureBlending")
        return lua_stringFromEnum_ParticleEmitterTextureBlending((ParticleEmitter::TextureBlending)value);
    if (enumname == "PhysicsCollisionObject::CollisionListener::EventType")
        return lua_stringFromEnum_PhysicsCollisionObjectCollisionListenerEventType((PhysicsCollisionObject::CollisionListener::EventType)value);
    if (enumname == "PhysicsCollisionObject::Type")
        return lua_stringFromEnum_PhysicsCollisionObjectType((PhysicsCollisionObject::Type)value);
    if (enumname == "PhysicsCollisionShape::Type")
        return lua_stringFromEnum_PhysicsCollisionShapeType((PhysicsCollisionShape::Type)value);
    if (enumname == "PhysicsController::Listener::EventType")
        return lua_stringFromEnum_PhysicsControllerListenerEventType((PhysicsController::Listener::EventType)value);
    if (enumname == "Properties::Type")
        return lua_stringFromEnum_PropertiesType((Properties::Type)value);
    if (enumname == "RenderState::AutoBinding")
        return lua_stringFromEnum_RenderStateAutoBinding((RenderState::AutoBinding)value);
    if (enumname == "RenderState::Blend")
        return lua_stringFromEnum_RenderStateBlend((RenderState::Blend)value);
    if (enumname == "Scene::DebugFlags")
        return lua_stringFromEnum_SceneDebugFlags((Scene::DebugFlags)value);
    if (enumname == "Texture::Filter")
        return lua_stringFromEnum_TextureFilter((Texture::Filter)value);
    if (enumname == "Texture::Format")
        return lua_stringFromEnum_TextureFormat((Texture::Format)value);
    if (enumname == "Texture::Wrap")
        return lua_stringFromEnum_TextureWrap((Texture::Wrap)value);
    if (enumname == "Touch::TouchEvent")
        return lua_stringFromEnum_TouchTouchEvent((Touch::TouchEvent)value);
    if (enumname == "VertexFormat::Usage")
        return lua_stringFromEnum_VertexFormatUsage((VertexFormat::Usage)value);

    GP_ERROR("Unrecognized enumeration type '%s'.", enumname.c_str());
    return enumStringEmpty;
}

}
