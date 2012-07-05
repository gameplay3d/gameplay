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
    hierarchy["PhysicsConstraint"].push_back("PhysicsFixedConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsGenericConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsHingeConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsSocketConstraint");
    hierarchy["PhysicsConstraint"].push_back("PhysicsSpringConstraint");
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
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
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

static const char* luaEnumString_AnimationClipListenerEventType_DEFAULT = "DEFAULT";
static const char* luaEnumString_AnimationClipListenerEventType_BEGIN = "BEGIN";
static const char* luaEnumString_AnimationClipListenerEventType_END = "END";

AnimationClip::Listener::EventType lua_enumFromString_AnimationClipListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_DEFAULT) == 0)
        return AnimationClip::Listener::DEFAULT;
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_BEGIN) == 0)
        return AnimationClip::Listener::BEGIN;
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_END) == 0)
        return AnimationClip::Listener::END;
    GP_ERROR("Invalid enumeration value '%s' for enumeration AnimationClip::Listener::EventType.", s);
    return AnimationClip::Listener::DEFAULT;
}

const char* lua_stringFromEnum_AnimationClipListenerEventType(AnimationClip::Listener::EventType e)
{
    if (e == AnimationClip::Listener::DEFAULT)
        return luaEnumString_AnimationClipListenerEventType_DEFAULT;
    if (e == AnimationClip::Listener::BEGIN)
        return luaEnumString_AnimationClipListenerEventType_BEGIN;
    if (e == AnimationClip::Listener::END)
        return luaEnumString_AnimationClipListenerEventType_END;
    GP_ERROR("Invalid enumeration value '%d' for enumeration AnimationClip::Listener::EventType.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_AudioSourceState_INITIAL = "INITIAL";
static const char* luaEnumString_AudioSourceState_PLAYING = "PLAYING";
static const char* luaEnumString_AudioSourceState_PAUSED = "PAUSED";
static const char* luaEnumString_AudioSourceState_STOPPED = "STOPPED";

AudioSource::State lua_enumFromString_AudioSourceState(const char* s)
{
    if (strcmp(s, luaEnumString_AudioSourceState_INITIAL) == 0)
        return AudioSource::INITIAL;
    if (strcmp(s, luaEnumString_AudioSourceState_PLAYING) == 0)
        return AudioSource::PLAYING;
    if (strcmp(s, luaEnumString_AudioSourceState_PAUSED) == 0)
        return AudioSource::PAUSED;
    if (strcmp(s, luaEnumString_AudioSourceState_STOPPED) == 0)
        return AudioSource::STOPPED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration AudioSource::State.", s);
    return AudioSource::INITIAL;
}

const char* lua_stringFromEnum_AudioSourceState(AudioSource::State e)
{
    if (e == AudioSource::INITIAL)
        return luaEnumString_AudioSourceState_INITIAL;
    if (e == AudioSource::PLAYING)
        return luaEnumString_AudioSourceState_PLAYING;
    if (e == AudioSource::PAUSED)
        return luaEnumString_AudioSourceState_PAUSED;
    if (e == AudioSource::STOPPED)
        return luaEnumString_AudioSourceState_STOPPED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration AudioSource::State.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_CameraType_PERSPECTIVE = "PERSPECTIVE";
static const char* luaEnumString_CameraType_ORTHOGRAPHIC = "ORTHOGRAPHIC";

Camera::Type lua_enumFromString_CameraType(const char* s)
{
    if (strcmp(s, luaEnumString_CameraType_PERSPECTIVE) == 0)
        return Camera::PERSPECTIVE;
    if (strcmp(s, luaEnumString_CameraType_ORTHOGRAPHIC) == 0)
        return Camera::ORTHOGRAPHIC;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Camera::Type.", s);
    return Camera::PERSPECTIVE;
}

const char* lua_stringFromEnum_CameraType(Camera::Type e)
{
    if (e == Camera::PERSPECTIVE)
        return luaEnumString_CameraType_PERSPECTIVE;
    if (e == Camera::ORTHOGRAPHIC)
        return luaEnumString_CameraType_ORTHOGRAPHIC;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Camera::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_ContainerScroll_SCROLL_NONE = "SCROLL_NONE";
static const char* luaEnumString_ContainerScroll_SCROLL_HORIZONTAL = "SCROLL_HORIZONTAL";
static const char* luaEnumString_ContainerScroll_SCROLL_VERTICAL = "SCROLL_VERTICAL";
static const char* luaEnumString_ContainerScroll_SCROLL_BOTH = "SCROLL_BOTH";

Container::Scroll lua_enumFromString_ContainerScroll(const char* s)
{
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_NONE) == 0)
        return Container::SCROLL_NONE;
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_HORIZONTAL) == 0)
        return Container::SCROLL_HORIZONTAL;
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_VERTICAL) == 0)
        return Container::SCROLL_VERTICAL;
    if (strcmp(s, luaEnumString_ContainerScroll_SCROLL_BOTH) == 0)
        return Container::SCROLL_BOTH;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Container::Scroll.", s);
    return Container::SCROLL_NONE;
}

const char* lua_stringFromEnum_ContainerScroll(Container::Scroll e)
{
    if (e == Container::SCROLL_NONE)
        return luaEnumString_ContainerScroll_SCROLL_NONE;
    if (e == Container::SCROLL_HORIZONTAL)
        return luaEnumString_ContainerScroll_SCROLL_HORIZONTAL;
    if (e == Container::SCROLL_VERTICAL)
        return luaEnumString_ContainerScroll_SCROLL_VERTICAL;
    if (e == Container::SCROLL_BOTH)
        return luaEnumString_ContainerScroll_SCROLL_BOTH;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Container::Scroll.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_ControlAlignment_ALIGN_LEFT = "ALIGN_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_HCENTER = "ALIGN_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_RIGHT = "ALIGN_RIGHT";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP = "ALIGN_TOP";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER = "ALIGN_VCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM = "ALIGN_BOTTOM";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP_LEFT = "ALIGN_TOP_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER_LEFT = "ALIGN_VCENTER_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM_LEFT = "ALIGN_BOTTOM_LEFT";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP_HCENTER = "ALIGN_TOP_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER_HCENTER = "ALIGN_VCENTER_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM_HCENTER = "ALIGN_BOTTOM_HCENTER";
static const char* luaEnumString_ControlAlignment_ALIGN_TOP_RIGHT = "ALIGN_TOP_RIGHT";
static const char* luaEnumString_ControlAlignment_ALIGN_VCENTER_RIGHT = "ALIGN_VCENTER_RIGHT";
static const char* luaEnumString_ControlAlignment_ALIGN_BOTTOM_RIGHT = "ALIGN_BOTTOM_RIGHT";

Control::Alignment lua_enumFromString_ControlAlignment(const char* s)
{
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_LEFT) == 0)
        return Control::ALIGN_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_HCENTER) == 0)
        return Control::ALIGN_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_RIGHT) == 0)
        return Control::ALIGN_RIGHT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP) == 0)
        return Control::ALIGN_TOP;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER) == 0)
        return Control::ALIGN_VCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM) == 0)
        return Control::ALIGN_BOTTOM;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP_LEFT) == 0)
        return Control::ALIGN_TOP_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER_LEFT) == 0)
        return Control::ALIGN_VCENTER_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM_LEFT) == 0)
        return Control::ALIGN_BOTTOM_LEFT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP_HCENTER) == 0)
        return Control::ALIGN_TOP_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER_HCENTER) == 0)
        return Control::ALIGN_VCENTER_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM_HCENTER) == 0)
        return Control::ALIGN_BOTTOM_HCENTER;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_TOP_RIGHT) == 0)
        return Control::ALIGN_TOP_RIGHT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_VCENTER_RIGHT) == 0)
        return Control::ALIGN_VCENTER_RIGHT;
    if (strcmp(s, luaEnumString_ControlAlignment_ALIGN_BOTTOM_RIGHT) == 0)
        return Control::ALIGN_BOTTOM_RIGHT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Control::Alignment.", s);
    return Control::ALIGN_LEFT;
}

const char* lua_stringFromEnum_ControlAlignment(Control::Alignment e)
{
    if (e == Control::ALIGN_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_LEFT;
    if (e == Control::ALIGN_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_HCENTER;
    if (e == Control::ALIGN_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_RIGHT;
    if (e == Control::ALIGN_TOP)
        return luaEnumString_ControlAlignment_ALIGN_TOP;
    if (e == Control::ALIGN_VCENTER)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER;
    if (e == Control::ALIGN_BOTTOM)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM;
    if (e == Control::ALIGN_TOP_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_TOP_LEFT;
    if (e == Control::ALIGN_VCENTER_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER_LEFT;
    if (e == Control::ALIGN_BOTTOM_LEFT)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM_LEFT;
    if (e == Control::ALIGN_TOP_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_TOP_HCENTER;
    if (e == Control::ALIGN_VCENTER_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER_HCENTER;
    if (e == Control::ALIGN_BOTTOM_HCENTER)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM_HCENTER;
    if (e == Control::ALIGN_TOP_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_TOP_RIGHT;
    if (e == Control::ALIGN_VCENTER_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_VCENTER_RIGHT;
    if (e == Control::ALIGN_BOTTOM_RIGHT)
        return luaEnumString_ControlAlignment_ALIGN_BOTTOM_RIGHT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Control::Alignment.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_ControlListenerEventType_PRESS = "PRESS";
static const char* luaEnumString_ControlListenerEventType_RELEASE = "RELEASE";
static const char* luaEnumString_ControlListenerEventType_CLICK = "CLICK";
static const char* luaEnumString_ControlListenerEventType_VALUE_CHANGED = "VALUE_CHANGED";
static const char* luaEnumString_ControlListenerEventType_TEXT_CHANGED = "TEXT_CHANGED";
static const char* luaEnumString_ControlListenerEventType_MIDDLE_CLICK = "MIDDLE_CLICK";
static const char* luaEnumString_ControlListenerEventType_RIGHT_CLICK = "RIGHT_CLICK";

Control::Listener::EventType lua_enumFromString_ControlListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_ControlListenerEventType_PRESS) == 0)
        return Control::Listener::PRESS;
    if (strcmp(s, luaEnumString_ControlListenerEventType_RELEASE) == 0)
        return Control::Listener::RELEASE;
    if (strcmp(s, luaEnumString_ControlListenerEventType_CLICK) == 0)
        return Control::Listener::CLICK;
    if (strcmp(s, luaEnumString_ControlListenerEventType_VALUE_CHANGED) == 0)
        return Control::Listener::VALUE_CHANGED;
    if (strcmp(s, luaEnumString_ControlListenerEventType_TEXT_CHANGED) == 0)
        return Control::Listener::TEXT_CHANGED;
    if (strcmp(s, luaEnumString_ControlListenerEventType_MIDDLE_CLICK) == 0)
        return Control::Listener::MIDDLE_CLICK;
    if (strcmp(s, luaEnumString_ControlListenerEventType_RIGHT_CLICK) == 0)
        return Control::Listener::RIGHT_CLICK;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Control::Listener::EventType.", s);
    return Control::Listener::PRESS;
}

const char* lua_stringFromEnum_ControlListenerEventType(Control::Listener::EventType e)
{
    if (e == Control::Listener::PRESS)
        return luaEnumString_ControlListenerEventType_PRESS;
    if (e == Control::Listener::RELEASE)
        return luaEnumString_ControlListenerEventType_RELEASE;
    if (e == Control::Listener::CLICK)
        return luaEnumString_ControlListenerEventType_CLICK;
    if (e == Control::Listener::VALUE_CHANGED)
        return luaEnumString_ControlListenerEventType_VALUE_CHANGED;
    if (e == Control::Listener::TEXT_CHANGED)
        return luaEnumString_ControlListenerEventType_TEXT_CHANGED;
    if (e == Control::Listener::MIDDLE_CLICK)
        return luaEnumString_ControlListenerEventType_MIDDLE_CLICK;
    if (e == Control::Listener::RIGHT_CLICK)
        return luaEnumString_ControlListenerEventType_RIGHT_CLICK;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Control::Listener::EventType.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_ControlState_NORMAL = "NORMAL";
static const char* luaEnumString_ControlState_FOCUS = "FOCUS";
static const char* luaEnumString_ControlState_ACTIVE = "ACTIVE";
static const char* luaEnumString_ControlState_DISABLED = "DISABLED";

Control::State lua_enumFromString_ControlState(const char* s)
{
    if (strcmp(s, luaEnumString_ControlState_NORMAL) == 0)
        return Control::NORMAL;
    if (strcmp(s, luaEnumString_ControlState_FOCUS) == 0)
        return Control::FOCUS;
    if (strcmp(s, luaEnumString_ControlState_ACTIVE) == 0)
        return Control::ACTIVE;
    if (strcmp(s, luaEnumString_ControlState_DISABLED) == 0)
        return Control::DISABLED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Control::State.", s);
    return Control::NORMAL;
}

const char* lua_stringFromEnum_ControlState(Control::State e)
{
    if (e == Control::NORMAL)
        return luaEnumString_ControlState_NORMAL;
    if (e == Control::FOCUS)
        return luaEnumString_ControlState_FOCUS;
    if (e == Control::ACTIVE)
        return luaEnumString_ControlState_ACTIVE;
    if (e == Control::DISABLED)
        return luaEnumString_ControlState_DISABLED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Control::State.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_CurveInterpolationType_BEZIER = "BEZIER";
static const char* luaEnumString_CurveInterpolationType_BSPLINE = "BSPLINE";
static const char* luaEnumString_CurveInterpolationType_FLAT = "FLAT";
static const char* luaEnumString_CurveInterpolationType_HERMITE = "HERMITE";
static const char* luaEnumString_CurveInterpolationType_LINEAR = "LINEAR";
static const char* luaEnumString_CurveInterpolationType_SMOOTH = "SMOOTH";
static const char* luaEnumString_CurveInterpolationType_STEP = "STEP";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_IN = "QUADRATIC_IN";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_OUT = "QUADRATIC_OUT";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_IN_OUT = "QUADRATIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_QUADRATIC_OUT_IN = "QUADRATIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_CUBIC_IN = "CUBIC_IN";
static const char* luaEnumString_CurveInterpolationType_CUBIC_OUT = "CUBIC_OUT";
static const char* luaEnumString_CurveInterpolationType_CUBIC_IN_OUT = "CUBIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_CUBIC_OUT_IN = "CUBIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_IN = "QUARTIC_IN";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_OUT = "QUARTIC_OUT";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_IN_OUT = "QUARTIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_QUARTIC_OUT_IN = "QUARTIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_IN = "QUINTIC_IN";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_OUT = "QUINTIC_OUT";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_IN_OUT = "QUINTIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_QUINTIC_OUT_IN = "QUINTIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_SINE_IN = "SINE_IN";
static const char* luaEnumString_CurveInterpolationType_SINE_OUT = "SINE_OUT";
static const char* luaEnumString_CurveInterpolationType_SINE_IN_OUT = "SINE_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_SINE_OUT_IN = "SINE_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_IN = "EXPONENTIAL_IN";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT = "EXPONENTIAL_OUT";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_IN_OUT = "EXPONENTIAL_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT_IN = "EXPONENTIAL_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_IN = "CIRCULAR_IN";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_OUT = "CIRCULAR_OUT";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_IN_OUT = "CIRCULAR_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_CIRCULAR_OUT_IN = "CIRCULAR_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_IN = "ELASTIC_IN";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_OUT = "ELASTIC_OUT";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_IN_OUT = "ELASTIC_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_ELASTIC_OUT_IN = "ELASTIC_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_IN = "OVERSHOOT_IN";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_OUT = "OVERSHOOT_OUT";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_IN_OUT = "OVERSHOOT_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_OVERSHOOT_OUT_IN = "OVERSHOOT_OUT_IN";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_IN = "BOUNCE_IN";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_OUT = "BOUNCE_OUT";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_IN_OUT = "BOUNCE_IN_OUT";
static const char* luaEnumString_CurveInterpolationType_BOUNCE_OUT_IN = "BOUNCE_OUT_IN";

Curve::InterpolationType lua_enumFromString_CurveInterpolationType(const char* s)
{
    if (strcmp(s, luaEnumString_CurveInterpolationType_BEZIER) == 0)
        return Curve::BEZIER;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BSPLINE) == 0)
        return Curve::BSPLINE;
    if (strcmp(s, luaEnumString_CurveInterpolationType_FLAT) == 0)
        return Curve::FLAT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_HERMITE) == 0)
        return Curve::HERMITE;
    if (strcmp(s, luaEnumString_CurveInterpolationType_LINEAR) == 0)
        return Curve::LINEAR;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SMOOTH) == 0)
        return Curve::SMOOTH;
    if (strcmp(s, luaEnumString_CurveInterpolationType_STEP) == 0)
        return Curve::STEP;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_IN) == 0)
        return Curve::QUADRATIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_OUT) == 0)
        return Curve::QUADRATIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_IN_OUT) == 0)
        return Curve::QUADRATIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUADRATIC_OUT_IN) == 0)
        return Curve::QUADRATIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_IN) == 0)
        return Curve::CUBIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_OUT) == 0)
        return Curve::CUBIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_IN_OUT) == 0)
        return Curve::CUBIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CUBIC_OUT_IN) == 0)
        return Curve::CUBIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_IN) == 0)
        return Curve::QUARTIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_OUT) == 0)
        return Curve::QUARTIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_IN_OUT) == 0)
        return Curve::QUARTIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUARTIC_OUT_IN) == 0)
        return Curve::QUARTIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_IN) == 0)
        return Curve::QUINTIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_OUT) == 0)
        return Curve::QUINTIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_IN_OUT) == 0)
        return Curve::QUINTIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_QUINTIC_OUT_IN) == 0)
        return Curve::QUINTIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_IN) == 0)
        return Curve::SINE_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_OUT) == 0)
        return Curve::SINE_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_IN_OUT) == 0)
        return Curve::SINE_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_SINE_OUT_IN) == 0)
        return Curve::SINE_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_IN) == 0)
        return Curve::EXPONENTIAL_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT) == 0)
        return Curve::EXPONENTIAL_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_IN_OUT) == 0)
        return Curve::EXPONENTIAL_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT_IN) == 0)
        return Curve::EXPONENTIAL_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_IN) == 0)
        return Curve::CIRCULAR_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_OUT) == 0)
        return Curve::CIRCULAR_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_IN_OUT) == 0)
        return Curve::CIRCULAR_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_CIRCULAR_OUT_IN) == 0)
        return Curve::CIRCULAR_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_IN) == 0)
        return Curve::ELASTIC_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_OUT) == 0)
        return Curve::ELASTIC_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_IN_OUT) == 0)
        return Curve::ELASTIC_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_ELASTIC_OUT_IN) == 0)
        return Curve::ELASTIC_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_IN) == 0)
        return Curve::OVERSHOOT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_OUT) == 0)
        return Curve::OVERSHOOT_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_IN_OUT) == 0)
        return Curve::OVERSHOOT_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_OVERSHOOT_OUT_IN) == 0)
        return Curve::OVERSHOOT_OUT_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_IN) == 0)
        return Curve::BOUNCE_IN;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_OUT) == 0)
        return Curve::BOUNCE_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_IN_OUT) == 0)
        return Curve::BOUNCE_IN_OUT;
    if (strcmp(s, luaEnumString_CurveInterpolationType_BOUNCE_OUT_IN) == 0)
        return Curve::BOUNCE_OUT_IN;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Curve::InterpolationType.", s);
    return Curve::BEZIER;
}

const char* lua_stringFromEnum_CurveInterpolationType(Curve::InterpolationType e)
{
    if (e == Curve::BEZIER)
        return luaEnumString_CurveInterpolationType_BEZIER;
    if (e == Curve::BSPLINE)
        return luaEnumString_CurveInterpolationType_BSPLINE;
    if (e == Curve::FLAT)
        return luaEnumString_CurveInterpolationType_FLAT;
    if (e == Curve::HERMITE)
        return luaEnumString_CurveInterpolationType_HERMITE;
    if (e == Curve::LINEAR)
        return luaEnumString_CurveInterpolationType_LINEAR;
    if (e == Curve::SMOOTH)
        return luaEnumString_CurveInterpolationType_SMOOTH;
    if (e == Curve::STEP)
        return luaEnumString_CurveInterpolationType_STEP;
    if (e == Curve::QUADRATIC_IN)
        return luaEnumString_CurveInterpolationType_QUADRATIC_IN;
    if (e == Curve::QUADRATIC_OUT)
        return luaEnumString_CurveInterpolationType_QUADRATIC_OUT;
    if (e == Curve::QUADRATIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_QUADRATIC_IN_OUT;
    if (e == Curve::QUADRATIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_QUADRATIC_OUT_IN;
    if (e == Curve::CUBIC_IN)
        return luaEnumString_CurveInterpolationType_CUBIC_IN;
    if (e == Curve::CUBIC_OUT)
        return luaEnumString_CurveInterpolationType_CUBIC_OUT;
    if (e == Curve::CUBIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_CUBIC_IN_OUT;
    if (e == Curve::CUBIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_CUBIC_OUT_IN;
    if (e == Curve::QUARTIC_IN)
        return luaEnumString_CurveInterpolationType_QUARTIC_IN;
    if (e == Curve::QUARTIC_OUT)
        return luaEnumString_CurveInterpolationType_QUARTIC_OUT;
    if (e == Curve::QUARTIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_QUARTIC_IN_OUT;
    if (e == Curve::QUARTIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_QUARTIC_OUT_IN;
    if (e == Curve::QUINTIC_IN)
        return luaEnumString_CurveInterpolationType_QUINTIC_IN;
    if (e == Curve::QUINTIC_OUT)
        return luaEnumString_CurveInterpolationType_QUINTIC_OUT;
    if (e == Curve::QUINTIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_QUINTIC_IN_OUT;
    if (e == Curve::QUINTIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_QUINTIC_OUT_IN;
    if (e == Curve::SINE_IN)
        return luaEnumString_CurveInterpolationType_SINE_IN;
    if (e == Curve::SINE_OUT)
        return luaEnumString_CurveInterpolationType_SINE_OUT;
    if (e == Curve::SINE_IN_OUT)
        return luaEnumString_CurveInterpolationType_SINE_IN_OUT;
    if (e == Curve::SINE_OUT_IN)
        return luaEnumString_CurveInterpolationType_SINE_OUT_IN;
    if (e == Curve::EXPONENTIAL_IN)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_IN;
    if (e == Curve::EXPONENTIAL_OUT)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT;
    if (e == Curve::EXPONENTIAL_IN_OUT)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_IN_OUT;
    if (e == Curve::EXPONENTIAL_OUT_IN)
        return luaEnumString_CurveInterpolationType_EXPONENTIAL_OUT_IN;
    if (e == Curve::CIRCULAR_IN)
        return luaEnumString_CurveInterpolationType_CIRCULAR_IN;
    if (e == Curve::CIRCULAR_OUT)
        return luaEnumString_CurveInterpolationType_CIRCULAR_OUT;
    if (e == Curve::CIRCULAR_IN_OUT)
        return luaEnumString_CurveInterpolationType_CIRCULAR_IN_OUT;
    if (e == Curve::CIRCULAR_OUT_IN)
        return luaEnumString_CurveInterpolationType_CIRCULAR_OUT_IN;
    if (e == Curve::ELASTIC_IN)
        return luaEnumString_CurveInterpolationType_ELASTIC_IN;
    if (e == Curve::ELASTIC_OUT)
        return luaEnumString_CurveInterpolationType_ELASTIC_OUT;
    if (e == Curve::ELASTIC_IN_OUT)
        return luaEnumString_CurveInterpolationType_ELASTIC_IN_OUT;
    if (e == Curve::ELASTIC_OUT_IN)
        return luaEnumString_CurveInterpolationType_ELASTIC_OUT_IN;
    if (e == Curve::OVERSHOOT_IN)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_IN;
    if (e == Curve::OVERSHOOT_OUT)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_OUT;
    if (e == Curve::OVERSHOOT_IN_OUT)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_IN_OUT;
    if (e == Curve::OVERSHOOT_OUT_IN)
        return luaEnumString_CurveInterpolationType_OVERSHOOT_OUT_IN;
    if (e == Curve::BOUNCE_IN)
        return luaEnumString_CurveInterpolationType_BOUNCE_IN;
    if (e == Curve::BOUNCE_OUT)
        return luaEnumString_CurveInterpolationType_BOUNCE_OUT;
    if (e == Curve::BOUNCE_IN_OUT)
        return luaEnumString_CurveInterpolationType_BOUNCE_IN_OUT;
    if (e == Curve::BOUNCE_OUT_IN)
        return luaEnumString_CurveInterpolationType_BOUNCE_OUT_IN;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Curve::InterpolationType.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_DepthStencilTargetFormat_DEPTH = "DEPTH";
static const char* luaEnumString_DepthStencilTargetFormat_DEPTH_STENCIL = "DEPTH_STENCIL";

DepthStencilTarget::Format lua_enumFromString_DepthStencilTargetFormat(const char* s)
{
    if (strcmp(s, luaEnumString_DepthStencilTargetFormat_DEPTH) == 0)
        return DepthStencilTarget::DEPTH;
    if (strcmp(s, luaEnumString_DepthStencilTargetFormat_DEPTH_STENCIL) == 0)
        return DepthStencilTarget::DEPTH_STENCIL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration DepthStencilTarget::Format.", s);
    return DepthStencilTarget::DEPTH;
}

const char* lua_stringFromEnum_DepthStencilTargetFormat(DepthStencilTarget::Format e)
{
    if (e == DepthStencilTarget::DEPTH)
        return luaEnumString_DepthStencilTargetFormat_DEPTH;
    if (e == DepthStencilTarget::DEPTH_STENCIL)
        return luaEnumString_DepthStencilTargetFormat_DEPTH_STENCIL;
    GP_ERROR("Invalid enumeration value '%d' for enumeration DepthStencilTarget::Format.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_FontJustify_ALIGN_LEFT = "ALIGN_LEFT";
static const char* luaEnumString_FontJustify_ALIGN_HCENTER = "ALIGN_HCENTER";
static const char* luaEnumString_FontJustify_ALIGN_RIGHT = "ALIGN_RIGHT";
static const char* luaEnumString_FontJustify_ALIGN_TOP = "ALIGN_TOP";
static const char* luaEnumString_FontJustify_ALIGN_VCENTER = "ALIGN_VCENTER";
static const char* luaEnumString_FontJustify_ALIGN_BOTTOM = "ALIGN_BOTTOM";
static const char* luaEnumString_FontJustify_ALIGN_TOP_LEFT = "ALIGN_TOP_LEFT";
static const char* luaEnumString_FontJustify_ALIGN_VCENTER_LEFT = "ALIGN_VCENTER_LEFT";
static const char* luaEnumString_FontJustify_ALIGN_BOTTOM_LEFT = "ALIGN_BOTTOM_LEFT";
static const char* luaEnumString_FontJustify_ALIGN_TOP_HCENTER = "ALIGN_TOP_HCENTER";
static const char* luaEnumString_FontJustify_ALIGN_VCENTER_HCENTER = "ALIGN_VCENTER_HCENTER";
static const char* luaEnumString_FontJustify_ALIGN_BOTTOM_HCENTER = "ALIGN_BOTTOM_HCENTER";
static const char* luaEnumString_FontJustify_ALIGN_TOP_RIGHT = "ALIGN_TOP_RIGHT";
static const char* luaEnumString_FontJustify_ALIGN_VCENTER_RIGHT = "ALIGN_VCENTER_RIGHT";
static const char* luaEnumString_FontJustify_ALIGN_BOTTOM_RIGHT = "ALIGN_BOTTOM_RIGHT";

Font::Justify lua_enumFromString_FontJustify(const char* s)
{
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_LEFT) == 0)
        return Font::ALIGN_LEFT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_HCENTER) == 0)
        return Font::ALIGN_HCENTER;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_RIGHT) == 0)
        return Font::ALIGN_RIGHT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_TOP) == 0)
        return Font::ALIGN_TOP;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_VCENTER) == 0)
        return Font::ALIGN_VCENTER;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_BOTTOM) == 0)
        return Font::ALIGN_BOTTOM;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_TOP_LEFT) == 0)
        return Font::ALIGN_TOP_LEFT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_VCENTER_LEFT) == 0)
        return Font::ALIGN_VCENTER_LEFT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_BOTTOM_LEFT) == 0)
        return Font::ALIGN_BOTTOM_LEFT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_TOP_HCENTER) == 0)
        return Font::ALIGN_TOP_HCENTER;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_VCENTER_HCENTER) == 0)
        return Font::ALIGN_VCENTER_HCENTER;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_BOTTOM_HCENTER) == 0)
        return Font::ALIGN_BOTTOM_HCENTER;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_TOP_RIGHT) == 0)
        return Font::ALIGN_TOP_RIGHT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_VCENTER_RIGHT) == 0)
        return Font::ALIGN_VCENTER_RIGHT;
    if (strcmp(s, luaEnumString_FontJustify_ALIGN_BOTTOM_RIGHT) == 0)
        return Font::ALIGN_BOTTOM_RIGHT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Font::Justify.", s);
    return Font::ALIGN_LEFT;
}

const char* lua_stringFromEnum_FontJustify(Font::Justify e)
{
    if (e == Font::ALIGN_LEFT)
        return luaEnumString_FontJustify_ALIGN_LEFT;
    if (e == Font::ALIGN_HCENTER)
        return luaEnumString_FontJustify_ALIGN_HCENTER;
    if (e == Font::ALIGN_RIGHT)
        return luaEnumString_FontJustify_ALIGN_RIGHT;
    if (e == Font::ALIGN_TOP)
        return luaEnumString_FontJustify_ALIGN_TOP;
    if (e == Font::ALIGN_VCENTER)
        return luaEnumString_FontJustify_ALIGN_VCENTER;
    if (e == Font::ALIGN_BOTTOM)
        return luaEnumString_FontJustify_ALIGN_BOTTOM;
    if (e == Font::ALIGN_TOP_LEFT)
        return luaEnumString_FontJustify_ALIGN_TOP_LEFT;
    if (e == Font::ALIGN_VCENTER_LEFT)
        return luaEnumString_FontJustify_ALIGN_VCENTER_LEFT;
    if (e == Font::ALIGN_BOTTOM_LEFT)
        return luaEnumString_FontJustify_ALIGN_BOTTOM_LEFT;
    if (e == Font::ALIGN_TOP_HCENTER)
        return luaEnumString_FontJustify_ALIGN_TOP_HCENTER;
    if (e == Font::ALIGN_VCENTER_HCENTER)
        return luaEnumString_FontJustify_ALIGN_VCENTER_HCENTER;
    if (e == Font::ALIGN_BOTTOM_HCENTER)
        return luaEnumString_FontJustify_ALIGN_BOTTOM_HCENTER;
    if (e == Font::ALIGN_TOP_RIGHT)
        return luaEnumString_FontJustify_ALIGN_TOP_RIGHT;
    if (e == Font::ALIGN_VCENTER_RIGHT)
        return luaEnumString_FontJustify_ALIGN_VCENTER_RIGHT;
    if (e == Font::ALIGN_BOTTOM_RIGHT)
        return luaEnumString_FontJustify_ALIGN_BOTTOM_RIGHT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Font::Justify.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_FontStyle_PLAIN = "PLAIN";
static const char* luaEnumString_FontStyle_BOLD = "BOLD";
static const char* luaEnumString_FontStyle_ITALIC = "ITALIC";
static const char* luaEnumString_FontStyle_BOLD_ITALIC = "BOLD_ITALIC";

Font::Style lua_enumFromString_FontStyle(const char* s)
{
    if (strcmp(s, luaEnumString_FontStyle_PLAIN) == 0)
        return Font::PLAIN;
    if (strcmp(s, luaEnumString_FontStyle_BOLD) == 0)
        return Font::BOLD;
    if (strcmp(s, luaEnumString_FontStyle_ITALIC) == 0)
        return Font::ITALIC;
    if (strcmp(s, luaEnumString_FontStyle_BOLD_ITALIC) == 0)
        return Font::BOLD_ITALIC;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Font::Style.", s);
    return Font::PLAIN;
}

const char* lua_stringFromEnum_FontStyle(Font::Style e)
{
    if (e == Font::PLAIN)
        return luaEnumString_FontStyle_PLAIN;
    if (e == Font::BOLD)
        return luaEnumString_FontStyle_BOLD;
    if (e == Font::ITALIC)
        return luaEnumString_FontStyle_ITALIC;
    if (e == Font::BOLD_ITALIC)
        return luaEnumString_FontStyle_BOLD_ITALIC;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Font::Style.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_GameClearFlags_CLEAR_COLOR = "CLEAR_COLOR";
static const char* luaEnumString_GameClearFlags_CLEAR_DEPTH = "CLEAR_DEPTH";
static const char* luaEnumString_GameClearFlags_CLEAR_STENCIL = "CLEAR_STENCIL";
static const char* luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH = "CLEAR_COLOR_DEPTH";
static const char* luaEnumString_GameClearFlags_CLEAR_COLOR_STENCIL = "CLEAR_COLOR_STENCIL";
static const char* luaEnumString_GameClearFlags_CLEAR_DEPTH_STENCIL = "CLEAR_DEPTH_STENCIL";
static const char* luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH_STENCIL = "CLEAR_COLOR_DEPTH_STENCIL";

Game::ClearFlags lua_enumFromString_GameClearFlags(const char* s)
{
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR) == 0)
        return Game::CLEAR_COLOR;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_DEPTH) == 0)
        return Game::CLEAR_DEPTH;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_STENCIL) == 0)
        return Game::CLEAR_STENCIL;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH) == 0)
        return Game::CLEAR_COLOR_DEPTH;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR_STENCIL) == 0)
        return Game::CLEAR_COLOR_STENCIL;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_DEPTH_STENCIL) == 0)
        return Game::CLEAR_DEPTH_STENCIL;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH_STENCIL) == 0)
        return Game::CLEAR_COLOR_DEPTH_STENCIL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Game::ClearFlags.", s);
    return Game::CLEAR_COLOR;
}

const char* lua_stringFromEnum_GameClearFlags(Game::ClearFlags e)
{
    if (e == Game::CLEAR_COLOR)
        return luaEnumString_GameClearFlags_CLEAR_COLOR;
    if (e == Game::CLEAR_DEPTH)
        return luaEnumString_GameClearFlags_CLEAR_DEPTH;
    if (e == Game::CLEAR_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_STENCIL;
    if (e == Game::CLEAR_COLOR_DEPTH)
        return luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH;
    if (e == Game::CLEAR_COLOR_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_COLOR_STENCIL;
    if (e == Game::CLEAR_DEPTH_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_DEPTH_STENCIL;
    if (e == Game::CLEAR_COLOR_DEPTH_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH_STENCIL;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Game::ClearFlags.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_GameState_UNINITIALIZED = "UNINITIALIZED";
static const char* luaEnumString_GameState_RUNNING = "RUNNING";
static const char* luaEnumString_GameState_PAUSED = "PAUSED";

Game::State lua_enumFromString_GameState(const char* s)
{
    if (strcmp(s, luaEnumString_GameState_UNINITIALIZED) == 0)
        return Game::UNINITIALIZED;
    if (strcmp(s, luaEnumString_GameState_RUNNING) == 0)
        return Game::RUNNING;
    if (strcmp(s, luaEnumString_GameState_PAUSED) == 0)
        return Game::PAUSED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Game::State.", s);
    return Game::UNINITIALIZED;
}

const char* lua_stringFromEnum_GameState(Game::State e)
{
    if (e == Game::UNINITIALIZED)
        return luaEnumString_GameState_UNINITIALIZED;
    if (e == Game::RUNNING)
        return luaEnumString_GameState_RUNNING;
    if (e == Game::PAUSED)
        return luaEnumString_GameState_PAUSED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Game::State.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_GamepadButtonState_BUTTON_PRESSED = "BUTTON_PRESSED";
static const char* luaEnumString_GamepadButtonState_BUTTON_RELEASED = "BUTTON_RELEASED";

Gamepad::ButtonState lua_enumFromString_GamepadButtonState(const char* s)
{
    if (strcmp(s, luaEnumString_GamepadButtonState_BUTTON_PRESSED) == 0)
        return Gamepad::BUTTON_PRESSED;
    if (strcmp(s, luaEnumString_GamepadButtonState_BUTTON_RELEASED) == 0)
        return Gamepad::BUTTON_RELEASED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gamepad::ButtonState.", s);
    return Gamepad::BUTTON_PRESSED;
}

const char* lua_stringFromEnum_GamepadButtonState(Gamepad::ButtonState e)
{
    if (e == Gamepad::BUTTON_PRESSED)
        return luaEnumString_GamepadButtonState_BUTTON_PRESSED;
    if (e == Gamepad::BUTTON_RELEASED)
        return luaEnumString_GamepadButtonState_BUTTON_RELEASED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gamepad::ButtonState.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_GamepadGamepadEvent_ATTACHED_EVENT = "ATTACHED_EVENT";
static const char* luaEnumString_GamepadGamepadEvent_DETACHED_EVENT = "DETACHED_EVENT";

Gamepad::GamepadEvent lua_enumFromString_GamepadGamepadEvent(const char* s)
{
    if (strcmp(s, luaEnumString_GamepadGamepadEvent_ATTACHED_EVENT) == 0)
        return Gamepad::ATTACHED_EVENT;
    if (strcmp(s, luaEnumString_GamepadGamepadEvent_DETACHED_EVENT) == 0)
        return Gamepad::DETACHED_EVENT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Gamepad::GamepadEvent.", s);
    return Gamepad::ATTACHED_EVENT;
}

const char* lua_stringFromEnum_GamepadGamepadEvent(Gamepad::GamepadEvent e)
{
    if (e == Gamepad::ATTACHED_EVENT)
        return luaEnumString_GamepadGamepadEvent_ATTACHED_EVENT;
    if (e == Gamepad::DETACHED_EVENT)
        return luaEnumString_GamepadGamepadEvent_DETACHED_EVENT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Gamepad::GamepadEvent.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_ImageFormat_RGB = "RGB";
static const char* luaEnumString_ImageFormat_RGBA = "RGBA";

Image::Format lua_enumFromString_ImageFormat(const char* s)
{
    if (strcmp(s, luaEnumString_ImageFormat_RGB) == 0)
        return Image::RGB;
    if (strcmp(s, luaEnumString_ImageFormat_RGBA) == 0)
        return Image::RGBA;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Image::Format.", s);
    return Image::RGB;
}

const char* lua_stringFromEnum_ImageFormat(Image::Format e)
{
    if (e == Image::RGB)
        return luaEnumString_ImageFormat_RGB;
    if (e == Image::RGBA)
        return luaEnumString_ImageFormat_RGBA;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Image::Format.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_KeyboardKey_KEY_NONE = "KEY_NONE";
static const char* luaEnumString_KeyboardKey_KEY_PAUSE = "KEY_PAUSE";
static const char* luaEnumString_KeyboardKey_KEY_SCROLL_LOCK = "KEY_SCROLL_LOCK";
static const char* luaEnumString_KeyboardKey_KEY_PRINT = "KEY_PRINT";
static const char* luaEnumString_KeyboardKey_KEY_SYSREQ = "KEY_SYSREQ";
static const char* luaEnumString_KeyboardKey_KEY_BREAK = "KEY_BREAK";
static const char* luaEnumString_KeyboardKey_KEY_ESCAPE = "KEY_ESCAPE";
static const char* luaEnumString_KeyboardKey_KEY_BACKSPACE = "KEY_BACKSPACE";
static const char* luaEnumString_KeyboardKey_KEY_TAB = "KEY_TAB";
static const char* luaEnumString_KeyboardKey_KEY_BACK_TAB = "KEY_BACK_TAB";
static const char* luaEnumString_KeyboardKey_KEY_RETURN = "KEY_RETURN";
static const char* luaEnumString_KeyboardKey_KEY_CAPS_LOCK = "KEY_CAPS_LOCK";
static const char* luaEnumString_KeyboardKey_KEY_SHIFT = "KEY_SHIFT";
static const char* luaEnumString_KeyboardKey_KEY_CTRL = "KEY_CTRL";
static const char* luaEnumString_KeyboardKey_KEY_ALT = "KEY_ALT";
static const char* luaEnumString_KeyboardKey_KEY_MENU = "KEY_MENU";
static const char* luaEnumString_KeyboardKey_KEY_HYPER = "KEY_HYPER";
static const char* luaEnumString_KeyboardKey_KEY_INSERT = "KEY_INSERT";
static const char* luaEnumString_KeyboardKey_KEY_HOME = "KEY_HOME";
static const char* luaEnumString_KeyboardKey_KEY_PG_UP = "KEY_PG_UP";
static const char* luaEnumString_KeyboardKey_KEY_DELETE = "KEY_DELETE";
static const char* luaEnumString_KeyboardKey_KEY_END = "KEY_END";
static const char* luaEnumString_KeyboardKey_KEY_PG_DOWN = "KEY_PG_DOWN";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_ARROW = "KEY_LEFT_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_ARROW = "KEY_RIGHT_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_UP_ARROW = "KEY_UP_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_DOWN_ARROW = "KEY_DOWN_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_NUM_LOCK = "KEY_NUM_LOCK";
static const char* luaEnumString_KeyboardKey_KEY_KP_PLUS = "KEY_KP_PLUS";
static const char* luaEnumString_KeyboardKey_KEY_KP_MINUS = "KEY_KP_MINUS";
static const char* luaEnumString_KeyboardKey_KEY_KP_MULTIPLY = "KEY_KP_MULTIPLY";
static const char* luaEnumString_KeyboardKey_KEY_KP_DIVIDE = "KEY_KP_DIVIDE";
static const char* luaEnumString_KeyboardKey_KEY_KP_ENTER = "KEY_KP_ENTER";
static const char* luaEnumString_KeyboardKey_KEY_KP_HOME = "KEY_KP_HOME";
static const char* luaEnumString_KeyboardKey_KEY_KP_UP = "KEY_KP_UP";
static const char* luaEnumString_KeyboardKey_KEY_KP_PG_UP = "KEY_KP_PG_UP";
static const char* luaEnumString_KeyboardKey_KEY_KP_LEFT = "KEY_KP_LEFT";
static const char* luaEnumString_KeyboardKey_KEY_KP_FIVE = "KEY_KP_FIVE";
static const char* luaEnumString_KeyboardKey_KEY_KP_RIGHT = "KEY_KP_RIGHT";
static const char* luaEnumString_KeyboardKey_KEY_KP_END = "KEY_KP_END";
static const char* luaEnumString_KeyboardKey_KEY_KP_DOWN = "KEY_KP_DOWN";
static const char* luaEnumString_KeyboardKey_KEY_KP_PG_DOWN = "KEY_KP_PG_DOWN";
static const char* luaEnumString_KeyboardKey_KEY_KP_INSERT = "KEY_KP_INSERT";
static const char* luaEnumString_KeyboardKey_KEY_KP_DELETE = "KEY_KP_DELETE";
static const char* luaEnumString_KeyboardKey_KEY_F1 = "KEY_F1";
static const char* luaEnumString_KeyboardKey_KEY_F2 = "KEY_F2";
static const char* luaEnumString_KeyboardKey_KEY_F3 = "KEY_F3";
static const char* luaEnumString_KeyboardKey_KEY_F4 = "KEY_F4";
static const char* luaEnumString_KeyboardKey_KEY_F5 = "KEY_F5";
static const char* luaEnumString_KeyboardKey_KEY_F6 = "KEY_F6";
static const char* luaEnumString_KeyboardKey_KEY_F7 = "KEY_F7";
static const char* luaEnumString_KeyboardKey_KEY_F8 = "KEY_F8";
static const char* luaEnumString_KeyboardKey_KEY_F9 = "KEY_F9";
static const char* luaEnumString_KeyboardKey_KEY_F10 = "KEY_F10";
static const char* luaEnumString_KeyboardKey_KEY_F11 = "KEY_F11";
static const char* luaEnumString_KeyboardKey_KEY_F12 = "KEY_F12";
static const char* luaEnumString_KeyboardKey_KEY_SPACE = "KEY_SPACE";
static const char* luaEnumString_KeyboardKey_KEY_EXCLAM = "KEY_EXCLAM";
static const char* luaEnumString_KeyboardKey_KEY_QUOTE = "KEY_QUOTE";
static const char* luaEnumString_KeyboardKey_KEY_NUMBER = "KEY_NUMBER";
static const char* luaEnumString_KeyboardKey_KEY_DOLLAR = "KEY_DOLLAR";
static const char* luaEnumString_KeyboardKey_KEY_PERCENT = "KEY_PERCENT";
static const char* luaEnumString_KeyboardKey_KEY_CIRCUMFLEX = "KEY_CIRCUMFLEX";
static const char* luaEnumString_KeyboardKey_KEY_AMPERSAND = "KEY_AMPERSAND";
static const char* luaEnumString_KeyboardKey_KEY_APOSTROPHE = "KEY_APOSTROPHE";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_PARENTHESIS = "KEY_LEFT_PARENTHESIS";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_PARENTHESIS = "KEY_RIGHT_PARENTHESIS";
static const char* luaEnumString_KeyboardKey_KEY_ASTERISK = "KEY_ASTERISK";
static const char* luaEnumString_KeyboardKey_KEY_PLUS = "KEY_PLUS";
static const char* luaEnumString_KeyboardKey_KEY_COMMA = "KEY_COMMA";
static const char* luaEnumString_KeyboardKey_KEY_MINUS = "KEY_MINUS";
static const char* luaEnumString_KeyboardKey_KEY_PERIOD = "KEY_PERIOD";
static const char* luaEnumString_KeyboardKey_KEY_SLASH = "KEY_SLASH";
static const char* luaEnumString_KeyboardKey_KEY_ZERO = "KEY_ZERO";
static const char* luaEnumString_KeyboardKey_KEY_ONE = "KEY_ONE";
static const char* luaEnumString_KeyboardKey_KEY_TWO = "KEY_TWO";
static const char* luaEnumString_KeyboardKey_KEY_THREE = "KEY_THREE";
static const char* luaEnumString_KeyboardKey_KEY_FOUR = "KEY_FOUR";
static const char* luaEnumString_KeyboardKey_KEY_FIVE = "KEY_FIVE";
static const char* luaEnumString_KeyboardKey_KEY_SIX = "KEY_SIX";
static const char* luaEnumString_KeyboardKey_KEY_SEVEN = "KEY_SEVEN";
static const char* luaEnumString_KeyboardKey_KEY_EIGHT = "KEY_EIGHT";
static const char* luaEnumString_KeyboardKey_KEY_NINE = "KEY_NINE";
static const char* luaEnumString_KeyboardKey_KEY_COLON = "KEY_COLON";
static const char* luaEnumString_KeyboardKey_KEY_SEMICOLON = "KEY_SEMICOLON";
static const char* luaEnumString_KeyboardKey_KEY_LESS_THAN = "KEY_LESS_THAN";
static const char* luaEnumString_KeyboardKey_KEY_EQUAL = "KEY_EQUAL";
static const char* luaEnumString_KeyboardKey_KEY_GREATER_THAN = "KEY_GREATER_THAN";
static const char* luaEnumString_KeyboardKey_KEY_QUESTION = "KEY_QUESTION";
static const char* luaEnumString_KeyboardKey_KEY_AT = "KEY_AT";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_A = "KEY_CAPITAL_A";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_B = "KEY_CAPITAL_B";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_C = "KEY_CAPITAL_C";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_D = "KEY_CAPITAL_D";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_E = "KEY_CAPITAL_E";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_F = "KEY_CAPITAL_F";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_G = "KEY_CAPITAL_G";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_H = "KEY_CAPITAL_H";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_I = "KEY_CAPITAL_I";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_J = "KEY_CAPITAL_J";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_K = "KEY_CAPITAL_K";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_L = "KEY_CAPITAL_L";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_M = "KEY_CAPITAL_M";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_N = "KEY_CAPITAL_N";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_O = "KEY_CAPITAL_O";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_P = "KEY_CAPITAL_P";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_Q = "KEY_CAPITAL_Q";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_R = "KEY_CAPITAL_R";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_S = "KEY_CAPITAL_S";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_T = "KEY_CAPITAL_T";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_U = "KEY_CAPITAL_U";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_V = "KEY_CAPITAL_V";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_W = "KEY_CAPITAL_W";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_X = "KEY_CAPITAL_X";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_Y = "KEY_CAPITAL_Y";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_Z = "KEY_CAPITAL_Z";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_BRACKET = "KEY_LEFT_BRACKET";
static const char* luaEnumString_KeyboardKey_KEY_BACK_SLASH = "KEY_BACK_SLASH";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_BRACKET = "KEY_RIGHT_BRACKET";
static const char* luaEnumString_KeyboardKey_KEY_UNDERSCORE = "KEY_UNDERSCORE";
static const char* luaEnumString_KeyboardKey_KEY_GRAVE = "KEY_GRAVE";
static const char* luaEnumString_KeyboardKey_KEY_A = "KEY_A";
static const char* luaEnumString_KeyboardKey_KEY_B = "KEY_B";
static const char* luaEnumString_KeyboardKey_KEY_C = "KEY_C";
static const char* luaEnumString_KeyboardKey_KEY_D = "KEY_D";
static const char* luaEnumString_KeyboardKey_KEY_E = "KEY_E";
static const char* luaEnumString_KeyboardKey_KEY_F = "KEY_F";
static const char* luaEnumString_KeyboardKey_KEY_G = "KEY_G";
static const char* luaEnumString_KeyboardKey_KEY_H = "KEY_H";
static const char* luaEnumString_KeyboardKey_KEY_I = "KEY_I";
static const char* luaEnumString_KeyboardKey_KEY_J = "KEY_J";
static const char* luaEnumString_KeyboardKey_KEY_K = "KEY_K";
static const char* luaEnumString_KeyboardKey_KEY_L = "KEY_L";
static const char* luaEnumString_KeyboardKey_KEY_M = "KEY_M";
static const char* luaEnumString_KeyboardKey_KEY_N = "KEY_N";
static const char* luaEnumString_KeyboardKey_KEY_O = "KEY_O";
static const char* luaEnumString_KeyboardKey_KEY_P = "KEY_P";
static const char* luaEnumString_KeyboardKey_KEY_Q = "KEY_Q";
static const char* luaEnumString_KeyboardKey_KEY_R = "KEY_R";
static const char* luaEnumString_KeyboardKey_KEY_S = "KEY_S";
static const char* luaEnumString_KeyboardKey_KEY_T = "KEY_T";
static const char* luaEnumString_KeyboardKey_KEY_U = "KEY_U";
static const char* luaEnumString_KeyboardKey_KEY_V = "KEY_V";
static const char* luaEnumString_KeyboardKey_KEY_W = "KEY_W";
static const char* luaEnumString_KeyboardKey_KEY_X = "KEY_X";
static const char* luaEnumString_KeyboardKey_KEY_Y = "KEY_Y";
static const char* luaEnumString_KeyboardKey_KEY_Z = "KEY_Z";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_BRACE = "KEY_LEFT_BRACE";
static const char* luaEnumString_KeyboardKey_KEY_BAR = "KEY_BAR";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_BRACE = "KEY_RIGHT_BRACE";
static const char* luaEnumString_KeyboardKey_KEY_TILDE = "KEY_TILDE";
static const char* luaEnumString_KeyboardKey_KEY_EURO = "KEY_EURO";
static const char* luaEnumString_KeyboardKey_KEY_POUND = "KEY_POUND";
static const char* luaEnumString_KeyboardKey_KEY_YEN = "KEY_YEN";
static const char* luaEnumString_KeyboardKey_KEY_MIDDLE_DOT = "KEY_MIDDLE_DOT";
static const char* luaEnumString_KeyboardKey_KEY_SEARCH = "KEY_SEARCH";

Keyboard::Key lua_enumFromString_KeyboardKey(const char* s)
{
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NONE) == 0)
        return Keyboard::KEY_NONE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PAUSE) == 0)
        return Keyboard::KEY_PAUSE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SCROLL_LOCK) == 0)
        return Keyboard::KEY_SCROLL_LOCK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PRINT) == 0)
        return Keyboard::KEY_PRINT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SYSREQ) == 0)
        return Keyboard::KEY_SYSREQ;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BREAK) == 0)
        return Keyboard::KEY_BREAK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ESCAPE) == 0)
        return Keyboard::KEY_ESCAPE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BACKSPACE) == 0)
        return Keyboard::KEY_BACKSPACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_TAB) == 0)
        return Keyboard::KEY_TAB;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BACK_TAB) == 0)
        return Keyboard::KEY_BACK_TAB;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RETURN) == 0)
        return Keyboard::KEY_RETURN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPS_LOCK) == 0)
        return Keyboard::KEY_CAPS_LOCK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SHIFT) == 0)
        return Keyboard::KEY_SHIFT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CTRL) == 0)
        return Keyboard::KEY_CTRL;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ALT) == 0)
        return Keyboard::KEY_ALT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_MENU) == 0)
        return Keyboard::KEY_MENU;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_HYPER) == 0)
        return Keyboard::KEY_HYPER;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_INSERT) == 0)
        return Keyboard::KEY_INSERT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_HOME) == 0)
        return Keyboard::KEY_HOME;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PG_UP) == 0)
        return Keyboard::KEY_PG_UP;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_DELETE) == 0)
        return Keyboard::KEY_DELETE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_END) == 0)
        return Keyboard::KEY_END;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PG_DOWN) == 0)
        return Keyboard::KEY_PG_DOWN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_ARROW) == 0)
        return Keyboard::KEY_LEFT_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_ARROW) == 0)
        return Keyboard::KEY_RIGHT_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_UP_ARROW) == 0)
        return Keyboard::KEY_UP_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_DOWN_ARROW) == 0)
        return Keyboard::KEY_DOWN_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NUM_LOCK) == 0)
        return Keyboard::KEY_NUM_LOCK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_PLUS) == 0)
        return Keyboard::KEY_KP_PLUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_MINUS) == 0)
        return Keyboard::KEY_KP_MINUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_MULTIPLY) == 0)
        return Keyboard::KEY_KP_MULTIPLY;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_DIVIDE) == 0)
        return Keyboard::KEY_KP_DIVIDE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_ENTER) == 0)
        return Keyboard::KEY_KP_ENTER;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_HOME) == 0)
        return Keyboard::KEY_KP_HOME;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_UP) == 0)
        return Keyboard::KEY_KP_UP;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_PG_UP) == 0)
        return Keyboard::KEY_KP_PG_UP;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_LEFT) == 0)
        return Keyboard::KEY_KP_LEFT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_FIVE) == 0)
        return Keyboard::KEY_KP_FIVE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_RIGHT) == 0)
        return Keyboard::KEY_KP_RIGHT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_END) == 0)
        return Keyboard::KEY_KP_END;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_DOWN) == 0)
        return Keyboard::KEY_KP_DOWN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_PG_DOWN) == 0)
        return Keyboard::KEY_KP_PG_DOWN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_INSERT) == 0)
        return Keyboard::KEY_KP_INSERT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_DELETE) == 0)
        return Keyboard::KEY_KP_DELETE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F1) == 0)
        return Keyboard::KEY_F1;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F2) == 0)
        return Keyboard::KEY_F2;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F3) == 0)
        return Keyboard::KEY_F3;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F4) == 0)
        return Keyboard::KEY_F4;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F5) == 0)
        return Keyboard::KEY_F5;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F6) == 0)
        return Keyboard::KEY_F6;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F7) == 0)
        return Keyboard::KEY_F7;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F8) == 0)
        return Keyboard::KEY_F8;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F9) == 0)
        return Keyboard::KEY_F9;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F10) == 0)
        return Keyboard::KEY_F10;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F11) == 0)
        return Keyboard::KEY_F11;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F12) == 0)
        return Keyboard::KEY_F12;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SPACE) == 0)
        return Keyboard::KEY_SPACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EXCLAM) == 0)
        return Keyboard::KEY_EXCLAM;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_QUOTE) == 0)
        return Keyboard::KEY_QUOTE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NUMBER) == 0)
        return Keyboard::KEY_NUMBER;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_DOLLAR) == 0)
        return Keyboard::KEY_DOLLAR;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PERCENT) == 0)
        return Keyboard::KEY_PERCENT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CIRCUMFLEX) == 0)
        return Keyboard::KEY_CIRCUMFLEX;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_AMPERSAND) == 0)
        return Keyboard::KEY_AMPERSAND;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_APOSTROPHE) == 0)
        return Keyboard::KEY_APOSTROPHE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_PARENTHESIS) == 0)
        return Keyboard::KEY_LEFT_PARENTHESIS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_PARENTHESIS) == 0)
        return Keyboard::KEY_RIGHT_PARENTHESIS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ASTERISK) == 0)
        return Keyboard::KEY_ASTERISK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PLUS) == 0)
        return Keyboard::KEY_PLUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_COMMA) == 0)
        return Keyboard::KEY_COMMA;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_MINUS) == 0)
        return Keyboard::KEY_MINUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PERIOD) == 0)
        return Keyboard::KEY_PERIOD;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SLASH) == 0)
        return Keyboard::KEY_SLASH;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ZERO) == 0)
        return Keyboard::KEY_ZERO;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ONE) == 0)
        return Keyboard::KEY_ONE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_TWO) == 0)
        return Keyboard::KEY_TWO;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_THREE) == 0)
        return Keyboard::KEY_THREE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_FOUR) == 0)
        return Keyboard::KEY_FOUR;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_FIVE) == 0)
        return Keyboard::KEY_FIVE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SIX) == 0)
        return Keyboard::KEY_SIX;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SEVEN) == 0)
        return Keyboard::KEY_SEVEN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EIGHT) == 0)
        return Keyboard::KEY_EIGHT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NINE) == 0)
        return Keyboard::KEY_NINE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_COLON) == 0)
        return Keyboard::KEY_COLON;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SEMICOLON) == 0)
        return Keyboard::KEY_SEMICOLON;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LESS_THAN) == 0)
        return Keyboard::KEY_LESS_THAN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EQUAL) == 0)
        return Keyboard::KEY_EQUAL;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_GREATER_THAN) == 0)
        return Keyboard::KEY_GREATER_THAN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_QUESTION) == 0)
        return Keyboard::KEY_QUESTION;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_AT) == 0)
        return Keyboard::KEY_AT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_A) == 0)
        return Keyboard::KEY_CAPITAL_A;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_B) == 0)
        return Keyboard::KEY_CAPITAL_B;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_C) == 0)
        return Keyboard::KEY_CAPITAL_C;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_D) == 0)
        return Keyboard::KEY_CAPITAL_D;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_E) == 0)
        return Keyboard::KEY_CAPITAL_E;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_F) == 0)
        return Keyboard::KEY_CAPITAL_F;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_G) == 0)
        return Keyboard::KEY_CAPITAL_G;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_H) == 0)
        return Keyboard::KEY_CAPITAL_H;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_I) == 0)
        return Keyboard::KEY_CAPITAL_I;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_J) == 0)
        return Keyboard::KEY_CAPITAL_J;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_K) == 0)
        return Keyboard::KEY_CAPITAL_K;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_L) == 0)
        return Keyboard::KEY_CAPITAL_L;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_M) == 0)
        return Keyboard::KEY_CAPITAL_M;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_N) == 0)
        return Keyboard::KEY_CAPITAL_N;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_O) == 0)
        return Keyboard::KEY_CAPITAL_O;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_P) == 0)
        return Keyboard::KEY_CAPITAL_P;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_Q) == 0)
        return Keyboard::KEY_CAPITAL_Q;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_R) == 0)
        return Keyboard::KEY_CAPITAL_R;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_S) == 0)
        return Keyboard::KEY_CAPITAL_S;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_T) == 0)
        return Keyboard::KEY_CAPITAL_T;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_U) == 0)
        return Keyboard::KEY_CAPITAL_U;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_V) == 0)
        return Keyboard::KEY_CAPITAL_V;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_W) == 0)
        return Keyboard::KEY_CAPITAL_W;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_X) == 0)
        return Keyboard::KEY_CAPITAL_X;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_Y) == 0)
        return Keyboard::KEY_CAPITAL_Y;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_Z) == 0)
        return Keyboard::KEY_CAPITAL_Z;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_BRACKET) == 0)
        return Keyboard::KEY_LEFT_BRACKET;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BACK_SLASH) == 0)
        return Keyboard::KEY_BACK_SLASH;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_BRACKET) == 0)
        return Keyboard::KEY_RIGHT_BRACKET;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_UNDERSCORE) == 0)
        return Keyboard::KEY_UNDERSCORE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_GRAVE) == 0)
        return Keyboard::KEY_GRAVE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_A) == 0)
        return Keyboard::KEY_A;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_B) == 0)
        return Keyboard::KEY_B;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_C) == 0)
        return Keyboard::KEY_C;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_D) == 0)
        return Keyboard::KEY_D;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_E) == 0)
        return Keyboard::KEY_E;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F) == 0)
        return Keyboard::KEY_F;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_G) == 0)
        return Keyboard::KEY_G;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_H) == 0)
        return Keyboard::KEY_H;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_I) == 0)
        return Keyboard::KEY_I;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_J) == 0)
        return Keyboard::KEY_J;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_K) == 0)
        return Keyboard::KEY_K;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_L) == 0)
        return Keyboard::KEY_L;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_M) == 0)
        return Keyboard::KEY_M;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_N) == 0)
        return Keyboard::KEY_N;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_O) == 0)
        return Keyboard::KEY_O;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_P) == 0)
        return Keyboard::KEY_P;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_Q) == 0)
        return Keyboard::KEY_Q;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_R) == 0)
        return Keyboard::KEY_R;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_S) == 0)
        return Keyboard::KEY_S;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_T) == 0)
        return Keyboard::KEY_T;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_U) == 0)
        return Keyboard::KEY_U;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_V) == 0)
        return Keyboard::KEY_V;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_W) == 0)
        return Keyboard::KEY_W;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_X) == 0)
        return Keyboard::KEY_X;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_Y) == 0)
        return Keyboard::KEY_Y;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_Z) == 0)
        return Keyboard::KEY_Z;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_BRACE) == 0)
        return Keyboard::KEY_LEFT_BRACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BAR) == 0)
        return Keyboard::KEY_BAR;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_BRACE) == 0)
        return Keyboard::KEY_RIGHT_BRACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_TILDE) == 0)
        return Keyboard::KEY_TILDE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EURO) == 0)
        return Keyboard::KEY_EURO;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_POUND) == 0)
        return Keyboard::KEY_POUND;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_YEN) == 0)
        return Keyboard::KEY_YEN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_MIDDLE_DOT) == 0)
        return Keyboard::KEY_MIDDLE_DOT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SEARCH) == 0)
        return Keyboard::KEY_SEARCH;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Keyboard::Key.", s);
    return Keyboard::KEY_NONE;
}

const char* lua_stringFromEnum_KeyboardKey(Keyboard::Key e)
{
    if (e == Keyboard::KEY_NONE)
        return luaEnumString_KeyboardKey_KEY_NONE;
    if (e == Keyboard::KEY_PAUSE)
        return luaEnumString_KeyboardKey_KEY_PAUSE;
    if (e == Keyboard::KEY_SCROLL_LOCK)
        return luaEnumString_KeyboardKey_KEY_SCROLL_LOCK;
    if (e == Keyboard::KEY_PRINT)
        return luaEnumString_KeyboardKey_KEY_PRINT;
    if (e == Keyboard::KEY_SYSREQ)
        return luaEnumString_KeyboardKey_KEY_SYSREQ;
    if (e == Keyboard::KEY_BREAK)
        return luaEnumString_KeyboardKey_KEY_BREAK;
    if (e == Keyboard::KEY_ESCAPE)
        return luaEnumString_KeyboardKey_KEY_ESCAPE;
    if (e == Keyboard::KEY_BACKSPACE)
        return luaEnumString_KeyboardKey_KEY_BACKSPACE;
    if (e == Keyboard::KEY_TAB)
        return luaEnumString_KeyboardKey_KEY_TAB;
    if (e == Keyboard::KEY_BACK_TAB)
        return luaEnumString_KeyboardKey_KEY_BACK_TAB;
    if (e == Keyboard::KEY_RETURN)
        return luaEnumString_KeyboardKey_KEY_RETURN;
    if (e == Keyboard::KEY_CAPS_LOCK)
        return luaEnumString_KeyboardKey_KEY_CAPS_LOCK;
    if (e == Keyboard::KEY_SHIFT)
        return luaEnumString_KeyboardKey_KEY_SHIFT;
    if (e == Keyboard::KEY_CTRL)
        return luaEnumString_KeyboardKey_KEY_CTRL;
    if (e == Keyboard::KEY_ALT)
        return luaEnumString_KeyboardKey_KEY_ALT;
    if (e == Keyboard::KEY_MENU)
        return luaEnumString_KeyboardKey_KEY_MENU;
    if (e == Keyboard::KEY_HYPER)
        return luaEnumString_KeyboardKey_KEY_HYPER;
    if (e == Keyboard::KEY_INSERT)
        return luaEnumString_KeyboardKey_KEY_INSERT;
    if (e == Keyboard::KEY_HOME)
        return luaEnumString_KeyboardKey_KEY_HOME;
    if (e == Keyboard::KEY_PG_UP)
        return luaEnumString_KeyboardKey_KEY_PG_UP;
    if (e == Keyboard::KEY_DELETE)
        return luaEnumString_KeyboardKey_KEY_DELETE;
    if (e == Keyboard::KEY_END)
        return luaEnumString_KeyboardKey_KEY_END;
    if (e == Keyboard::KEY_PG_DOWN)
        return luaEnumString_KeyboardKey_KEY_PG_DOWN;
    if (e == Keyboard::KEY_LEFT_ARROW)
        return luaEnumString_KeyboardKey_KEY_LEFT_ARROW;
    if (e == Keyboard::KEY_RIGHT_ARROW)
        return luaEnumString_KeyboardKey_KEY_RIGHT_ARROW;
    if (e == Keyboard::KEY_UP_ARROW)
        return luaEnumString_KeyboardKey_KEY_UP_ARROW;
    if (e == Keyboard::KEY_DOWN_ARROW)
        return luaEnumString_KeyboardKey_KEY_DOWN_ARROW;
    if (e == Keyboard::KEY_NUM_LOCK)
        return luaEnumString_KeyboardKey_KEY_NUM_LOCK;
    if (e == Keyboard::KEY_KP_PLUS)
        return luaEnumString_KeyboardKey_KEY_KP_PLUS;
    if (e == Keyboard::KEY_KP_MINUS)
        return luaEnumString_KeyboardKey_KEY_KP_MINUS;
    if (e == Keyboard::KEY_KP_MULTIPLY)
        return luaEnumString_KeyboardKey_KEY_KP_MULTIPLY;
    if (e == Keyboard::KEY_KP_DIVIDE)
        return luaEnumString_KeyboardKey_KEY_KP_DIVIDE;
    if (e == Keyboard::KEY_KP_ENTER)
        return luaEnumString_KeyboardKey_KEY_KP_ENTER;
    if (e == Keyboard::KEY_KP_HOME)
        return luaEnumString_KeyboardKey_KEY_KP_HOME;
    if (e == Keyboard::KEY_KP_UP)
        return luaEnumString_KeyboardKey_KEY_KP_UP;
    if (e == Keyboard::KEY_KP_PG_UP)
        return luaEnumString_KeyboardKey_KEY_KP_PG_UP;
    if (e == Keyboard::KEY_KP_LEFT)
        return luaEnumString_KeyboardKey_KEY_KP_LEFT;
    if (e == Keyboard::KEY_KP_FIVE)
        return luaEnumString_KeyboardKey_KEY_KP_FIVE;
    if (e == Keyboard::KEY_KP_RIGHT)
        return luaEnumString_KeyboardKey_KEY_KP_RIGHT;
    if (e == Keyboard::KEY_KP_END)
        return luaEnumString_KeyboardKey_KEY_KP_END;
    if (e == Keyboard::KEY_KP_DOWN)
        return luaEnumString_KeyboardKey_KEY_KP_DOWN;
    if (e == Keyboard::KEY_KP_PG_DOWN)
        return luaEnumString_KeyboardKey_KEY_KP_PG_DOWN;
    if (e == Keyboard::KEY_KP_INSERT)
        return luaEnumString_KeyboardKey_KEY_KP_INSERT;
    if (e == Keyboard::KEY_KP_DELETE)
        return luaEnumString_KeyboardKey_KEY_KP_DELETE;
    if (e == Keyboard::KEY_F1)
        return luaEnumString_KeyboardKey_KEY_F1;
    if (e == Keyboard::KEY_F2)
        return luaEnumString_KeyboardKey_KEY_F2;
    if (e == Keyboard::KEY_F3)
        return luaEnumString_KeyboardKey_KEY_F3;
    if (e == Keyboard::KEY_F4)
        return luaEnumString_KeyboardKey_KEY_F4;
    if (e == Keyboard::KEY_F5)
        return luaEnumString_KeyboardKey_KEY_F5;
    if (e == Keyboard::KEY_F6)
        return luaEnumString_KeyboardKey_KEY_F6;
    if (e == Keyboard::KEY_F7)
        return luaEnumString_KeyboardKey_KEY_F7;
    if (e == Keyboard::KEY_F8)
        return luaEnumString_KeyboardKey_KEY_F8;
    if (e == Keyboard::KEY_F9)
        return luaEnumString_KeyboardKey_KEY_F9;
    if (e == Keyboard::KEY_F10)
        return luaEnumString_KeyboardKey_KEY_F10;
    if (e == Keyboard::KEY_F11)
        return luaEnumString_KeyboardKey_KEY_F11;
    if (e == Keyboard::KEY_F12)
        return luaEnumString_KeyboardKey_KEY_F12;
    if (e == Keyboard::KEY_SPACE)
        return luaEnumString_KeyboardKey_KEY_SPACE;
    if (e == Keyboard::KEY_EXCLAM)
        return luaEnumString_KeyboardKey_KEY_EXCLAM;
    if (e == Keyboard::KEY_QUOTE)
        return luaEnumString_KeyboardKey_KEY_QUOTE;
    if (e == Keyboard::KEY_NUMBER)
        return luaEnumString_KeyboardKey_KEY_NUMBER;
    if (e == Keyboard::KEY_DOLLAR)
        return luaEnumString_KeyboardKey_KEY_DOLLAR;
    if (e == Keyboard::KEY_PERCENT)
        return luaEnumString_KeyboardKey_KEY_PERCENT;
    if (e == Keyboard::KEY_CIRCUMFLEX)
        return luaEnumString_KeyboardKey_KEY_CIRCUMFLEX;
    if (e == Keyboard::KEY_AMPERSAND)
        return luaEnumString_KeyboardKey_KEY_AMPERSAND;
    if (e == Keyboard::KEY_APOSTROPHE)
        return luaEnumString_KeyboardKey_KEY_APOSTROPHE;
    if (e == Keyboard::KEY_LEFT_PARENTHESIS)
        return luaEnumString_KeyboardKey_KEY_LEFT_PARENTHESIS;
    if (e == Keyboard::KEY_RIGHT_PARENTHESIS)
        return luaEnumString_KeyboardKey_KEY_RIGHT_PARENTHESIS;
    if (e == Keyboard::KEY_ASTERISK)
        return luaEnumString_KeyboardKey_KEY_ASTERISK;
    if (e == Keyboard::KEY_PLUS)
        return luaEnumString_KeyboardKey_KEY_PLUS;
    if (e == Keyboard::KEY_COMMA)
        return luaEnumString_KeyboardKey_KEY_COMMA;
    if (e == Keyboard::KEY_MINUS)
        return luaEnumString_KeyboardKey_KEY_MINUS;
    if (e == Keyboard::KEY_PERIOD)
        return luaEnumString_KeyboardKey_KEY_PERIOD;
    if (e == Keyboard::KEY_SLASH)
        return luaEnumString_KeyboardKey_KEY_SLASH;
    if (e == Keyboard::KEY_ZERO)
        return luaEnumString_KeyboardKey_KEY_ZERO;
    if (e == Keyboard::KEY_ONE)
        return luaEnumString_KeyboardKey_KEY_ONE;
    if (e == Keyboard::KEY_TWO)
        return luaEnumString_KeyboardKey_KEY_TWO;
    if (e == Keyboard::KEY_THREE)
        return luaEnumString_KeyboardKey_KEY_THREE;
    if (e == Keyboard::KEY_FOUR)
        return luaEnumString_KeyboardKey_KEY_FOUR;
    if (e == Keyboard::KEY_FIVE)
        return luaEnumString_KeyboardKey_KEY_FIVE;
    if (e == Keyboard::KEY_SIX)
        return luaEnumString_KeyboardKey_KEY_SIX;
    if (e == Keyboard::KEY_SEVEN)
        return luaEnumString_KeyboardKey_KEY_SEVEN;
    if (e == Keyboard::KEY_EIGHT)
        return luaEnumString_KeyboardKey_KEY_EIGHT;
    if (e == Keyboard::KEY_NINE)
        return luaEnumString_KeyboardKey_KEY_NINE;
    if (e == Keyboard::KEY_COLON)
        return luaEnumString_KeyboardKey_KEY_COLON;
    if (e == Keyboard::KEY_SEMICOLON)
        return luaEnumString_KeyboardKey_KEY_SEMICOLON;
    if (e == Keyboard::KEY_LESS_THAN)
        return luaEnumString_KeyboardKey_KEY_LESS_THAN;
    if (e == Keyboard::KEY_EQUAL)
        return luaEnumString_KeyboardKey_KEY_EQUAL;
    if (e == Keyboard::KEY_GREATER_THAN)
        return luaEnumString_KeyboardKey_KEY_GREATER_THAN;
    if (e == Keyboard::KEY_QUESTION)
        return luaEnumString_KeyboardKey_KEY_QUESTION;
    if (e == Keyboard::KEY_AT)
        return luaEnumString_KeyboardKey_KEY_AT;
    if (e == Keyboard::KEY_CAPITAL_A)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_A;
    if (e == Keyboard::KEY_CAPITAL_B)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_B;
    if (e == Keyboard::KEY_CAPITAL_C)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_C;
    if (e == Keyboard::KEY_CAPITAL_D)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_D;
    if (e == Keyboard::KEY_CAPITAL_E)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_E;
    if (e == Keyboard::KEY_CAPITAL_F)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_F;
    if (e == Keyboard::KEY_CAPITAL_G)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_G;
    if (e == Keyboard::KEY_CAPITAL_H)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_H;
    if (e == Keyboard::KEY_CAPITAL_I)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_I;
    if (e == Keyboard::KEY_CAPITAL_J)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_J;
    if (e == Keyboard::KEY_CAPITAL_K)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_K;
    if (e == Keyboard::KEY_CAPITAL_L)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_L;
    if (e == Keyboard::KEY_CAPITAL_M)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_M;
    if (e == Keyboard::KEY_CAPITAL_N)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_N;
    if (e == Keyboard::KEY_CAPITAL_O)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_O;
    if (e == Keyboard::KEY_CAPITAL_P)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_P;
    if (e == Keyboard::KEY_CAPITAL_Q)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_Q;
    if (e == Keyboard::KEY_CAPITAL_R)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_R;
    if (e == Keyboard::KEY_CAPITAL_S)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_S;
    if (e == Keyboard::KEY_CAPITAL_T)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_T;
    if (e == Keyboard::KEY_CAPITAL_U)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_U;
    if (e == Keyboard::KEY_CAPITAL_V)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_V;
    if (e == Keyboard::KEY_CAPITAL_W)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_W;
    if (e == Keyboard::KEY_CAPITAL_X)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_X;
    if (e == Keyboard::KEY_CAPITAL_Y)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_Y;
    if (e == Keyboard::KEY_CAPITAL_Z)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_Z;
    if (e == Keyboard::KEY_LEFT_BRACKET)
        return luaEnumString_KeyboardKey_KEY_LEFT_BRACKET;
    if (e == Keyboard::KEY_BACK_SLASH)
        return luaEnumString_KeyboardKey_KEY_BACK_SLASH;
    if (e == Keyboard::KEY_RIGHT_BRACKET)
        return luaEnumString_KeyboardKey_KEY_RIGHT_BRACKET;
    if (e == Keyboard::KEY_UNDERSCORE)
        return luaEnumString_KeyboardKey_KEY_UNDERSCORE;
    if (e == Keyboard::KEY_GRAVE)
        return luaEnumString_KeyboardKey_KEY_GRAVE;
    if (e == Keyboard::KEY_A)
        return luaEnumString_KeyboardKey_KEY_A;
    if (e == Keyboard::KEY_B)
        return luaEnumString_KeyboardKey_KEY_B;
    if (e == Keyboard::KEY_C)
        return luaEnumString_KeyboardKey_KEY_C;
    if (e == Keyboard::KEY_D)
        return luaEnumString_KeyboardKey_KEY_D;
    if (e == Keyboard::KEY_E)
        return luaEnumString_KeyboardKey_KEY_E;
    if (e == Keyboard::KEY_F)
        return luaEnumString_KeyboardKey_KEY_F;
    if (e == Keyboard::KEY_G)
        return luaEnumString_KeyboardKey_KEY_G;
    if (e == Keyboard::KEY_H)
        return luaEnumString_KeyboardKey_KEY_H;
    if (e == Keyboard::KEY_I)
        return luaEnumString_KeyboardKey_KEY_I;
    if (e == Keyboard::KEY_J)
        return luaEnumString_KeyboardKey_KEY_J;
    if (e == Keyboard::KEY_K)
        return luaEnumString_KeyboardKey_KEY_K;
    if (e == Keyboard::KEY_L)
        return luaEnumString_KeyboardKey_KEY_L;
    if (e == Keyboard::KEY_M)
        return luaEnumString_KeyboardKey_KEY_M;
    if (e == Keyboard::KEY_N)
        return luaEnumString_KeyboardKey_KEY_N;
    if (e == Keyboard::KEY_O)
        return luaEnumString_KeyboardKey_KEY_O;
    if (e == Keyboard::KEY_P)
        return luaEnumString_KeyboardKey_KEY_P;
    if (e == Keyboard::KEY_Q)
        return luaEnumString_KeyboardKey_KEY_Q;
    if (e == Keyboard::KEY_R)
        return luaEnumString_KeyboardKey_KEY_R;
    if (e == Keyboard::KEY_S)
        return luaEnumString_KeyboardKey_KEY_S;
    if (e == Keyboard::KEY_T)
        return luaEnumString_KeyboardKey_KEY_T;
    if (e == Keyboard::KEY_U)
        return luaEnumString_KeyboardKey_KEY_U;
    if (e == Keyboard::KEY_V)
        return luaEnumString_KeyboardKey_KEY_V;
    if (e == Keyboard::KEY_W)
        return luaEnumString_KeyboardKey_KEY_W;
    if (e == Keyboard::KEY_X)
        return luaEnumString_KeyboardKey_KEY_X;
    if (e == Keyboard::KEY_Y)
        return luaEnumString_KeyboardKey_KEY_Y;
    if (e == Keyboard::KEY_Z)
        return luaEnumString_KeyboardKey_KEY_Z;
    if (e == Keyboard::KEY_LEFT_BRACE)
        return luaEnumString_KeyboardKey_KEY_LEFT_BRACE;
    if (e == Keyboard::KEY_BAR)
        return luaEnumString_KeyboardKey_KEY_BAR;
    if (e == Keyboard::KEY_RIGHT_BRACE)
        return luaEnumString_KeyboardKey_KEY_RIGHT_BRACE;
    if (e == Keyboard::KEY_TILDE)
        return luaEnumString_KeyboardKey_KEY_TILDE;
    if (e == Keyboard::KEY_EURO)
        return luaEnumString_KeyboardKey_KEY_EURO;
    if (e == Keyboard::KEY_POUND)
        return luaEnumString_KeyboardKey_KEY_POUND;
    if (e == Keyboard::KEY_YEN)
        return luaEnumString_KeyboardKey_KEY_YEN;
    if (e == Keyboard::KEY_MIDDLE_DOT)
        return luaEnumString_KeyboardKey_KEY_MIDDLE_DOT;
    if (e == Keyboard::KEY_SEARCH)
        return luaEnumString_KeyboardKey_KEY_SEARCH;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Keyboard::Key.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_KeyboardKeyEvent_KEY_PRESS = "KEY_PRESS";
static const char* luaEnumString_KeyboardKeyEvent_KEY_RELEASE = "KEY_RELEASE";
static const char* luaEnumString_KeyboardKeyEvent_KEY_CHAR = "KEY_CHAR";

Keyboard::KeyEvent lua_enumFromString_KeyboardKeyEvent(const char* s)
{
    if (strcmp(s, luaEnumString_KeyboardKeyEvent_KEY_PRESS) == 0)
        return Keyboard::KEY_PRESS;
    if (strcmp(s, luaEnumString_KeyboardKeyEvent_KEY_RELEASE) == 0)
        return Keyboard::KEY_RELEASE;
    if (strcmp(s, luaEnumString_KeyboardKeyEvent_KEY_CHAR) == 0)
        return Keyboard::KEY_CHAR;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Keyboard::KeyEvent.", s);
    return Keyboard::KEY_PRESS;
}

const char* lua_stringFromEnum_KeyboardKeyEvent(Keyboard::KeyEvent e)
{
    if (e == Keyboard::KEY_PRESS)
        return luaEnumString_KeyboardKeyEvent_KEY_PRESS;
    if (e == Keyboard::KEY_RELEASE)
        return luaEnumString_KeyboardKeyEvent_KEY_RELEASE;
    if (e == Keyboard::KEY_CHAR)
        return luaEnumString_KeyboardKeyEvent_KEY_CHAR;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Keyboard::KeyEvent.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_LayoutType_LAYOUT_FLOW = "LAYOUT_FLOW";
static const char* luaEnumString_LayoutType_LAYOUT_VERTICAL = "LAYOUT_VERTICAL";
static const char* luaEnumString_LayoutType_LAYOUT_ABSOLUTE = "LAYOUT_ABSOLUTE";
static const char* luaEnumString_LayoutType_LAYOUT_SCROLL = "LAYOUT_SCROLL";

Layout::Type lua_enumFromString_LayoutType(const char* s)
{
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_FLOW) == 0)
        return Layout::LAYOUT_FLOW;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_VERTICAL) == 0)
        return Layout::LAYOUT_VERTICAL;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_ABSOLUTE) == 0)
        return Layout::LAYOUT_ABSOLUTE;
    if (strcmp(s, luaEnumString_LayoutType_LAYOUT_SCROLL) == 0)
        return Layout::LAYOUT_SCROLL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Layout::Type.", s);
    return Layout::LAYOUT_FLOW;
}

const char* lua_stringFromEnum_LayoutType(Layout::Type e)
{
    if (e == Layout::LAYOUT_FLOW)
        return luaEnumString_LayoutType_LAYOUT_FLOW;
    if (e == Layout::LAYOUT_VERTICAL)
        return luaEnumString_LayoutType_LAYOUT_VERTICAL;
    if (e == Layout::LAYOUT_ABSOLUTE)
        return luaEnumString_LayoutType_LAYOUT_ABSOLUTE;
    if (e == Layout::LAYOUT_SCROLL)
        return luaEnumString_LayoutType_LAYOUT_SCROLL;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Layout::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_LightType_DIRECTIONAL = "DIRECTIONAL";
static const char* luaEnumString_LightType_POINT = "POINT";
static const char* luaEnumString_LightType_SPOT = "SPOT";

Light::Type lua_enumFromString_LightType(const char* s)
{
    if (strcmp(s, luaEnumString_LightType_DIRECTIONAL) == 0)
        return Light::DIRECTIONAL;
    if (strcmp(s, luaEnumString_LightType_POINT) == 0)
        return Light::POINT;
    if (strcmp(s, luaEnumString_LightType_SPOT) == 0)
        return Light::SPOT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Light::Type.", s);
    return Light::DIRECTIONAL;
}

const char* lua_stringFromEnum_LightType(Light::Type e)
{
    if (e == Light::DIRECTIONAL)
        return luaEnumString_LightType_DIRECTIONAL;
    if (e == Light::POINT)
        return luaEnumString_LightType_POINT;
    if (e == Light::SPOT)
        return luaEnumString_LightType_SPOT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Light::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_MeshIndexFormat_INDEX8 = "INDEX8";
static const char* luaEnumString_MeshIndexFormat_INDEX16 = "INDEX16";
static const char* luaEnumString_MeshIndexFormat_INDEX32 = "INDEX32";

Mesh::IndexFormat lua_enumFromString_MeshIndexFormat(const char* s)
{
    if (strcmp(s, luaEnumString_MeshIndexFormat_INDEX8) == 0)
        return Mesh::INDEX8;
    if (strcmp(s, luaEnumString_MeshIndexFormat_INDEX16) == 0)
        return Mesh::INDEX16;
    if (strcmp(s, luaEnumString_MeshIndexFormat_INDEX32) == 0)
        return Mesh::INDEX32;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Mesh::IndexFormat.", s);
    return Mesh::INDEX8;
}

const char* lua_stringFromEnum_MeshIndexFormat(Mesh::IndexFormat e)
{
    if (e == Mesh::INDEX8)
        return luaEnumString_MeshIndexFormat_INDEX8;
    if (e == Mesh::INDEX16)
        return luaEnumString_MeshIndexFormat_INDEX16;
    if (e == Mesh::INDEX32)
        return luaEnumString_MeshIndexFormat_INDEX32;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Mesh::IndexFormat.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_MeshPrimitiveType_TRIANGLES = "TRIANGLES";
static const char* luaEnumString_MeshPrimitiveType_TRIANGLE_STRIP = "TRIANGLE_STRIP";
static const char* luaEnumString_MeshPrimitiveType_LINES = "LINES";
static const char* luaEnumString_MeshPrimitiveType_LINE_STRIP = "LINE_STRIP";
static const char* luaEnumString_MeshPrimitiveType_POINTS = "POINTS";

Mesh::PrimitiveType lua_enumFromString_MeshPrimitiveType(const char* s)
{
    if (strcmp(s, luaEnumString_MeshPrimitiveType_TRIANGLES) == 0)
        return Mesh::TRIANGLES;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_TRIANGLE_STRIP) == 0)
        return Mesh::TRIANGLE_STRIP;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_LINES) == 0)
        return Mesh::LINES;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_LINE_STRIP) == 0)
        return Mesh::LINE_STRIP;
    if (strcmp(s, luaEnumString_MeshPrimitiveType_POINTS) == 0)
        return Mesh::POINTS;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Mesh::PrimitiveType.", s);
    return Mesh::TRIANGLES;
}

const char* lua_stringFromEnum_MeshPrimitiveType(Mesh::PrimitiveType e)
{
    if (e == Mesh::TRIANGLES)
        return luaEnumString_MeshPrimitiveType_TRIANGLES;
    if (e == Mesh::TRIANGLE_STRIP)
        return luaEnumString_MeshPrimitiveType_TRIANGLE_STRIP;
    if (e == Mesh::LINES)
        return luaEnumString_MeshPrimitiveType_LINES;
    if (e == Mesh::LINE_STRIP)
        return luaEnumString_MeshPrimitiveType_LINE_STRIP;
    if (e == Mesh::POINTS)
        return luaEnumString_MeshPrimitiveType_POINTS;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Mesh::PrimitiveType.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_MouseMouseEvent_MOUSE_PRESS_LEFT_BUTTON = "MOUSE_PRESS_LEFT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_RELEASE_LEFT_BUTTON = "MOUSE_RELEASE_LEFT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_PRESS_MIDDLE_BUTTON = "MOUSE_PRESS_MIDDLE_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_RELEASE_MIDDLE_BUTTON = "MOUSE_RELEASE_MIDDLE_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_PRESS_RIGHT_BUTTON = "MOUSE_PRESS_RIGHT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_RELEASE_RIGHT_BUTTON = "MOUSE_RELEASE_RIGHT_BUTTON";
static const char* luaEnumString_MouseMouseEvent_MOUSE_MOVE = "MOUSE_MOVE";
static const char* luaEnumString_MouseMouseEvent_MOUSE_WHEEL = "MOUSE_WHEEL";

Mouse::MouseEvent lua_enumFromString_MouseMouseEvent(const char* s)
{
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_PRESS_LEFT_BUTTON) == 0)
        return Mouse::MOUSE_PRESS_LEFT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_RELEASE_LEFT_BUTTON) == 0)
        return Mouse::MOUSE_RELEASE_LEFT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_PRESS_MIDDLE_BUTTON) == 0)
        return Mouse::MOUSE_PRESS_MIDDLE_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_RELEASE_MIDDLE_BUTTON) == 0)
        return Mouse::MOUSE_RELEASE_MIDDLE_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_PRESS_RIGHT_BUTTON) == 0)
        return Mouse::MOUSE_PRESS_RIGHT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_RELEASE_RIGHT_BUTTON) == 0)
        return Mouse::MOUSE_RELEASE_RIGHT_BUTTON;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_MOVE) == 0)
        return Mouse::MOUSE_MOVE;
    if (strcmp(s, luaEnumString_MouseMouseEvent_MOUSE_WHEEL) == 0)
        return Mouse::MOUSE_WHEEL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Mouse::MouseEvent.", s);
    return Mouse::MOUSE_PRESS_LEFT_BUTTON;
}

const char* lua_stringFromEnum_MouseMouseEvent(Mouse::MouseEvent e)
{
    if (e == Mouse::MOUSE_PRESS_LEFT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_PRESS_LEFT_BUTTON;
    if (e == Mouse::MOUSE_RELEASE_LEFT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_RELEASE_LEFT_BUTTON;
    if (e == Mouse::MOUSE_PRESS_MIDDLE_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_PRESS_MIDDLE_BUTTON;
    if (e == Mouse::MOUSE_RELEASE_MIDDLE_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_RELEASE_MIDDLE_BUTTON;
    if (e == Mouse::MOUSE_PRESS_RIGHT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_PRESS_RIGHT_BUTTON;
    if (e == Mouse::MOUSE_RELEASE_RIGHT_BUTTON)
        return luaEnumString_MouseMouseEvent_MOUSE_RELEASE_RIGHT_BUTTON;
    if (e == Mouse::MOUSE_MOVE)
        return luaEnumString_MouseMouseEvent_MOUSE_MOVE;
    if (e == Mouse::MOUSE_WHEEL)
        return luaEnumString_MouseMouseEvent_MOUSE_WHEEL;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Mouse::MouseEvent.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_NodeType_NODE = "NODE";
static const char* luaEnumString_NodeType_JOINT = "JOINT";

Node::Type lua_enumFromString_NodeType(const char* s)
{
    if (strcmp(s, luaEnumString_NodeType_NODE) == 0)
        return Node::NODE;
    if (strcmp(s, luaEnumString_NodeType_JOINT) == 0)
        return Node::JOINT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Node::Type.", s);
    return Node::NODE;
}

const char* lua_stringFromEnum_NodeType(Node::Type e)
{
    if (e == Node::NODE)
        return luaEnumString_NodeType_NODE;
    if (e == Node::JOINT)
        return luaEnumString_NodeType_JOINT;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Node::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_OPAQUE = "BLEND_OPAQUE";
static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_TRANSPARENT = "BLEND_TRANSPARENT";
static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_ADDITIVE = "BLEND_ADDITIVE";
static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_MULTIPLIED = "BLEND_MULTIPLIED";

ParticleEmitter::TextureBlending lua_enumFromString_ParticleEmitterTextureBlending(const char* s)
{
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_OPAQUE) == 0)
        return ParticleEmitter::BLEND_OPAQUE;
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_TRANSPARENT) == 0)
        return ParticleEmitter::BLEND_TRANSPARENT;
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_ADDITIVE) == 0)
        return ParticleEmitter::BLEND_ADDITIVE;
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_MULTIPLIED) == 0)
        return ParticleEmitter::BLEND_MULTIPLIED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration ParticleEmitter::TextureBlending.", s);
    return ParticleEmitter::BLEND_OPAQUE;
}

const char* lua_stringFromEnum_ParticleEmitterTextureBlending(ParticleEmitter::TextureBlending e)
{
    if (e == ParticleEmitter::BLEND_OPAQUE)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_OPAQUE;
    if (e == ParticleEmitter::BLEND_TRANSPARENT)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_TRANSPARENT;
    if (e == ParticleEmitter::BLEND_ADDITIVE)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_ADDITIVE;
    if (e == ParticleEmitter::BLEND_MULTIPLIED)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_MULTIPLIED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration ParticleEmitter::TextureBlending.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_COLLIDING = "COLLIDING";
static const char* luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_NOT_COLLIDING = "NOT_COLLIDING";

PhysicsCollisionObject::CollisionListener::EventType lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_COLLIDING) == 0)
        return PhysicsCollisionObject::CollisionListener::COLLIDING;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_NOT_COLLIDING) == 0)
        return PhysicsCollisionObject::CollisionListener::NOT_COLLIDING;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsCollisionObject::CollisionListener::EventType.", s);
    return PhysicsCollisionObject::CollisionListener::COLLIDING;
}

const char* lua_stringFromEnum_PhysicsCollisionObjectCollisionListenerEventType(PhysicsCollisionObject::CollisionListener::EventType e)
{
    if (e == PhysicsCollisionObject::CollisionListener::COLLIDING)
        return luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_COLLIDING;
    if (e == PhysicsCollisionObject::CollisionListener::NOT_COLLIDING)
        return luaEnumString_PhysicsCollisionObjectCollisionListenerEventType_NOT_COLLIDING;
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsCollisionObject::CollisionListener::EventType.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_PhysicsCollisionObjectType_RIGID_BODY = "RIGID_BODY";
static const char* luaEnumString_PhysicsCollisionObjectType_CHARACTER = "CHARACTER";
static const char* luaEnumString_PhysicsCollisionObjectType_GHOST_OBJECT = "GHOST_OBJECT";
static const char* luaEnumString_PhysicsCollisionObjectType_NONE = "NONE";

PhysicsCollisionObject::Type lua_enumFromString_PhysicsCollisionObjectType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_RIGID_BODY) == 0)
        return PhysicsCollisionObject::RIGID_BODY;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_CHARACTER) == 0)
        return PhysicsCollisionObject::CHARACTER;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_GHOST_OBJECT) == 0)
        return PhysicsCollisionObject::GHOST_OBJECT;
    if (strcmp(s, luaEnumString_PhysicsCollisionObjectType_NONE) == 0)
        return PhysicsCollisionObject::NONE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsCollisionObject::Type.", s);
    return PhysicsCollisionObject::RIGID_BODY;
}

const char* lua_stringFromEnum_PhysicsCollisionObjectType(PhysicsCollisionObject::Type e)
{
    if (e == PhysicsCollisionObject::RIGID_BODY)
        return luaEnumString_PhysicsCollisionObjectType_RIGID_BODY;
    if (e == PhysicsCollisionObject::CHARACTER)
        return luaEnumString_PhysicsCollisionObjectType_CHARACTER;
    if (e == PhysicsCollisionObject::GHOST_OBJECT)
        return luaEnumString_PhysicsCollisionObjectType_GHOST_OBJECT;
    if (e == PhysicsCollisionObject::NONE)
        return luaEnumString_PhysicsCollisionObjectType_NONE;
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsCollisionObject::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_BOX = "SHAPE_BOX";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_SPHERE = "SHAPE_SPHERE";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_CAPSULE = "SHAPE_CAPSULE";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_MESH = "SHAPE_MESH";
static const char* luaEnumString_PhysicsCollisionShapeType_SHAPE_HEIGHTFIELD = "SHAPE_HEIGHTFIELD";

PhysicsCollisionShape::Type lua_enumFromString_PhysicsCollisionShapeType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_BOX) == 0)
        return PhysicsCollisionShape::SHAPE_BOX;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_SPHERE) == 0)
        return PhysicsCollisionShape::SHAPE_SPHERE;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_CAPSULE) == 0)
        return PhysicsCollisionShape::SHAPE_CAPSULE;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_MESH) == 0)
        return PhysicsCollisionShape::SHAPE_MESH;
    if (strcmp(s, luaEnumString_PhysicsCollisionShapeType_SHAPE_HEIGHTFIELD) == 0)
        return PhysicsCollisionShape::SHAPE_HEIGHTFIELD;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsCollisionShape::Type.", s);
    return PhysicsCollisionShape::SHAPE_BOX;
}

const char* lua_stringFromEnum_PhysicsCollisionShapeType(PhysicsCollisionShape::Type e)
{
    if (e == PhysicsCollisionShape::SHAPE_BOX)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_BOX;
    if (e == PhysicsCollisionShape::SHAPE_SPHERE)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_SPHERE;
    if (e == PhysicsCollisionShape::SHAPE_CAPSULE)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_CAPSULE;
    if (e == PhysicsCollisionShape::SHAPE_MESH)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_MESH;
    if (e == PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
        return luaEnumString_PhysicsCollisionShapeType_SHAPE_HEIGHTFIELD;
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsCollisionShape::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_PhysicsControllerListenerEventType_ACTIVATED = "ACTIVATED";
static const char* luaEnumString_PhysicsControllerListenerEventType_DEACTIVATED = "DEACTIVATED";

PhysicsController::Listener::EventType lua_enumFromString_PhysicsControllerListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsControllerListenerEventType_ACTIVATED) == 0)
        return PhysicsController::Listener::ACTIVATED;
    if (strcmp(s, luaEnumString_PhysicsControllerListenerEventType_DEACTIVATED) == 0)
        return PhysicsController::Listener::DEACTIVATED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsController::Listener::EventType.", s);
    return PhysicsController::Listener::ACTIVATED;
}

const char* lua_stringFromEnum_PhysicsControllerListenerEventType(PhysicsController::Listener::EventType e)
{
    if (e == PhysicsController::Listener::ACTIVATED)
        return luaEnumString_PhysicsControllerListenerEventType_ACTIVATED;
    if (e == PhysicsController::Listener::DEACTIVATED)
        return luaEnumString_PhysicsControllerListenerEventType_DEACTIVATED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsController::Listener::EventType.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_PropertiesType_NONE = "NONE";
static const char* luaEnumString_PropertiesType_STRING = "STRING";
static const char* luaEnumString_PropertiesType_NUMBER = "NUMBER";
static const char* luaEnumString_PropertiesType_VECTOR2 = "VECTOR2";
static const char* luaEnumString_PropertiesType_VECTOR3 = "VECTOR3";
static const char* luaEnumString_PropertiesType_VECTOR4 = "VECTOR4";
static const char* luaEnumString_PropertiesType_MATRIX = "MATRIX";

Properties::Type lua_enumFromString_PropertiesType(const char* s)
{
    if (strcmp(s, luaEnumString_PropertiesType_NONE) == 0)
        return Properties::NONE;
    if (strcmp(s, luaEnumString_PropertiesType_STRING) == 0)
        return Properties::STRING;
    if (strcmp(s, luaEnumString_PropertiesType_NUMBER) == 0)
        return Properties::NUMBER;
    if (strcmp(s, luaEnumString_PropertiesType_VECTOR2) == 0)
        return Properties::VECTOR2;
    if (strcmp(s, luaEnumString_PropertiesType_VECTOR3) == 0)
        return Properties::VECTOR3;
    if (strcmp(s, luaEnumString_PropertiesType_VECTOR4) == 0)
        return Properties::VECTOR4;
    if (strcmp(s, luaEnumString_PropertiesType_MATRIX) == 0)
        return Properties::MATRIX;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Properties::Type.", s);
    return Properties::NONE;
}

const char* lua_stringFromEnum_PropertiesType(Properties::Type e)
{
    if (e == Properties::NONE)
        return luaEnumString_PropertiesType_NONE;
    if (e == Properties::STRING)
        return luaEnumString_PropertiesType_STRING;
    if (e == Properties::NUMBER)
        return luaEnumString_PropertiesType_NUMBER;
    if (e == Properties::VECTOR2)
        return luaEnumString_PropertiesType_VECTOR2;
    if (e == Properties::VECTOR3)
        return luaEnumString_PropertiesType_VECTOR3;
    if (e == Properties::VECTOR4)
        return luaEnumString_PropertiesType_VECTOR4;
    if (e == Properties::MATRIX)
        return luaEnumString_PropertiesType_MATRIX;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Properties::Type.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_RenderStateAutoBinding_NONE = "NONE";
static const char* luaEnumString_RenderStateAutoBinding_WORLD_MATRIX = "WORLD_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_VIEW_MATRIX = "VIEW_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_PROJECTION_MATRIX = "PROJECTION_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_WORLD_VIEW_MATRIX = "WORLD_VIEW_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_VIEW_PROJECTION_MATRIX = "VIEW_PROJECTION_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_WORLD_VIEW_PROJECTION_MATRIX = "WORLD_VIEW_PROJECTION_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_MATRIX = "INVERSE_TRANSPOSE_WORLD_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX = "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_CAMERA_WORLD_POSITION = "CAMERA_WORLD_POSITION";
static const char* luaEnumString_RenderStateAutoBinding_CAMERA_VIEW_POSITION = "CAMERA_VIEW_POSITION";
static const char* luaEnumString_RenderStateAutoBinding_MATRIX_PALETTE = "MATRIX_PALETTE";

RenderState::AutoBinding lua_enumFromString_RenderStateAutoBinding(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_NONE) == 0)
        return RenderState::NONE;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_WORLD_MATRIX) == 0)
        return RenderState::WORLD_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_VIEW_MATRIX) == 0)
        return RenderState::VIEW_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_PROJECTION_MATRIX) == 0)
        return RenderState::PROJECTION_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_WORLD_VIEW_MATRIX) == 0)
        return RenderState::WORLD_VIEW_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_VIEW_PROJECTION_MATRIX) == 0)
        return RenderState::VIEW_PROJECTION_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_WORLD_VIEW_PROJECTION_MATRIX) == 0)
        return RenderState::WORLD_VIEW_PROJECTION_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_MATRIX) == 0)
        return RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX) == 0)
        return RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_CAMERA_WORLD_POSITION) == 0)
        return RenderState::CAMERA_WORLD_POSITION;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_CAMERA_VIEW_POSITION) == 0)
        return RenderState::CAMERA_VIEW_POSITION;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_MATRIX_PALETTE) == 0)
        return RenderState::MATRIX_PALETTE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration RenderState::AutoBinding.", s);
    return RenderState::NONE;
}

const char* lua_stringFromEnum_RenderStateAutoBinding(RenderState::AutoBinding e)
{
    if (e == RenderState::NONE)
        return luaEnumString_RenderStateAutoBinding_NONE;
    if (e == RenderState::WORLD_MATRIX)
        return luaEnumString_RenderStateAutoBinding_WORLD_MATRIX;
    if (e == RenderState::VIEW_MATRIX)
        return luaEnumString_RenderStateAutoBinding_VIEW_MATRIX;
    if (e == RenderState::PROJECTION_MATRIX)
        return luaEnumString_RenderStateAutoBinding_PROJECTION_MATRIX;
    if (e == RenderState::WORLD_VIEW_MATRIX)
        return luaEnumString_RenderStateAutoBinding_WORLD_VIEW_MATRIX;
    if (e == RenderState::VIEW_PROJECTION_MATRIX)
        return luaEnumString_RenderStateAutoBinding_VIEW_PROJECTION_MATRIX;
    if (e == RenderState::WORLD_VIEW_PROJECTION_MATRIX)
        return luaEnumString_RenderStateAutoBinding_WORLD_VIEW_PROJECTION_MATRIX;
    if (e == RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX)
        return luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_MATRIX;
    if (e == RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
        return luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    if (e == RenderState::CAMERA_WORLD_POSITION)
        return luaEnumString_RenderStateAutoBinding_CAMERA_WORLD_POSITION;
    if (e == RenderState::CAMERA_VIEW_POSITION)
        return luaEnumString_RenderStateAutoBinding_CAMERA_VIEW_POSITION;
    if (e == RenderState::MATRIX_PALETTE)
        return luaEnumString_RenderStateAutoBinding_MATRIX_PALETTE;
    GP_ERROR("Invalid enumeration value '%d' for enumeration RenderState::AutoBinding.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_RenderStateBlend_BLEND_ZERO = "BLEND_ZERO";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE = "BLEND_ONE";
static const char* luaEnumString_RenderStateBlend_BLEND_SRC_COLOR = "BLEND_SRC_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEN_ONE_MINUS_SRC_COLOR = "BLEN_ONE_MINUS_SRC_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_DST_COLOR = "BLEND_DST_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_COLOR = "BLEND_ONE_MINUS_DST_COLOR";
static const char* luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA = "BLEND_SRC_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_ALPHA = "BLEND_ONE_MINUS_SRC_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_DST_ALPHA = "BLEND_DST_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_ALPHA = "BLEND_ONE_MINUS_DST_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_CONSTANT_ALPHA = "BLEND_CONSTANT_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_CONSTANT_ALPHA = "BLEND_ONE_MINUS_CONSTANT_ALPHA";
static const char* luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA_SATURATE = "BLEND_SRC_ALPHA_SATURATE";

RenderState::Blend lua_enumFromString_RenderStateBlend(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ZERO) == 0)
        return RenderState::BLEND_ZERO;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE) == 0)
        return RenderState::BLEND_ONE;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_SRC_COLOR) == 0)
        return RenderState::BLEND_SRC_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEN_ONE_MINUS_SRC_COLOR) == 0)
        return RenderState::BLEN_ONE_MINUS_SRC_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_DST_COLOR) == 0)
        return RenderState::BLEND_DST_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_COLOR) == 0)
        return RenderState::BLEND_ONE_MINUS_DST_COLOR;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA) == 0)
        return RenderState::BLEND_SRC_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_ALPHA) == 0)
        return RenderState::BLEND_ONE_MINUS_SRC_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_DST_ALPHA) == 0)
        return RenderState::BLEND_DST_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_ALPHA) == 0)
        return RenderState::BLEND_ONE_MINUS_DST_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_CONSTANT_ALPHA) == 0)
        return RenderState::BLEND_CONSTANT_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_CONSTANT_ALPHA) == 0)
        return RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA;
    if (strcmp(s, luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA_SATURATE) == 0)
        return RenderState::BLEND_SRC_ALPHA_SATURATE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration RenderState::Blend.", s);
    return RenderState::BLEND_ZERO;
}

const char* lua_stringFromEnum_RenderStateBlend(RenderState::Blend e)
{
    if (e == RenderState::BLEND_ZERO)
        return luaEnumString_RenderStateBlend_BLEND_ZERO;
    if (e == RenderState::BLEND_ONE)
        return luaEnumString_RenderStateBlend_BLEND_ONE;
    if (e == RenderState::BLEND_SRC_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_SRC_COLOR;
    if (e == RenderState::BLEN_ONE_MINUS_SRC_COLOR)
        return luaEnumString_RenderStateBlend_BLEN_ONE_MINUS_SRC_COLOR;
    if (e == RenderState::BLEND_DST_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_DST_COLOR;
    if (e == RenderState::BLEND_ONE_MINUS_DST_COLOR)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_COLOR;
    if (e == RenderState::BLEND_SRC_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA;
    if (e == RenderState::BLEND_ONE_MINUS_SRC_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_SRC_ALPHA;
    if (e == RenderState::BLEND_DST_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_DST_ALPHA;
    if (e == RenderState::BLEND_ONE_MINUS_DST_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_DST_ALPHA;
    if (e == RenderState::BLEND_CONSTANT_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_CONSTANT_ALPHA;
    if (e == RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA)
        return luaEnumString_RenderStateBlend_BLEND_ONE_MINUS_CONSTANT_ALPHA;
    if (e == RenderState::BLEND_SRC_ALPHA_SATURATE)
        return luaEnumString_RenderStateBlend_BLEND_SRC_ALPHA_SATURATE;
    GP_ERROR("Invalid enumeration value '%d' for enumeration RenderState::Blend.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_SceneDebugFlags_DEBUG_BOXES = "DEBUG_BOXES";
static const char* luaEnumString_SceneDebugFlags_DEBUG_SPHERES = "DEBUG_SPHERES";

Scene::DebugFlags lua_enumFromString_SceneDebugFlags(const char* s)
{
    if (strcmp(s, luaEnumString_SceneDebugFlags_DEBUG_BOXES) == 0)
        return Scene::DEBUG_BOXES;
    if (strcmp(s, luaEnumString_SceneDebugFlags_DEBUG_SPHERES) == 0)
        return Scene::DEBUG_SPHERES;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Scene::DebugFlags.", s);
    return Scene::DEBUG_BOXES;
}

const char* lua_stringFromEnum_SceneDebugFlags(Scene::DebugFlags e)
{
    if (e == Scene::DEBUG_BOXES)
        return luaEnumString_SceneDebugFlags_DEBUG_BOXES;
    if (e == Scene::DEBUG_SPHERES)
        return luaEnumString_SceneDebugFlags_DEBUG_SPHERES;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Scene::DebugFlags.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_TextureFilter_NEAREST = "NEAREST";
static const char* luaEnumString_TextureFilter_LINEAR = "LINEAR";
static const char* luaEnumString_TextureFilter_NEAREST_MIPMAP_NEAREST = "NEAREST_MIPMAP_NEAREST";
static const char* luaEnumString_TextureFilter_LINEAR_MIPMAP_NEAREST = "LINEAR_MIPMAP_NEAREST";
static const char* luaEnumString_TextureFilter_NEAREST_MIPMAP_LINEAR = "NEAREST_MIPMAP_LINEAR";
static const char* luaEnumString_TextureFilter_LINEAR_MIPMAP_LINEAR = "LINEAR_MIPMAP_LINEAR";

Texture::Filter lua_enumFromString_TextureFilter(const char* s)
{
    if (strcmp(s, luaEnumString_TextureFilter_NEAREST) == 0)
        return Texture::NEAREST;
    if (strcmp(s, luaEnumString_TextureFilter_LINEAR) == 0)
        return Texture::LINEAR;
    if (strcmp(s, luaEnumString_TextureFilter_NEAREST_MIPMAP_NEAREST) == 0)
        return Texture::NEAREST_MIPMAP_NEAREST;
    if (strcmp(s, luaEnumString_TextureFilter_LINEAR_MIPMAP_NEAREST) == 0)
        return Texture::LINEAR_MIPMAP_NEAREST;
    if (strcmp(s, luaEnumString_TextureFilter_NEAREST_MIPMAP_LINEAR) == 0)
        return Texture::NEAREST_MIPMAP_LINEAR;
    if (strcmp(s, luaEnumString_TextureFilter_LINEAR_MIPMAP_LINEAR) == 0)
        return Texture::LINEAR_MIPMAP_LINEAR;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Texture::Filter.", s);
    return Texture::NEAREST;
}

const char* lua_stringFromEnum_TextureFilter(Texture::Filter e)
{
    if (e == Texture::NEAREST)
        return luaEnumString_TextureFilter_NEAREST;
    if (e == Texture::LINEAR)
        return luaEnumString_TextureFilter_LINEAR;
    if (e == Texture::NEAREST_MIPMAP_NEAREST)
        return luaEnumString_TextureFilter_NEAREST_MIPMAP_NEAREST;
    if (e == Texture::LINEAR_MIPMAP_NEAREST)
        return luaEnumString_TextureFilter_LINEAR_MIPMAP_NEAREST;
    if (e == Texture::NEAREST_MIPMAP_LINEAR)
        return luaEnumString_TextureFilter_NEAREST_MIPMAP_LINEAR;
    if (e == Texture::LINEAR_MIPMAP_LINEAR)
        return luaEnumString_TextureFilter_LINEAR_MIPMAP_LINEAR;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Texture::Filter.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_TextureFormat_RGB = "RGB";
static const char* luaEnumString_TextureFormat_RGBA = "RGBA";
static const char* luaEnumString_TextureFormat_ALPHA = "ALPHA";

Texture::Format lua_enumFromString_TextureFormat(const char* s)
{
    if (strcmp(s, luaEnumString_TextureFormat_RGB) == 0)
        return Texture::RGB;
    if (strcmp(s, luaEnumString_TextureFormat_RGBA) == 0)
        return Texture::RGBA;
    if (strcmp(s, luaEnumString_TextureFormat_ALPHA) == 0)
        return Texture::ALPHA;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Texture::Format.", s);
    return Texture::RGB;
}

const char* lua_stringFromEnum_TextureFormat(Texture::Format e)
{
    if (e == Texture::RGB)
        return luaEnumString_TextureFormat_RGB;
    if (e == Texture::RGBA)
        return luaEnumString_TextureFormat_RGBA;
    if (e == Texture::ALPHA)
        return luaEnumString_TextureFormat_ALPHA;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Texture::Format.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_TextureWrap_REPEAT = "REPEAT";
static const char* luaEnumString_TextureWrap_CLAMP = "CLAMP";

Texture::Wrap lua_enumFromString_TextureWrap(const char* s)
{
    if (strcmp(s, luaEnumString_TextureWrap_REPEAT) == 0)
        return Texture::REPEAT;
    if (strcmp(s, luaEnumString_TextureWrap_CLAMP) == 0)
        return Texture::CLAMP;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Texture::Wrap.", s);
    return Texture::REPEAT;
}

const char* lua_stringFromEnum_TextureWrap(Texture::Wrap e)
{
    if (e == Texture::REPEAT)
        return luaEnumString_TextureWrap_REPEAT;
    if (e == Texture::CLAMP)
        return luaEnumString_TextureWrap_CLAMP;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Texture::Wrap.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_TouchTouchEvent_TOUCH_PRESS = "TOUCH_PRESS";
static const char* luaEnumString_TouchTouchEvent_TOUCH_RELEASE = "TOUCH_RELEASE";
static const char* luaEnumString_TouchTouchEvent_TOUCH_MOVE = "TOUCH_MOVE";

Touch::TouchEvent lua_enumFromString_TouchTouchEvent(const char* s)
{
    if (strcmp(s, luaEnumString_TouchTouchEvent_TOUCH_PRESS) == 0)
        return Touch::TOUCH_PRESS;
    if (strcmp(s, luaEnumString_TouchTouchEvent_TOUCH_RELEASE) == 0)
        return Touch::TOUCH_RELEASE;
    if (strcmp(s, luaEnumString_TouchTouchEvent_TOUCH_MOVE) == 0)
        return Touch::TOUCH_MOVE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Touch::TouchEvent.", s);
    return Touch::TOUCH_PRESS;
}

const char* lua_stringFromEnum_TouchTouchEvent(Touch::TouchEvent e)
{
    if (e == Touch::TOUCH_PRESS)
        return luaEnumString_TouchTouchEvent_TOUCH_PRESS;
    if (e == Touch::TOUCH_RELEASE)
        return luaEnumString_TouchTouchEvent_TOUCH_RELEASE;
    if (e == Touch::TOUCH_MOVE)
        return luaEnumString_TouchTouchEvent_TOUCH_MOVE;
    GP_ERROR("Invalid enumeration value '%d' for enumeration Touch::TouchEvent.", e);
    return enumStringEmpty;
}

static const char* luaEnumString_VertexFormatUsage_POSITION = "POSITION";
static const char* luaEnumString_VertexFormatUsage_NORMAL = "NORMAL";
static const char* luaEnumString_VertexFormatUsage_COLOR = "COLOR";
static const char* luaEnumString_VertexFormatUsage_TANGENT = "TANGENT";
static const char* luaEnumString_VertexFormatUsage_BINORMAL = "BINORMAL";
static const char* luaEnumString_VertexFormatUsage_BLENDWEIGHTS = "BLENDWEIGHTS";
static const char* luaEnumString_VertexFormatUsage_BLENDINDICES = "BLENDINDICES";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD0 = "TEXCOORD0";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD1 = "TEXCOORD1";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD2 = "TEXCOORD2";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD3 = "TEXCOORD3";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD4 = "TEXCOORD4";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD5 = "TEXCOORD5";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD6 = "TEXCOORD6";
static const char* luaEnumString_VertexFormatUsage_TEXCOORD7 = "TEXCOORD7";

VertexFormat::Usage lua_enumFromString_VertexFormatUsage(const char* s)
{
    if (strcmp(s, luaEnumString_VertexFormatUsage_POSITION) == 0)
        return VertexFormat::POSITION;
    if (strcmp(s, luaEnumString_VertexFormatUsage_NORMAL) == 0)
        return VertexFormat::NORMAL;
    if (strcmp(s, luaEnumString_VertexFormatUsage_COLOR) == 0)
        return VertexFormat::COLOR;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TANGENT) == 0)
        return VertexFormat::TANGENT;
    if (strcmp(s, luaEnumString_VertexFormatUsage_BINORMAL) == 0)
        return VertexFormat::BINORMAL;
    if (strcmp(s, luaEnumString_VertexFormatUsage_BLENDWEIGHTS) == 0)
        return VertexFormat::BLENDWEIGHTS;
    if (strcmp(s, luaEnumString_VertexFormatUsage_BLENDINDICES) == 0)
        return VertexFormat::BLENDINDICES;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD0) == 0)
        return VertexFormat::TEXCOORD0;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD1) == 0)
        return VertexFormat::TEXCOORD1;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD2) == 0)
        return VertexFormat::TEXCOORD2;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD3) == 0)
        return VertexFormat::TEXCOORD3;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD4) == 0)
        return VertexFormat::TEXCOORD4;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD5) == 0)
        return VertexFormat::TEXCOORD5;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD6) == 0)
        return VertexFormat::TEXCOORD6;
    if (strcmp(s, luaEnumString_VertexFormatUsage_TEXCOORD7) == 0)
        return VertexFormat::TEXCOORD7;
    GP_ERROR("Invalid enumeration value '%s' for enumeration VertexFormat::Usage.", s);
    return VertexFormat::POSITION;
}

const char* lua_stringFromEnum_VertexFormatUsage(VertexFormat::Usage e)
{
    if (e == VertexFormat::POSITION)
        return luaEnumString_VertexFormatUsage_POSITION;
    if (e == VertexFormat::NORMAL)
        return luaEnumString_VertexFormatUsage_NORMAL;
    if (e == VertexFormat::COLOR)
        return luaEnumString_VertexFormatUsage_COLOR;
    if (e == VertexFormat::TANGENT)
        return luaEnumString_VertexFormatUsage_TANGENT;
    if (e == VertexFormat::BINORMAL)
        return luaEnumString_VertexFormatUsage_BINORMAL;
    if (e == VertexFormat::BLENDWEIGHTS)
        return luaEnumString_VertexFormatUsage_BLENDWEIGHTS;
    if (e == VertexFormat::BLENDINDICES)
        return luaEnumString_VertexFormatUsage_BLENDINDICES;
    if (e == VertexFormat::TEXCOORD0)
        return luaEnumString_VertexFormatUsage_TEXCOORD0;
    if (e == VertexFormat::TEXCOORD1)
        return luaEnumString_VertexFormatUsage_TEXCOORD1;
    if (e == VertexFormat::TEXCOORD2)
        return luaEnumString_VertexFormatUsage_TEXCOORD2;
    if (e == VertexFormat::TEXCOORD3)
        return luaEnumString_VertexFormatUsage_TEXCOORD3;
    if (e == VertexFormat::TEXCOORD4)
        return luaEnumString_VertexFormatUsage_TEXCOORD4;
    if (e == VertexFormat::TEXCOORD5)
        return luaEnumString_VertexFormatUsage_TEXCOORD5;
    if (e == VertexFormat::TEXCOORD6)
        return luaEnumString_VertexFormatUsage_TEXCOORD6;
    if (e == VertexFormat::TEXCOORD7)
        return luaEnumString_VertexFormatUsage_TEXCOORD7;
    GP_ERROR("Invalid enumeration value '%d' for enumeration VertexFormat::Usage.", e);
    return enumStringEmpty;
}

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
