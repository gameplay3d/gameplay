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
            if (lua_type(state, 1) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

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

AnimationClip::Listener::EventType lua_enumFromString_AnimationClipListenerEventType(const char* s)
{
    if (strcmp(s, "DEFAULT") == 0)
        return AnimationClip::Listener::DEFAULT;
    if (strcmp(s, "BEGIN") == 0)
        return AnimationClip::Listener::BEGIN;
    if (strcmp(s, "END") == 0)
        return AnimationClip::Listener::END;
    GP_ERROR("Invalid enumeration value '%s' for enumeration AnimationClip::Listener::EventType.", s);
    return AnimationClip::Listener::DEFAULT;
}

std::string lua_stringFromEnum_AnimationClipListenerEventType(AnimationClip::Listener::EventType e)
{
    if (e == AnimationClip::Listener::DEFAULT)
        return std::string("DEFAULT");
    if (e == AnimationClip::Listener::BEGIN)
        return std::string("BEGIN");
    if (e == AnimationClip::Listener::END)
        return std::string("END");
    GP_ERROR("Invalid enumeration value '%d' for enumeration AnimationClip::Listener::EventType.", e);
    return std::string();
}

AudioSource::State lua_enumFromString_AudioSourceState(const char* s)
{
    if (strcmp(s, "INITIAL") == 0)
        return AudioSource::INITIAL;
    if (strcmp(s, "PLAYING") == 0)
        return AudioSource::PLAYING;
    if (strcmp(s, "PAUSED") == 0)
        return AudioSource::PAUSED;
    if (strcmp(s, "STOPPED") == 0)
        return AudioSource::STOPPED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration AudioSource::State.", s);
    return AudioSource::INITIAL;
}

std::string lua_stringFromEnum_AudioSourceState(AudioSource::State e)
{
    if (e == AudioSource::INITIAL)
        return std::string("INITIAL");
    if (e == AudioSource::PLAYING)
        return std::string("PLAYING");
    if (e == AudioSource::PAUSED)
        return std::string("PAUSED");
    if (e == AudioSource::STOPPED)
        return std::string("STOPPED");
    GP_ERROR("Invalid enumeration value '%d' for enumeration AudioSource::State.", e);
    return std::string();
}

Camera::Type lua_enumFromString_CameraType(const char* s)
{
    if (strcmp(s, "PERSPECTIVE") == 0)
        return Camera::PERSPECTIVE;
    if (strcmp(s, "ORTHOGRAPHIC") == 0)
        return Camera::ORTHOGRAPHIC;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Camera::Type.", s);
    return Camera::PERSPECTIVE;
}

std::string lua_stringFromEnum_CameraType(Camera::Type e)
{
    if (e == Camera::PERSPECTIVE)
        return std::string("PERSPECTIVE");
    if (e == Camera::ORTHOGRAPHIC)
        return std::string("ORTHOGRAPHIC");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Camera::Type.", e);
    return std::string();
}

Container::Scroll lua_enumFromString_ContainerScroll(const char* s)
{
    if (strcmp(s, "SCROLL_NONE") == 0)
        return Container::SCROLL_NONE;
    if (strcmp(s, "SCROLL_HORIZONTAL") == 0)
        return Container::SCROLL_HORIZONTAL;
    if (strcmp(s, "SCROLL_VERTICAL") == 0)
        return Container::SCROLL_VERTICAL;
    if (strcmp(s, "SCROLL_BOTH") == 0)
        return Container::SCROLL_BOTH;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Container::Scroll.", s);
    return Container::SCROLL_NONE;
}

std::string lua_stringFromEnum_ContainerScroll(Container::Scroll e)
{
    if (e == Container::SCROLL_NONE)
        return std::string("SCROLL_NONE");
    if (e == Container::SCROLL_HORIZONTAL)
        return std::string("SCROLL_HORIZONTAL");
    if (e == Container::SCROLL_VERTICAL)
        return std::string("SCROLL_VERTICAL");
    if (e == Container::SCROLL_BOTH)
        return std::string("SCROLL_BOTH");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Container::Scroll.", e);
    return std::string();
}

Control::Alignment lua_enumFromString_ControlAlignment(const char* s)
{
    if (strcmp(s, "ALIGN_LEFT") == 0)
        return Control::ALIGN_LEFT;
    if (strcmp(s, "ALIGN_HCENTER") == 0)
        return Control::ALIGN_HCENTER;
    if (strcmp(s, "ALIGN_RIGHT") == 0)
        return Control::ALIGN_RIGHT;
    if (strcmp(s, "ALIGN_TOP") == 0)
        return Control::ALIGN_TOP;
    if (strcmp(s, "ALIGN_VCENTER") == 0)
        return Control::ALIGN_VCENTER;
    if (strcmp(s, "ALIGN_BOTTOM") == 0)
        return Control::ALIGN_BOTTOM;
    if (strcmp(s, "ALIGN_TOP_LEFT") == 0)
        return Control::ALIGN_TOP_LEFT;
    if (strcmp(s, "ALIGN_VCENTER_LEFT") == 0)
        return Control::ALIGN_VCENTER_LEFT;
    if (strcmp(s, "ALIGN_BOTTOM_LEFT") == 0)
        return Control::ALIGN_BOTTOM_LEFT;
    if (strcmp(s, "ALIGN_TOP_HCENTER") == 0)
        return Control::ALIGN_TOP_HCENTER;
    if (strcmp(s, "ALIGN_VCENTER_HCENTER") == 0)
        return Control::ALIGN_VCENTER_HCENTER;
    if (strcmp(s, "ALIGN_BOTTOM_HCENTER") == 0)
        return Control::ALIGN_BOTTOM_HCENTER;
    if (strcmp(s, "ALIGN_TOP_RIGHT") == 0)
        return Control::ALIGN_TOP_RIGHT;
    if (strcmp(s, "ALIGN_VCENTER_RIGHT") == 0)
        return Control::ALIGN_VCENTER_RIGHT;
    if (strcmp(s, "ALIGN_BOTTOM_RIGHT") == 0)
        return Control::ALIGN_BOTTOM_RIGHT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Control::Alignment.", s);
    return Control::ALIGN_LEFT;
}

std::string lua_stringFromEnum_ControlAlignment(Control::Alignment e)
{
    if (e == Control::ALIGN_LEFT)
        return std::string("ALIGN_LEFT");
    if (e == Control::ALIGN_HCENTER)
        return std::string("ALIGN_HCENTER");
    if (e == Control::ALIGN_RIGHT)
        return std::string("ALIGN_RIGHT");
    if (e == Control::ALIGN_TOP)
        return std::string("ALIGN_TOP");
    if (e == Control::ALIGN_VCENTER)
        return std::string("ALIGN_VCENTER");
    if (e == Control::ALIGN_BOTTOM)
        return std::string("ALIGN_BOTTOM");
    if (e == Control::ALIGN_TOP_LEFT)
        return std::string("ALIGN_TOP_LEFT");
    if (e == Control::ALIGN_VCENTER_LEFT)
        return std::string("ALIGN_VCENTER_LEFT");
    if (e == Control::ALIGN_BOTTOM_LEFT)
        return std::string("ALIGN_BOTTOM_LEFT");
    if (e == Control::ALIGN_TOP_HCENTER)
        return std::string("ALIGN_TOP_HCENTER");
    if (e == Control::ALIGN_VCENTER_HCENTER)
        return std::string("ALIGN_VCENTER_HCENTER");
    if (e == Control::ALIGN_BOTTOM_HCENTER)
        return std::string("ALIGN_BOTTOM_HCENTER");
    if (e == Control::ALIGN_TOP_RIGHT)
        return std::string("ALIGN_TOP_RIGHT");
    if (e == Control::ALIGN_VCENTER_RIGHT)
        return std::string("ALIGN_VCENTER_RIGHT");
    if (e == Control::ALIGN_BOTTOM_RIGHT)
        return std::string("ALIGN_BOTTOM_RIGHT");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Control::Alignment.", e);
    return std::string();
}

Control::Listener::EventType lua_enumFromString_ControlListenerEventType(const char* s)
{
    if (strcmp(s, "PRESS") == 0)
        return Control::Listener::PRESS;
    if (strcmp(s, "RELEASE") == 0)
        return Control::Listener::RELEASE;
    if (strcmp(s, "CLICK") == 0)
        return Control::Listener::CLICK;
    if (strcmp(s, "VALUE_CHANGED") == 0)
        return Control::Listener::VALUE_CHANGED;
    if (strcmp(s, "TEXT_CHANGED") == 0)
        return Control::Listener::TEXT_CHANGED;
    if (strcmp(s, "MIDDLE_CLICK") == 0)
        return Control::Listener::MIDDLE_CLICK;
    if (strcmp(s, "RIGHT_CLICK") == 0)
        return Control::Listener::RIGHT_CLICK;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Control::Listener::EventType.", s);
    return Control::Listener::PRESS;
}

std::string lua_stringFromEnum_ControlListenerEventType(Control::Listener::EventType e)
{
    if (e == Control::Listener::PRESS)
        return std::string("PRESS");
    if (e == Control::Listener::RELEASE)
        return std::string("RELEASE");
    if (e == Control::Listener::CLICK)
        return std::string("CLICK");
    if (e == Control::Listener::VALUE_CHANGED)
        return std::string("VALUE_CHANGED");
    if (e == Control::Listener::TEXT_CHANGED)
        return std::string("TEXT_CHANGED");
    if (e == Control::Listener::MIDDLE_CLICK)
        return std::string("MIDDLE_CLICK");
    if (e == Control::Listener::RIGHT_CLICK)
        return std::string("RIGHT_CLICK");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Control::Listener::EventType.", e);
    return std::string();
}

Control::State lua_enumFromString_ControlState(const char* s)
{
    if (strcmp(s, "NORMAL") == 0)
        return Control::NORMAL;
    if (strcmp(s, "FOCUS") == 0)
        return Control::FOCUS;
    if (strcmp(s, "ACTIVE") == 0)
        return Control::ACTIVE;
    if (strcmp(s, "DISABLED") == 0)
        return Control::DISABLED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Control::State.", s);
    return Control::NORMAL;
}

std::string lua_stringFromEnum_ControlState(Control::State e)
{
    if (e == Control::NORMAL)
        return std::string("NORMAL");
    if (e == Control::FOCUS)
        return std::string("FOCUS");
    if (e == Control::ACTIVE)
        return std::string("ACTIVE");
    if (e == Control::DISABLED)
        return std::string("DISABLED");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Control::State.", e);
    return std::string();
}

Curve::InterpolationType lua_enumFromString_CurveInterpolationType(const char* s)
{
    if (strcmp(s, "BEZIER") == 0)
        return Curve::BEZIER;
    if (strcmp(s, "BSPLINE") == 0)
        return Curve::BSPLINE;
    if (strcmp(s, "FLAT") == 0)
        return Curve::FLAT;
    if (strcmp(s, "HERMITE") == 0)
        return Curve::HERMITE;
    if (strcmp(s, "LINEAR") == 0)
        return Curve::LINEAR;
    if (strcmp(s, "SMOOTH") == 0)
        return Curve::SMOOTH;
    if (strcmp(s, "STEP") == 0)
        return Curve::STEP;
    if (strcmp(s, "QUADRATIC_IN") == 0)
        return Curve::QUADRATIC_IN;
    if (strcmp(s, "QUADRATIC_OUT") == 0)
        return Curve::QUADRATIC_OUT;
    if (strcmp(s, "QUADRATIC_IN_OUT") == 0)
        return Curve::QUADRATIC_IN_OUT;
    if (strcmp(s, "QUADRATIC_OUT_IN") == 0)
        return Curve::QUADRATIC_OUT_IN;
    if (strcmp(s, "CUBIC_IN") == 0)
        return Curve::CUBIC_IN;
    if (strcmp(s, "CUBIC_OUT") == 0)
        return Curve::CUBIC_OUT;
    if (strcmp(s, "CUBIC_IN_OUT") == 0)
        return Curve::CUBIC_IN_OUT;
    if (strcmp(s, "CUBIC_OUT_IN") == 0)
        return Curve::CUBIC_OUT_IN;
    if (strcmp(s, "QUARTIC_IN") == 0)
        return Curve::QUARTIC_IN;
    if (strcmp(s, "QUARTIC_OUT") == 0)
        return Curve::QUARTIC_OUT;
    if (strcmp(s, "QUARTIC_IN_OUT") == 0)
        return Curve::QUARTIC_IN_OUT;
    if (strcmp(s, "QUARTIC_OUT_IN") == 0)
        return Curve::QUARTIC_OUT_IN;
    if (strcmp(s, "QUINTIC_IN") == 0)
        return Curve::QUINTIC_IN;
    if (strcmp(s, "QUINTIC_OUT") == 0)
        return Curve::QUINTIC_OUT;
    if (strcmp(s, "QUINTIC_IN_OUT") == 0)
        return Curve::QUINTIC_IN_OUT;
    if (strcmp(s, "QUINTIC_OUT_IN") == 0)
        return Curve::QUINTIC_OUT_IN;
    if (strcmp(s, "SINE_IN") == 0)
        return Curve::SINE_IN;
    if (strcmp(s, "SINE_OUT") == 0)
        return Curve::SINE_OUT;
    if (strcmp(s, "SINE_IN_OUT") == 0)
        return Curve::SINE_IN_OUT;
    if (strcmp(s, "SINE_OUT_IN") == 0)
        return Curve::SINE_OUT_IN;
    if (strcmp(s, "EXPONENTIAL_IN") == 0)
        return Curve::EXPONENTIAL_IN;
    if (strcmp(s, "EXPONENTIAL_OUT") == 0)
        return Curve::EXPONENTIAL_OUT;
    if (strcmp(s, "EXPONENTIAL_IN_OUT") == 0)
        return Curve::EXPONENTIAL_IN_OUT;
    if (strcmp(s, "EXPONENTIAL_OUT_IN") == 0)
        return Curve::EXPONENTIAL_OUT_IN;
    if (strcmp(s, "CIRCULAR_IN") == 0)
        return Curve::CIRCULAR_IN;
    if (strcmp(s, "CIRCULAR_OUT") == 0)
        return Curve::CIRCULAR_OUT;
    if (strcmp(s, "CIRCULAR_IN_OUT") == 0)
        return Curve::CIRCULAR_IN_OUT;
    if (strcmp(s, "CIRCULAR_OUT_IN") == 0)
        return Curve::CIRCULAR_OUT_IN;
    if (strcmp(s, "ELASTIC_IN") == 0)
        return Curve::ELASTIC_IN;
    if (strcmp(s, "ELASTIC_OUT") == 0)
        return Curve::ELASTIC_OUT;
    if (strcmp(s, "ELASTIC_IN_OUT") == 0)
        return Curve::ELASTIC_IN_OUT;
    if (strcmp(s, "ELASTIC_OUT_IN") == 0)
        return Curve::ELASTIC_OUT_IN;
    if (strcmp(s, "OVERSHOOT_IN") == 0)
        return Curve::OVERSHOOT_IN;
    if (strcmp(s, "OVERSHOOT_OUT") == 0)
        return Curve::OVERSHOOT_OUT;
    if (strcmp(s, "OVERSHOOT_IN_OUT") == 0)
        return Curve::OVERSHOOT_IN_OUT;
    if (strcmp(s, "OVERSHOOT_OUT_IN") == 0)
        return Curve::OVERSHOOT_OUT_IN;
    if (strcmp(s, "BOUNCE_IN") == 0)
        return Curve::BOUNCE_IN;
    if (strcmp(s, "BOUNCE_OUT") == 0)
        return Curve::BOUNCE_OUT;
    if (strcmp(s, "BOUNCE_IN_OUT") == 0)
        return Curve::BOUNCE_IN_OUT;
    if (strcmp(s, "BOUNCE_OUT_IN") == 0)
        return Curve::BOUNCE_OUT_IN;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Curve::InterpolationType.", s);
    return Curve::BEZIER;
}

std::string lua_stringFromEnum_CurveInterpolationType(Curve::InterpolationType e)
{
    if (e == Curve::BEZIER)
        return std::string("BEZIER");
    if (e == Curve::BSPLINE)
        return std::string("BSPLINE");
    if (e == Curve::FLAT)
        return std::string("FLAT");
    if (e == Curve::HERMITE)
        return std::string("HERMITE");
    if (e == Curve::LINEAR)
        return std::string("LINEAR");
    if (e == Curve::SMOOTH)
        return std::string("SMOOTH");
    if (e == Curve::STEP)
        return std::string("STEP");
    if (e == Curve::QUADRATIC_IN)
        return std::string("QUADRATIC_IN");
    if (e == Curve::QUADRATIC_OUT)
        return std::string("QUADRATIC_OUT");
    if (e == Curve::QUADRATIC_IN_OUT)
        return std::string("QUADRATIC_IN_OUT");
    if (e == Curve::QUADRATIC_OUT_IN)
        return std::string("QUADRATIC_OUT_IN");
    if (e == Curve::CUBIC_IN)
        return std::string("CUBIC_IN");
    if (e == Curve::CUBIC_OUT)
        return std::string("CUBIC_OUT");
    if (e == Curve::CUBIC_IN_OUT)
        return std::string("CUBIC_IN_OUT");
    if (e == Curve::CUBIC_OUT_IN)
        return std::string("CUBIC_OUT_IN");
    if (e == Curve::QUARTIC_IN)
        return std::string("QUARTIC_IN");
    if (e == Curve::QUARTIC_OUT)
        return std::string("QUARTIC_OUT");
    if (e == Curve::QUARTIC_IN_OUT)
        return std::string("QUARTIC_IN_OUT");
    if (e == Curve::QUARTIC_OUT_IN)
        return std::string("QUARTIC_OUT_IN");
    if (e == Curve::QUINTIC_IN)
        return std::string("QUINTIC_IN");
    if (e == Curve::QUINTIC_OUT)
        return std::string("QUINTIC_OUT");
    if (e == Curve::QUINTIC_IN_OUT)
        return std::string("QUINTIC_IN_OUT");
    if (e == Curve::QUINTIC_OUT_IN)
        return std::string("QUINTIC_OUT_IN");
    if (e == Curve::SINE_IN)
        return std::string("SINE_IN");
    if (e == Curve::SINE_OUT)
        return std::string("SINE_OUT");
    if (e == Curve::SINE_IN_OUT)
        return std::string("SINE_IN_OUT");
    if (e == Curve::SINE_OUT_IN)
        return std::string("SINE_OUT_IN");
    if (e == Curve::EXPONENTIAL_IN)
        return std::string("EXPONENTIAL_IN");
    if (e == Curve::EXPONENTIAL_OUT)
        return std::string("EXPONENTIAL_OUT");
    if (e == Curve::EXPONENTIAL_IN_OUT)
        return std::string("EXPONENTIAL_IN_OUT");
    if (e == Curve::EXPONENTIAL_OUT_IN)
        return std::string("EXPONENTIAL_OUT_IN");
    if (e == Curve::CIRCULAR_IN)
        return std::string("CIRCULAR_IN");
    if (e == Curve::CIRCULAR_OUT)
        return std::string("CIRCULAR_OUT");
    if (e == Curve::CIRCULAR_IN_OUT)
        return std::string("CIRCULAR_IN_OUT");
    if (e == Curve::CIRCULAR_OUT_IN)
        return std::string("CIRCULAR_OUT_IN");
    if (e == Curve::ELASTIC_IN)
        return std::string("ELASTIC_IN");
    if (e == Curve::ELASTIC_OUT)
        return std::string("ELASTIC_OUT");
    if (e == Curve::ELASTIC_IN_OUT)
        return std::string("ELASTIC_IN_OUT");
    if (e == Curve::ELASTIC_OUT_IN)
        return std::string("ELASTIC_OUT_IN");
    if (e == Curve::OVERSHOOT_IN)
        return std::string("OVERSHOOT_IN");
    if (e == Curve::OVERSHOOT_OUT)
        return std::string("OVERSHOOT_OUT");
    if (e == Curve::OVERSHOOT_IN_OUT)
        return std::string("OVERSHOOT_IN_OUT");
    if (e == Curve::OVERSHOOT_OUT_IN)
        return std::string("OVERSHOOT_OUT_IN");
    if (e == Curve::BOUNCE_IN)
        return std::string("BOUNCE_IN");
    if (e == Curve::BOUNCE_OUT)
        return std::string("BOUNCE_OUT");
    if (e == Curve::BOUNCE_IN_OUT)
        return std::string("BOUNCE_IN_OUT");
    if (e == Curve::BOUNCE_OUT_IN)
        return std::string("BOUNCE_OUT_IN");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Curve::InterpolationType.", e);
    return std::string();
}

DepthStencilTarget::Format lua_enumFromString_DepthStencilTargetFormat(const char* s)
{
    if (strcmp(s, "DEPTH") == 0)
        return DepthStencilTarget::DEPTH;
    if (strcmp(s, "DEPTH_STENCIL") == 0)
        return DepthStencilTarget::DEPTH_STENCIL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration DepthStencilTarget::Format.", s);
    return DepthStencilTarget::DEPTH;
}

std::string lua_stringFromEnum_DepthStencilTargetFormat(DepthStencilTarget::Format e)
{
    if (e == DepthStencilTarget::DEPTH)
        return std::string("DEPTH");
    if (e == DepthStencilTarget::DEPTH_STENCIL)
        return std::string("DEPTH_STENCIL");
    GP_ERROR("Invalid enumeration value '%d' for enumeration DepthStencilTarget::Format.", e);
    return std::string();
}

Font::Justify lua_enumFromString_FontJustify(const char* s)
{
    if (strcmp(s, "ALIGN_LEFT") == 0)
        return Font::ALIGN_LEFT;
    if (strcmp(s, "ALIGN_HCENTER") == 0)
        return Font::ALIGN_HCENTER;
    if (strcmp(s, "ALIGN_RIGHT") == 0)
        return Font::ALIGN_RIGHT;
    if (strcmp(s, "ALIGN_TOP") == 0)
        return Font::ALIGN_TOP;
    if (strcmp(s, "ALIGN_VCENTER") == 0)
        return Font::ALIGN_VCENTER;
    if (strcmp(s, "ALIGN_BOTTOM") == 0)
        return Font::ALIGN_BOTTOM;
    if (strcmp(s, "ALIGN_TOP_LEFT") == 0)
        return Font::ALIGN_TOP_LEFT;
    if (strcmp(s, "ALIGN_VCENTER_LEFT") == 0)
        return Font::ALIGN_VCENTER_LEFT;
    if (strcmp(s, "ALIGN_BOTTOM_LEFT") == 0)
        return Font::ALIGN_BOTTOM_LEFT;
    if (strcmp(s, "ALIGN_TOP_HCENTER") == 0)
        return Font::ALIGN_TOP_HCENTER;
    if (strcmp(s, "ALIGN_VCENTER_HCENTER") == 0)
        return Font::ALIGN_VCENTER_HCENTER;
    if (strcmp(s, "ALIGN_BOTTOM_HCENTER") == 0)
        return Font::ALIGN_BOTTOM_HCENTER;
    if (strcmp(s, "ALIGN_TOP_RIGHT") == 0)
        return Font::ALIGN_TOP_RIGHT;
    if (strcmp(s, "ALIGN_VCENTER_RIGHT") == 0)
        return Font::ALIGN_VCENTER_RIGHT;
    if (strcmp(s, "ALIGN_BOTTOM_RIGHT") == 0)
        return Font::ALIGN_BOTTOM_RIGHT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Font::Justify.", s);
    return Font::ALIGN_LEFT;
}

std::string lua_stringFromEnum_FontJustify(Font::Justify e)
{
    if (e == Font::ALIGN_LEFT)
        return std::string("ALIGN_LEFT");
    if (e == Font::ALIGN_HCENTER)
        return std::string("ALIGN_HCENTER");
    if (e == Font::ALIGN_RIGHT)
        return std::string("ALIGN_RIGHT");
    if (e == Font::ALIGN_TOP)
        return std::string("ALIGN_TOP");
    if (e == Font::ALIGN_VCENTER)
        return std::string("ALIGN_VCENTER");
    if (e == Font::ALIGN_BOTTOM)
        return std::string("ALIGN_BOTTOM");
    if (e == Font::ALIGN_TOP_LEFT)
        return std::string("ALIGN_TOP_LEFT");
    if (e == Font::ALIGN_VCENTER_LEFT)
        return std::string("ALIGN_VCENTER_LEFT");
    if (e == Font::ALIGN_BOTTOM_LEFT)
        return std::string("ALIGN_BOTTOM_LEFT");
    if (e == Font::ALIGN_TOP_HCENTER)
        return std::string("ALIGN_TOP_HCENTER");
    if (e == Font::ALIGN_VCENTER_HCENTER)
        return std::string("ALIGN_VCENTER_HCENTER");
    if (e == Font::ALIGN_BOTTOM_HCENTER)
        return std::string("ALIGN_BOTTOM_HCENTER");
    if (e == Font::ALIGN_TOP_RIGHT)
        return std::string("ALIGN_TOP_RIGHT");
    if (e == Font::ALIGN_VCENTER_RIGHT)
        return std::string("ALIGN_VCENTER_RIGHT");
    if (e == Font::ALIGN_BOTTOM_RIGHT)
        return std::string("ALIGN_BOTTOM_RIGHT");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Font::Justify.", e);
    return std::string();
}

Font::Style lua_enumFromString_FontStyle(const char* s)
{
    if (strcmp(s, "PLAIN") == 0)
        return Font::PLAIN;
    if (strcmp(s, "BOLD") == 0)
        return Font::BOLD;
    if (strcmp(s, "ITALIC") == 0)
        return Font::ITALIC;
    if (strcmp(s, "BOLD_ITALIC") == 0)
        return Font::BOLD_ITALIC;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Font::Style.", s);
    return Font::PLAIN;
}

std::string lua_stringFromEnum_FontStyle(Font::Style e)
{
    if (e == Font::PLAIN)
        return std::string("PLAIN");
    if (e == Font::BOLD)
        return std::string("BOLD");
    if (e == Font::ITALIC)
        return std::string("ITALIC");
    if (e == Font::BOLD_ITALIC)
        return std::string("BOLD_ITALIC");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Font::Style.", e);
    return std::string();
}

Game::ClearFlags lua_enumFromString_GameClearFlags(const char* s)
{
    if (strcmp(s, "CLEAR_COLOR") == 0)
        return Game::CLEAR_COLOR;
    if (strcmp(s, "CLEAR_DEPTH") == 0)
        return Game::CLEAR_DEPTH;
    if (strcmp(s, "CLEAR_STENCIL") == 0)
        return Game::CLEAR_STENCIL;
    if (strcmp(s, "CLEAR_COLOR_DEPTH") == 0)
        return Game::CLEAR_COLOR_DEPTH;
    if (strcmp(s, "CLEAR_COLOR_STENCIL") == 0)
        return Game::CLEAR_COLOR_STENCIL;
    if (strcmp(s, "CLEAR_DEPTH_STENCIL") == 0)
        return Game::CLEAR_DEPTH_STENCIL;
    if (strcmp(s, "CLEAR_COLOR_DEPTH_STENCIL") == 0)
        return Game::CLEAR_COLOR_DEPTH_STENCIL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Game::ClearFlags.", s);
    return Game::CLEAR_COLOR;
}

std::string lua_stringFromEnum_GameClearFlags(Game::ClearFlags e)
{
    if (e == Game::CLEAR_COLOR)
        return std::string("CLEAR_COLOR");
    if (e == Game::CLEAR_DEPTH)
        return std::string("CLEAR_DEPTH");
    if (e == Game::CLEAR_STENCIL)
        return std::string("CLEAR_STENCIL");
    if (e == Game::CLEAR_COLOR_DEPTH)
        return std::string("CLEAR_COLOR_DEPTH");
    if (e == Game::CLEAR_COLOR_STENCIL)
        return std::string("CLEAR_COLOR_STENCIL");
    if (e == Game::CLEAR_DEPTH_STENCIL)
        return std::string("CLEAR_DEPTH_STENCIL");
    if (e == Game::CLEAR_COLOR_DEPTH_STENCIL)
        return std::string("CLEAR_COLOR_DEPTH_STENCIL");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Game::ClearFlags.", e);
    return std::string();
}

Game::State lua_enumFromString_GameState(const char* s)
{
    if (strcmp(s, "UNINITIALIZED") == 0)
        return Game::UNINITIALIZED;
    if (strcmp(s, "RUNNING") == 0)
        return Game::RUNNING;
    if (strcmp(s, "PAUSED") == 0)
        return Game::PAUSED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Game::State.", s);
    return Game::UNINITIALIZED;
}

std::string lua_stringFromEnum_GameState(Game::State e)
{
    if (e == Game::UNINITIALIZED)
        return std::string("UNINITIALIZED");
    if (e == Game::RUNNING)
        return std::string("RUNNING");
    if (e == Game::PAUSED)
        return std::string("PAUSED");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Game::State.", e);
    return std::string();
}

Image::Format lua_enumFromString_ImageFormat(const char* s)
{
    if (strcmp(s, "RGB") == 0)
        return Image::RGB;
    if (strcmp(s, "RGBA") == 0)
        return Image::RGBA;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Image::Format.", s);
    return Image::RGB;
}

std::string lua_stringFromEnum_ImageFormat(Image::Format e)
{
    if (e == Image::RGB)
        return std::string("RGB");
    if (e == Image::RGBA)
        return std::string("RGBA");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Image::Format.", e);
    return std::string();
}

Keyboard::Key lua_enumFromString_KeyboardKey(const char* s)
{
    if (strcmp(s, "KEY_NONE") == 0)
        return Keyboard::KEY_NONE;
    if (strcmp(s, "KEY_PAUSE") == 0)
        return Keyboard::KEY_PAUSE;
    if (strcmp(s, "KEY_SCROLL_LOCK") == 0)
        return Keyboard::KEY_SCROLL_LOCK;
    if (strcmp(s, "KEY_PRINT") == 0)
        return Keyboard::KEY_PRINT;
    if (strcmp(s, "KEY_SYSREQ") == 0)
        return Keyboard::KEY_SYSREQ;
    if (strcmp(s, "KEY_BREAK") == 0)
        return Keyboard::KEY_BREAK;
    if (strcmp(s, "KEY_ESCAPE") == 0)
        return Keyboard::KEY_ESCAPE;
    if (strcmp(s, "KEY_BACKSPACE") == 0)
        return Keyboard::KEY_BACKSPACE;
    if (strcmp(s, "KEY_TAB") == 0)
        return Keyboard::KEY_TAB;
    if (strcmp(s, "KEY_BACK_TAB") == 0)
        return Keyboard::KEY_BACK_TAB;
    if (strcmp(s, "KEY_RETURN") == 0)
        return Keyboard::KEY_RETURN;
    if (strcmp(s, "KEY_CAPS_LOCK") == 0)
        return Keyboard::KEY_CAPS_LOCK;
    if (strcmp(s, "KEY_SHIFT") == 0)
        return Keyboard::KEY_SHIFT;
    if (strcmp(s, "KEY_CTRL") == 0)
        return Keyboard::KEY_CTRL;
    if (strcmp(s, "KEY_ALT") == 0)
        return Keyboard::KEY_ALT;
    if (strcmp(s, "KEY_MENU") == 0)
        return Keyboard::KEY_MENU;
    if (strcmp(s, "KEY_HYPER") == 0)
        return Keyboard::KEY_HYPER;
    if (strcmp(s, "KEY_INSERT") == 0)
        return Keyboard::KEY_INSERT;
    if (strcmp(s, "KEY_HOME") == 0)
        return Keyboard::KEY_HOME;
    if (strcmp(s, "KEY_PG_UP") == 0)
        return Keyboard::KEY_PG_UP;
    if (strcmp(s, "KEY_DELETE") == 0)
        return Keyboard::KEY_DELETE;
    if (strcmp(s, "KEY_END") == 0)
        return Keyboard::KEY_END;
    if (strcmp(s, "KEY_PG_DOWN") == 0)
        return Keyboard::KEY_PG_DOWN;
    if (strcmp(s, "KEY_LEFT_ARROW") == 0)
        return Keyboard::KEY_LEFT_ARROW;
    if (strcmp(s, "KEY_RIGHT_ARROW") == 0)
        return Keyboard::KEY_RIGHT_ARROW;
    if (strcmp(s, "KEY_UP_ARROW") == 0)
        return Keyboard::KEY_UP_ARROW;
    if (strcmp(s, "KEY_DOWN_ARROW") == 0)
        return Keyboard::KEY_DOWN_ARROW;
    if (strcmp(s, "KEY_NUM_LOCK") == 0)
        return Keyboard::KEY_NUM_LOCK;
    if (strcmp(s, "KEY_KP_PLUS") == 0)
        return Keyboard::KEY_KP_PLUS;
    if (strcmp(s, "KEY_KP_MINUS") == 0)
        return Keyboard::KEY_KP_MINUS;
    if (strcmp(s, "KEY_KP_MULTIPLY") == 0)
        return Keyboard::KEY_KP_MULTIPLY;
    if (strcmp(s, "KEY_KP_DIVIDE") == 0)
        return Keyboard::KEY_KP_DIVIDE;
    if (strcmp(s, "KEY_KP_ENTER") == 0)
        return Keyboard::KEY_KP_ENTER;
    if (strcmp(s, "KEY_KP_HOME") == 0)
        return Keyboard::KEY_KP_HOME;
    if (strcmp(s, "KEY_KP_UP") == 0)
        return Keyboard::KEY_KP_UP;
    if (strcmp(s, "KEY_KP_PG_UP") == 0)
        return Keyboard::KEY_KP_PG_UP;
    if (strcmp(s, "KEY_KP_LEFT") == 0)
        return Keyboard::KEY_KP_LEFT;
    if (strcmp(s, "KEY_KP_FIVE") == 0)
        return Keyboard::KEY_KP_FIVE;
    if (strcmp(s, "KEY_KP_RIGHT") == 0)
        return Keyboard::KEY_KP_RIGHT;
    if (strcmp(s, "KEY_KP_END") == 0)
        return Keyboard::KEY_KP_END;
    if (strcmp(s, "KEY_KP_DOWN") == 0)
        return Keyboard::KEY_KP_DOWN;
    if (strcmp(s, "KEY_KP_PG_DOWN") == 0)
        return Keyboard::KEY_KP_PG_DOWN;
    if (strcmp(s, "KEY_KP_INSERT") == 0)
        return Keyboard::KEY_KP_INSERT;
    if (strcmp(s, "KEY_KP_DELETE") == 0)
        return Keyboard::KEY_KP_DELETE;
    if (strcmp(s, "KEY_F1") == 0)
        return Keyboard::KEY_F1;
    if (strcmp(s, "KEY_F2") == 0)
        return Keyboard::KEY_F2;
    if (strcmp(s, "KEY_F3") == 0)
        return Keyboard::KEY_F3;
    if (strcmp(s, "KEY_F4") == 0)
        return Keyboard::KEY_F4;
    if (strcmp(s, "KEY_F5") == 0)
        return Keyboard::KEY_F5;
    if (strcmp(s, "KEY_F6") == 0)
        return Keyboard::KEY_F6;
    if (strcmp(s, "KEY_F7") == 0)
        return Keyboard::KEY_F7;
    if (strcmp(s, "KEY_F8") == 0)
        return Keyboard::KEY_F8;
    if (strcmp(s, "KEY_F9") == 0)
        return Keyboard::KEY_F9;
    if (strcmp(s, "KEY_F10") == 0)
        return Keyboard::KEY_F10;
    if (strcmp(s, "KEY_F11") == 0)
        return Keyboard::KEY_F11;
    if (strcmp(s, "KEY_F12") == 0)
        return Keyboard::KEY_F12;
    if (strcmp(s, "KEY_SPACE") == 0)
        return Keyboard::KEY_SPACE;
    if (strcmp(s, "KEY_EXCLAM") == 0)
        return Keyboard::KEY_EXCLAM;
    if (strcmp(s, "KEY_QUOTE") == 0)
        return Keyboard::KEY_QUOTE;
    if (strcmp(s, "KEY_NUMBER") == 0)
        return Keyboard::KEY_NUMBER;
    if (strcmp(s, "KEY_DOLLAR") == 0)
        return Keyboard::KEY_DOLLAR;
    if (strcmp(s, "KEY_PERCENT") == 0)
        return Keyboard::KEY_PERCENT;
    if (strcmp(s, "KEY_CIRCUMFLEX") == 0)
        return Keyboard::KEY_CIRCUMFLEX;
    if (strcmp(s, "KEY_AMPERSAND") == 0)
        return Keyboard::KEY_AMPERSAND;
    if (strcmp(s, "KEY_APOSTROPHE") == 0)
        return Keyboard::KEY_APOSTROPHE;
    if (strcmp(s, "KEY_LEFT_PARENTHESIS") == 0)
        return Keyboard::KEY_LEFT_PARENTHESIS;
    if (strcmp(s, "KEY_RIGHT_PARENTHESIS") == 0)
        return Keyboard::KEY_RIGHT_PARENTHESIS;
    if (strcmp(s, "KEY_ASTERISK") == 0)
        return Keyboard::KEY_ASTERISK;
    if (strcmp(s, "KEY_PLUS") == 0)
        return Keyboard::KEY_PLUS;
    if (strcmp(s, "KEY_COMMA") == 0)
        return Keyboard::KEY_COMMA;
    if (strcmp(s, "KEY_MINUS") == 0)
        return Keyboard::KEY_MINUS;
    if (strcmp(s, "KEY_PERIOD") == 0)
        return Keyboard::KEY_PERIOD;
    if (strcmp(s, "KEY_SLASH") == 0)
        return Keyboard::KEY_SLASH;
    if (strcmp(s, "KEY_ZERO") == 0)
        return Keyboard::KEY_ZERO;
    if (strcmp(s, "KEY_ONE") == 0)
        return Keyboard::KEY_ONE;
    if (strcmp(s, "KEY_TWO") == 0)
        return Keyboard::KEY_TWO;
    if (strcmp(s, "KEY_THREE") == 0)
        return Keyboard::KEY_THREE;
    if (strcmp(s, "KEY_FOUR") == 0)
        return Keyboard::KEY_FOUR;
    if (strcmp(s, "KEY_FIVE") == 0)
        return Keyboard::KEY_FIVE;
    if (strcmp(s, "KEY_SIX") == 0)
        return Keyboard::KEY_SIX;
    if (strcmp(s, "KEY_SEVEN") == 0)
        return Keyboard::KEY_SEVEN;
    if (strcmp(s, "KEY_EIGHT") == 0)
        return Keyboard::KEY_EIGHT;
    if (strcmp(s, "KEY_NINE") == 0)
        return Keyboard::KEY_NINE;
    if (strcmp(s, "KEY_COLON") == 0)
        return Keyboard::KEY_COLON;
    if (strcmp(s, "KEY_SEMICOLON") == 0)
        return Keyboard::KEY_SEMICOLON;
    if (strcmp(s, "KEY_LESS_THAN") == 0)
        return Keyboard::KEY_LESS_THAN;
    if (strcmp(s, "KEY_EQUAL") == 0)
        return Keyboard::KEY_EQUAL;
    if (strcmp(s, "KEY_GREATER_THAN") == 0)
        return Keyboard::KEY_GREATER_THAN;
    if (strcmp(s, "KEY_QUESTION") == 0)
        return Keyboard::KEY_QUESTION;
    if (strcmp(s, "KEY_AT") == 0)
        return Keyboard::KEY_AT;
    if (strcmp(s, "KEY_CAPITAL_A") == 0)
        return Keyboard::KEY_CAPITAL_A;
    if (strcmp(s, "KEY_CAPITAL_B") == 0)
        return Keyboard::KEY_CAPITAL_B;
    if (strcmp(s, "KEY_CAPITAL_C") == 0)
        return Keyboard::KEY_CAPITAL_C;
    if (strcmp(s, "KEY_CAPITAL_D") == 0)
        return Keyboard::KEY_CAPITAL_D;
    if (strcmp(s, "KEY_CAPITAL_E") == 0)
        return Keyboard::KEY_CAPITAL_E;
    if (strcmp(s, "KEY_CAPITAL_F") == 0)
        return Keyboard::KEY_CAPITAL_F;
    if (strcmp(s, "KEY_CAPITAL_G") == 0)
        return Keyboard::KEY_CAPITAL_G;
    if (strcmp(s, "KEY_CAPITAL_H") == 0)
        return Keyboard::KEY_CAPITAL_H;
    if (strcmp(s, "KEY_CAPITAL_I") == 0)
        return Keyboard::KEY_CAPITAL_I;
    if (strcmp(s, "KEY_CAPITAL_J") == 0)
        return Keyboard::KEY_CAPITAL_J;
    if (strcmp(s, "KEY_CAPITAL_K") == 0)
        return Keyboard::KEY_CAPITAL_K;
    if (strcmp(s, "KEY_CAPITAL_L") == 0)
        return Keyboard::KEY_CAPITAL_L;
    if (strcmp(s, "KEY_CAPITAL_M") == 0)
        return Keyboard::KEY_CAPITAL_M;
    if (strcmp(s, "KEY_CAPITAL_N") == 0)
        return Keyboard::KEY_CAPITAL_N;
    if (strcmp(s, "KEY_CAPITAL_O") == 0)
        return Keyboard::KEY_CAPITAL_O;
    if (strcmp(s, "KEY_CAPITAL_P") == 0)
        return Keyboard::KEY_CAPITAL_P;
    if (strcmp(s, "KEY_CAPITAL_Q") == 0)
        return Keyboard::KEY_CAPITAL_Q;
    if (strcmp(s, "KEY_CAPITAL_R") == 0)
        return Keyboard::KEY_CAPITAL_R;
    if (strcmp(s, "KEY_CAPITAL_S") == 0)
        return Keyboard::KEY_CAPITAL_S;
    if (strcmp(s, "KEY_CAPITAL_T") == 0)
        return Keyboard::KEY_CAPITAL_T;
    if (strcmp(s, "KEY_CAPITAL_U") == 0)
        return Keyboard::KEY_CAPITAL_U;
    if (strcmp(s, "KEY_CAPITAL_V") == 0)
        return Keyboard::KEY_CAPITAL_V;
    if (strcmp(s, "KEY_CAPITAL_W") == 0)
        return Keyboard::KEY_CAPITAL_W;
    if (strcmp(s, "KEY_CAPITAL_X") == 0)
        return Keyboard::KEY_CAPITAL_X;
    if (strcmp(s, "KEY_CAPITAL_Y") == 0)
        return Keyboard::KEY_CAPITAL_Y;
    if (strcmp(s, "KEY_CAPITAL_Z") == 0)
        return Keyboard::KEY_CAPITAL_Z;
    if (strcmp(s, "KEY_LEFT_BRACKET") == 0)
        return Keyboard::KEY_LEFT_BRACKET;
    if (strcmp(s, "KEY_BACK_SLASH") == 0)
        return Keyboard::KEY_BACK_SLASH;
    if (strcmp(s, "KEY_RIGHT_BRACKET") == 0)
        return Keyboard::KEY_RIGHT_BRACKET;
    if (strcmp(s, "KEY_UNDERSCORE") == 0)
        return Keyboard::KEY_UNDERSCORE;
    if (strcmp(s, "KEY_GRAVE") == 0)
        return Keyboard::KEY_GRAVE;
    if (strcmp(s, "KEY_A") == 0)
        return Keyboard::KEY_A;
    if (strcmp(s, "KEY_B") == 0)
        return Keyboard::KEY_B;
    if (strcmp(s, "KEY_C") == 0)
        return Keyboard::KEY_C;
    if (strcmp(s, "KEY_D") == 0)
        return Keyboard::KEY_D;
    if (strcmp(s, "KEY_E") == 0)
        return Keyboard::KEY_E;
    if (strcmp(s, "KEY_F") == 0)
        return Keyboard::KEY_F;
    if (strcmp(s, "KEY_G") == 0)
        return Keyboard::KEY_G;
    if (strcmp(s, "KEY_H") == 0)
        return Keyboard::KEY_H;
    if (strcmp(s, "KEY_I") == 0)
        return Keyboard::KEY_I;
    if (strcmp(s, "KEY_J") == 0)
        return Keyboard::KEY_J;
    if (strcmp(s, "KEY_K") == 0)
        return Keyboard::KEY_K;
    if (strcmp(s, "KEY_L") == 0)
        return Keyboard::KEY_L;
    if (strcmp(s, "KEY_M") == 0)
        return Keyboard::KEY_M;
    if (strcmp(s, "KEY_N") == 0)
        return Keyboard::KEY_N;
    if (strcmp(s, "KEY_O") == 0)
        return Keyboard::KEY_O;
    if (strcmp(s, "KEY_P") == 0)
        return Keyboard::KEY_P;
    if (strcmp(s, "KEY_Q") == 0)
        return Keyboard::KEY_Q;
    if (strcmp(s, "KEY_R") == 0)
        return Keyboard::KEY_R;
    if (strcmp(s, "KEY_S") == 0)
        return Keyboard::KEY_S;
    if (strcmp(s, "KEY_T") == 0)
        return Keyboard::KEY_T;
    if (strcmp(s, "KEY_U") == 0)
        return Keyboard::KEY_U;
    if (strcmp(s, "KEY_V") == 0)
        return Keyboard::KEY_V;
    if (strcmp(s, "KEY_W") == 0)
        return Keyboard::KEY_W;
    if (strcmp(s, "KEY_X") == 0)
        return Keyboard::KEY_X;
    if (strcmp(s, "KEY_Y") == 0)
        return Keyboard::KEY_Y;
    if (strcmp(s, "KEY_Z") == 0)
        return Keyboard::KEY_Z;
    if (strcmp(s, "KEY_LEFT_BRACE") == 0)
        return Keyboard::KEY_LEFT_BRACE;
    if (strcmp(s, "KEY_BAR") == 0)
        return Keyboard::KEY_BAR;
    if (strcmp(s, "KEY_RIGHT_BRACE") == 0)
        return Keyboard::KEY_RIGHT_BRACE;
    if (strcmp(s, "KEY_TILDE") == 0)
        return Keyboard::KEY_TILDE;
    if (strcmp(s, "KEY_EURO") == 0)
        return Keyboard::KEY_EURO;
    if (strcmp(s, "KEY_POUND") == 0)
        return Keyboard::KEY_POUND;
    if (strcmp(s, "KEY_YEN") == 0)
        return Keyboard::KEY_YEN;
    if (strcmp(s, "KEY_MIDDLE_DOT") == 0)
        return Keyboard::KEY_MIDDLE_DOT;
    if (strcmp(s, "KEY_SEARCH") == 0)
        return Keyboard::KEY_SEARCH;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Keyboard::Key.", s);
    return Keyboard::KEY_NONE;
}

std::string lua_stringFromEnum_KeyboardKey(Keyboard::Key e)
{
    if (e == Keyboard::KEY_NONE)
        return std::string("KEY_NONE");
    if (e == Keyboard::KEY_PAUSE)
        return std::string("KEY_PAUSE");
    if (e == Keyboard::KEY_SCROLL_LOCK)
        return std::string("KEY_SCROLL_LOCK");
    if (e == Keyboard::KEY_PRINT)
        return std::string("KEY_PRINT");
    if (e == Keyboard::KEY_SYSREQ)
        return std::string("KEY_SYSREQ");
    if (e == Keyboard::KEY_BREAK)
        return std::string("KEY_BREAK");
    if (e == Keyboard::KEY_ESCAPE)
        return std::string("KEY_ESCAPE");
    if (e == Keyboard::KEY_BACKSPACE)
        return std::string("KEY_BACKSPACE");
    if (e == Keyboard::KEY_TAB)
        return std::string("KEY_TAB");
    if (e == Keyboard::KEY_BACK_TAB)
        return std::string("KEY_BACK_TAB");
    if (e == Keyboard::KEY_RETURN)
        return std::string("KEY_RETURN");
    if (e == Keyboard::KEY_CAPS_LOCK)
        return std::string("KEY_CAPS_LOCK");
    if (e == Keyboard::KEY_SHIFT)
        return std::string("KEY_SHIFT");
    if (e == Keyboard::KEY_CTRL)
        return std::string("KEY_CTRL");
    if (e == Keyboard::KEY_ALT)
        return std::string("KEY_ALT");
    if (e == Keyboard::KEY_MENU)
        return std::string("KEY_MENU");
    if (e == Keyboard::KEY_HYPER)
        return std::string("KEY_HYPER");
    if (e == Keyboard::KEY_INSERT)
        return std::string("KEY_INSERT");
    if (e == Keyboard::KEY_HOME)
        return std::string("KEY_HOME");
    if (e == Keyboard::KEY_PG_UP)
        return std::string("KEY_PG_UP");
    if (e == Keyboard::KEY_DELETE)
        return std::string("KEY_DELETE");
    if (e == Keyboard::KEY_END)
        return std::string("KEY_END");
    if (e == Keyboard::KEY_PG_DOWN)
        return std::string("KEY_PG_DOWN");
    if (e == Keyboard::KEY_LEFT_ARROW)
        return std::string("KEY_LEFT_ARROW");
    if (e == Keyboard::KEY_RIGHT_ARROW)
        return std::string("KEY_RIGHT_ARROW");
    if (e == Keyboard::KEY_UP_ARROW)
        return std::string("KEY_UP_ARROW");
    if (e == Keyboard::KEY_DOWN_ARROW)
        return std::string("KEY_DOWN_ARROW");
    if (e == Keyboard::KEY_NUM_LOCK)
        return std::string("KEY_NUM_LOCK");
    if (e == Keyboard::KEY_KP_PLUS)
        return std::string("KEY_KP_PLUS");
    if (e == Keyboard::KEY_KP_MINUS)
        return std::string("KEY_KP_MINUS");
    if (e == Keyboard::KEY_KP_MULTIPLY)
        return std::string("KEY_KP_MULTIPLY");
    if (e == Keyboard::KEY_KP_DIVIDE)
        return std::string("KEY_KP_DIVIDE");
    if (e == Keyboard::KEY_KP_ENTER)
        return std::string("KEY_KP_ENTER");
    if (e == Keyboard::KEY_KP_HOME)
        return std::string("KEY_KP_HOME");
    if (e == Keyboard::KEY_KP_UP)
        return std::string("KEY_KP_UP");
    if (e == Keyboard::KEY_KP_PG_UP)
        return std::string("KEY_KP_PG_UP");
    if (e == Keyboard::KEY_KP_LEFT)
        return std::string("KEY_KP_LEFT");
    if (e == Keyboard::KEY_KP_FIVE)
        return std::string("KEY_KP_FIVE");
    if (e == Keyboard::KEY_KP_RIGHT)
        return std::string("KEY_KP_RIGHT");
    if (e == Keyboard::KEY_KP_END)
        return std::string("KEY_KP_END");
    if (e == Keyboard::KEY_KP_DOWN)
        return std::string("KEY_KP_DOWN");
    if (e == Keyboard::KEY_KP_PG_DOWN)
        return std::string("KEY_KP_PG_DOWN");
    if (e == Keyboard::KEY_KP_INSERT)
        return std::string("KEY_KP_INSERT");
    if (e == Keyboard::KEY_KP_DELETE)
        return std::string("KEY_KP_DELETE");
    if (e == Keyboard::KEY_F1)
        return std::string("KEY_F1");
    if (e == Keyboard::KEY_F2)
        return std::string("KEY_F2");
    if (e == Keyboard::KEY_F3)
        return std::string("KEY_F3");
    if (e == Keyboard::KEY_F4)
        return std::string("KEY_F4");
    if (e == Keyboard::KEY_F5)
        return std::string("KEY_F5");
    if (e == Keyboard::KEY_F6)
        return std::string("KEY_F6");
    if (e == Keyboard::KEY_F7)
        return std::string("KEY_F7");
    if (e == Keyboard::KEY_F8)
        return std::string("KEY_F8");
    if (e == Keyboard::KEY_F9)
        return std::string("KEY_F9");
    if (e == Keyboard::KEY_F10)
        return std::string("KEY_F10");
    if (e == Keyboard::KEY_F11)
        return std::string("KEY_F11");
    if (e == Keyboard::KEY_F12)
        return std::string("KEY_F12");
    if (e == Keyboard::KEY_SPACE)
        return std::string("KEY_SPACE");
    if (e == Keyboard::KEY_EXCLAM)
        return std::string("KEY_EXCLAM");
    if (e == Keyboard::KEY_QUOTE)
        return std::string("KEY_QUOTE");
    if (e == Keyboard::KEY_NUMBER)
        return std::string("KEY_NUMBER");
    if (e == Keyboard::KEY_DOLLAR)
        return std::string("KEY_DOLLAR");
    if (e == Keyboard::KEY_PERCENT)
        return std::string("KEY_PERCENT");
    if (e == Keyboard::KEY_CIRCUMFLEX)
        return std::string("KEY_CIRCUMFLEX");
    if (e == Keyboard::KEY_AMPERSAND)
        return std::string("KEY_AMPERSAND");
    if (e == Keyboard::KEY_APOSTROPHE)
        return std::string("KEY_APOSTROPHE");
    if (e == Keyboard::KEY_LEFT_PARENTHESIS)
        return std::string("KEY_LEFT_PARENTHESIS");
    if (e == Keyboard::KEY_RIGHT_PARENTHESIS)
        return std::string("KEY_RIGHT_PARENTHESIS");
    if (e == Keyboard::KEY_ASTERISK)
        return std::string("KEY_ASTERISK");
    if (e == Keyboard::KEY_PLUS)
        return std::string("KEY_PLUS");
    if (e == Keyboard::KEY_COMMA)
        return std::string("KEY_COMMA");
    if (e == Keyboard::KEY_MINUS)
        return std::string("KEY_MINUS");
    if (e == Keyboard::KEY_PERIOD)
        return std::string("KEY_PERIOD");
    if (e == Keyboard::KEY_SLASH)
        return std::string("KEY_SLASH");
    if (e == Keyboard::KEY_ZERO)
        return std::string("KEY_ZERO");
    if (e == Keyboard::KEY_ONE)
        return std::string("KEY_ONE");
    if (e == Keyboard::KEY_TWO)
        return std::string("KEY_TWO");
    if (e == Keyboard::KEY_THREE)
        return std::string("KEY_THREE");
    if (e == Keyboard::KEY_FOUR)
        return std::string("KEY_FOUR");
    if (e == Keyboard::KEY_FIVE)
        return std::string("KEY_FIVE");
    if (e == Keyboard::KEY_SIX)
        return std::string("KEY_SIX");
    if (e == Keyboard::KEY_SEVEN)
        return std::string("KEY_SEVEN");
    if (e == Keyboard::KEY_EIGHT)
        return std::string("KEY_EIGHT");
    if (e == Keyboard::KEY_NINE)
        return std::string("KEY_NINE");
    if (e == Keyboard::KEY_COLON)
        return std::string("KEY_COLON");
    if (e == Keyboard::KEY_SEMICOLON)
        return std::string("KEY_SEMICOLON");
    if (e == Keyboard::KEY_LESS_THAN)
        return std::string("KEY_LESS_THAN");
    if (e == Keyboard::KEY_EQUAL)
        return std::string("KEY_EQUAL");
    if (e == Keyboard::KEY_GREATER_THAN)
        return std::string("KEY_GREATER_THAN");
    if (e == Keyboard::KEY_QUESTION)
        return std::string("KEY_QUESTION");
    if (e == Keyboard::KEY_AT)
        return std::string("KEY_AT");
    if (e == Keyboard::KEY_CAPITAL_A)
        return std::string("KEY_CAPITAL_A");
    if (e == Keyboard::KEY_CAPITAL_B)
        return std::string("KEY_CAPITAL_B");
    if (e == Keyboard::KEY_CAPITAL_C)
        return std::string("KEY_CAPITAL_C");
    if (e == Keyboard::KEY_CAPITAL_D)
        return std::string("KEY_CAPITAL_D");
    if (e == Keyboard::KEY_CAPITAL_E)
        return std::string("KEY_CAPITAL_E");
    if (e == Keyboard::KEY_CAPITAL_F)
        return std::string("KEY_CAPITAL_F");
    if (e == Keyboard::KEY_CAPITAL_G)
        return std::string("KEY_CAPITAL_G");
    if (e == Keyboard::KEY_CAPITAL_H)
        return std::string("KEY_CAPITAL_H");
    if (e == Keyboard::KEY_CAPITAL_I)
        return std::string("KEY_CAPITAL_I");
    if (e == Keyboard::KEY_CAPITAL_J)
        return std::string("KEY_CAPITAL_J");
    if (e == Keyboard::KEY_CAPITAL_K)
        return std::string("KEY_CAPITAL_K");
    if (e == Keyboard::KEY_CAPITAL_L)
        return std::string("KEY_CAPITAL_L");
    if (e == Keyboard::KEY_CAPITAL_M)
        return std::string("KEY_CAPITAL_M");
    if (e == Keyboard::KEY_CAPITAL_N)
        return std::string("KEY_CAPITAL_N");
    if (e == Keyboard::KEY_CAPITAL_O)
        return std::string("KEY_CAPITAL_O");
    if (e == Keyboard::KEY_CAPITAL_P)
        return std::string("KEY_CAPITAL_P");
    if (e == Keyboard::KEY_CAPITAL_Q)
        return std::string("KEY_CAPITAL_Q");
    if (e == Keyboard::KEY_CAPITAL_R)
        return std::string("KEY_CAPITAL_R");
    if (e == Keyboard::KEY_CAPITAL_S)
        return std::string("KEY_CAPITAL_S");
    if (e == Keyboard::KEY_CAPITAL_T)
        return std::string("KEY_CAPITAL_T");
    if (e == Keyboard::KEY_CAPITAL_U)
        return std::string("KEY_CAPITAL_U");
    if (e == Keyboard::KEY_CAPITAL_V)
        return std::string("KEY_CAPITAL_V");
    if (e == Keyboard::KEY_CAPITAL_W)
        return std::string("KEY_CAPITAL_W");
    if (e == Keyboard::KEY_CAPITAL_X)
        return std::string("KEY_CAPITAL_X");
    if (e == Keyboard::KEY_CAPITAL_Y)
        return std::string("KEY_CAPITAL_Y");
    if (e == Keyboard::KEY_CAPITAL_Z)
        return std::string("KEY_CAPITAL_Z");
    if (e == Keyboard::KEY_LEFT_BRACKET)
        return std::string("KEY_LEFT_BRACKET");
    if (e == Keyboard::KEY_BACK_SLASH)
        return std::string("KEY_BACK_SLASH");
    if (e == Keyboard::KEY_RIGHT_BRACKET)
        return std::string("KEY_RIGHT_BRACKET");
    if (e == Keyboard::KEY_UNDERSCORE)
        return std::string("KEY_UNDERSCORE");
    if (e == Keyboard::KEY_GRAVE)
        return std::string("KEY_GRAVE");
    if (e == Keyboard::KEY_A)
        return std::string("KEY_A");
    if (e == Keyboard::KEY_B)
        return std::string("KEY_B");
    if (e == Keyboard::KEY_C)
        return std::string("KEY_C");
    if (e == Keyboard::KEY_D)
        return std::string("KEY_D");
    if (e == Keyboard::KEY_E)
        return std::string("KEY_E");
    if (e == Keyboard::KEY_F)
        return std::string("KEY_F");
    if (e == Keyboard::KEY_G)
        return std::string("KEY_G");
    if (e == Keyboard::KEY_H)
        return std::string("KEY_H");
    if (e == Keyboard::KEY_I)
        return std::string("KEY_I");
    if (e == Keyboard::KEY_J)
        return std::string("KEY_J");
    if (e == Keyboard::KEY_K)
        return std::string("KEY_K");
    if (e == Keyboard::KEY_L)
        return std::string("KEY_L");
    if (e == Keyboard::KEY_M)
        return std::string("KEY_M");
    if (e == Keyboard::KEY_N)
        return std::string("KEY_N");
    if (e == Keyboard::KEY_O)
        return std::string("KEY_O");
    if (e == Keyboard::KEY_P)
        return std::string("KEY_P");
    if (e == Keyboard::KEY_Q)
        return std::string("KEY_Q");
    if (e == Keyboard::KEY_R)
        return std::string("KEY_R");
    if (e == Keyboard::KEY_S)
        return std::string("KEY_S");
    if (e == Keyboard::KEY_T)
        return std::string("KEY_T");
    if (e == Keyboard::KEY_U)
        return std::string("KEY_U");
    if (e == Keyboard::KEY_V)
        return std::string("KEY_V");
    if (e == Keyboard::KEY_W)
        return std::string("KEY_W");
    if (e == Keyboard::KEY_X)
        return std::string("KEY_X");
    if (e == Keyboard::KEY_Y)
        return std::string("KEY_Y");
    if (e == Keyboard::KEY_Z)
        return std::string("KEY_Z");
    if (e == Keyboard::KEY_LEFT_BRACE)
        return std::string("KEY_LEFT_BRACE");
    if (e == Keyboard::KEY_BAR)
        return std::string("KEY_BAR");
    if (e == Keyboard::KEY_RIGHT_BRACE)
        return std::string("KEY_RIGHT_BRACE");
    if (e == Keyboard::KEY_TILDE)
        return std::string("KEY_TILDE");
    if (e == Keyboard::KEY_EURO)
        return std::string("KEY_EURO");
    if (e == Keyboard::KEY_POUND)
        return std::string("KEY_POUND");
    if (e == Keyboard::KEY_YEN)
        return std::string("KEY_YEN");
    if (e == Keyboard::KEY_MIDDLE_DOT)
        return std::string("KEY_MIDDLE_DOT");
    if (e == Keyboard::KEY_SEARCH)
        return std::string("KEY_SEARCH");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Keyboard::Key.", e);
    return std::string();
}

Keyboard::KeyEvent lua_enumFromString_KeyboardKeyEvent(const char* s)
{
    if (strcmp(s, "KEY_PRESS") == 0)
        return Keyboard::KEY_PRESS;
    if (strcmp(s, "KEY_RELEASE") == 0)
        return Keyboard::KEY_RELEASE;
    if (strcmp(s, "KEY_CHAR") == 0)
        return Keyboard::KEY_CHAR;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Keyboard::KeyEvent.", s);
    return Keyboard::KEY_PRESS;
}

std::string lua_stringFromEnum_KeyboardKeyEvent(Keyboard::KeyEvent e)
{
    if (e == Keyboard::KEY_PRESS)
        return std::string("KEY_PRESS");
    if (e == Keyboard::KEY_RELEASE)
        return std::string("KEY_RELEASE");
    if (e == Keyboard::KEY_CHAR)
        return std::string("KEY_CHAR");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Keyboard::KeyEvent.", e);
    return std::string();
}

Layout::Type lua_enumFromString_LayoutType(const char* s)
{
    if (strcmp(s, "LAYOUT_FLOW") == 0)
        return Layout::LAYOUT_FLOW;
    if (strcmp(s, "LAYOUT_VERTICAL") == 0)
        return Layout::LAYOUT_VERTICAL;
    if (strcmp(s, "LAYOUT_ABSOLUTE") == 0)
        return Layout::LAYOUT_ABSOLUTE;
    if (strcmp(s, "LAYOUT_SCROLL") == 0)
        return Layout::LAYOUT_SCROLL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Layout::Type.", s);
    return Layout::LAYOUT_FLOW;
}

std::string lua_stringFromEnum_LayoutType(Layout::Type e)
{
    if (e == Layout::LAYOUT_FLOW)
        return std::string("LAYOUT_FLOW");
    if (e == Layout::LAYOUT_VERTICAL)
        return std::string("LAYOUT_VERTICAL");
    if (e == Layout::LAYOUT_ABSOLUTE)
        return std::string("LAYOUT_ABSOLUTE");
    if (e == Layout::LAYOUT_SCROLL)
        return std::string("LAYOUT_SCROLL");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Layout::Type.", e);
    return std::string();
}

Light::Type lua_enumFromString_LightType(const char* s)
{
    if (strcmp(s, "DIRECTIONAL") == 0)
        return Light::DIRECTIONAL;
    if (strcmp(s, "POINT") == 0)
        return Light::POINT;
    if (strcmp(s, "SPOT") == 0)
        return Light::SPOT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Light::Type.", s);
    return Light::DIRECTIONAL;
}

std::string lua_stringFromEnum_LightType(Light::Type e)
{
    if (e == Light::DIRECTIONAL)
        return std::string("DIRECTIONAL");
    if (e == Light::POINT)
        return std::string("POINT");
    if (e == Light::SPOT)
        return std::string("SPOT");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Light::Type.", e);
    return std::string();
}

Mesh::IndexFormat lua_enumFromString_MeshIndexFormat(const char* s)
{
    if (strcmp(s, "INDEX8") == 0)
        return Mesh::INDEX8;
    if (strcmp(s, "INDEX16") == 0)
        return Mesh::INDEX16;
    if (strcmp(s, "INDEX32") == 0)
        return Mesh::INDEX32;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Mesh::IndexFormat.", s);
    return Mesh::INDEX8;
}

std::string lua_stringFromEnum_MeshIndexFormat(Mesh::IndexFormat e)
{
    if (e == Mesh::INDEX8)
        return std::string("INDEX8");
    if (e == Mesh::INDEX16)
        return std::string("INDEX16");
    if (e == Mesh::INDEX32)
        return std::string("INDEX32");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Mesh::IndexFormat.", e);
    return std::string();
}

Mesh::PrimitiveType lua_enumFromString_MeshPrimitiveType(const char* s)
{
    if (strcmp(s, "TRIANGLES") == 0)
        return Mesh::TRIANGLES;
    if (strcmp(s, "TRIANGLE_STRIP") == 0)
        return Mesh::TRIANGLE_STRIP;
    if (strcmp(s, "LINES") == 0)
        return Mesh::LINES;
    if (strcmp(s, "LINE_STRIP") == 0)
        return Mesh::LINE_STRIP;
    if (strcmp(s, "POINTS") == 0)
        return Mesh::POINTS;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Mesh::PrimitiveType.", s);
    return Mesh::TRIANGLES;
}

std::string lua_stringFromEnum_MeshPrimitiveType(Mesh::PrimitiveType e)
{
    if (e == Mesh::TRIANGLES)
        return std::string("TRIANGLES");
    if (e == Mesh::TRIANGLE_STRIP)
        return std::string("TRIANGLE_STRIP");
    if (e == Mesh::LINES)
        return std::string("LINES");
    if (e == Mesh::LINE_STRIP)
        return std::string("LINE_STRIP");
    if (e == Mesh::POINTS)
        return std::string("POINTS");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Mesh::PrimitiveType.", e);
    return std::string();
}

Mouse::MouseEvent lua_enumFromString_MouseMouseEvent(const char* s)
{
    if (strcmp(s, "MOUSE_PRESS_LEFT_BUTTON") == 0)
        return Mouse::MOUSE_PRESS_LEFT_BUTTON;
    if (strcmp(s, "MOUSE_RELEASE_LEFT_BUTTON") == 0)
        return Mouse::MOUSE_RELEASE_LEFT_BUTTON;
    if (strcmp(s, "MOUSE_PRESS_MIDDLE_BUTTON") == 0)
        return Mouse::MOUSE_PRESS_MIDDLE_BUTTON;
    if (strcmp(s, "MOUSE_RELEASE_MIDDLE_BUTTON") == 0)
        return Mouse::MOUSE_RELEASE_MIDDLE_BUTTON;
    if (strcmp(s, "MOUSE_PRESS_RIGHT_BUTTON") == 0)
        return Mouse::MOUSE_PRESS_RIGHT_BUTTON;
    if (strcmp(s, "MOUSE_RELEASE_RIGHT_BUTTON") == 0)
        return Mouse::MOUSE_RELEASE_RIGHT_BUTTON;
    if (strcmp(s, "MOUSE_MOVE") == 0)
        return Mouse::MOUSE_MOVE;
    if (strcmp(s, "MOUSE_WHEEL") == 0)
        return Mouse::MOUSE_WHEEL;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Mouse::MouseEvent.", s);
    return Mouse::MOUSE_PRESS_LEFT_BUTTON;
}

std::string lua_stringFromEnum_MouseMouseEvent(Mouse::MouseEvent e)
{
    if (e == Mouse::MOUSE_PRESS_LEFT_BUTTON)
        return std::string("MOUSE_PRESS_LEFT_BUTTON");
    if (e == Mouse::MOUSE_RELEASE_LEFT_BUTTON)
        return std::string("MOUSE_RELEASE_LEFT_BUTTON");
    if (e == Mouse::MOUSE_PRESS_MIDDLE_BUTTON)
        return std::string("MOUSE_PRESS_MIDDLE_BUTTON");
    if (e == Mouse::MOUSE_RELEASE_MIDDLE_BUTTON)
        return std::string("MOUSE_RELEASE_MIDDLE_BUTTON");
    if (e == Mouse::MOUSE_PRESS_RIGHT_BUTTON)
        return std::string("MOUSE_PRESS_RIGHT_BUTTON");
    if (e == Mouse::MOUSE_RELEASE_RIGHT_BUTTON)
        return std::string("MOUSE_RELEASE_RIGHT_BUTTON");
    if (e == Mouse::MOUSE_MOVE)
        return std::string("MOUSE_MOVE");
    if (e == Mouse::MOUSE_WHEEL)
        return std::string("MOUSE_WHEEL");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Mouse::MouseEvent.", e);
    return std::string();
}

Node::Type lua_enumFromString_NodeType(const char* s)
{
    if (strcmp(s, "NODE") == 0)
        return Node::NODE;
    if (strcmp(s, "JOINT") == 0)
        return Node::JOINT;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Node::Type.", s);
    return Node::NODE;
}

std::string lua_stringFromEnum_NodeType(Node::Type e)
{
    if (e == Node::NODE)
        return std::string("NODE");
    if (e == Node::JOINT)
        return std::string("JOINT");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Node::Type.", e);
    return std::string();
}

ParticleEmitter::TextureBlending lua_enumFromString_ParticleEmitterTextureBlending(const char* s)
{
    if (strcmp(s, "BLEND_OPAQUE") == 0)
        return ParticleEmitter::BLEND_OPAQUE;
    if (strcmp(s, "BLEND_TRANSPARENT") == 0)
        return ParticleEmitter::BLEND_TRANSPARENT;
    if (strcmp(s, "BLEND_ADDITIVE") == 0)
        return ParticleEmitter::BLEND_ADDITIVE;
    if (strcmp(s, "BLEND_MULTIPLIED") == 0)
        return ParticleEmitter::BLEND_MULTIPLIED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration ParticleEmitter::TextureBlending.", s);
    return ParticleEmitter::BLEND_OPAQUE;
}

std::string lua_stringFromEnum_ParticleEmitterTextureBlending(ParticleEmitter::TextureBlending e)
{
    if (e == ParticleEmitter::BLEND_OPAQUE)
        return std::string("BLEND_OPAQUE");
    if (e == ParticleEmitter::BLEND_TRANSPARENT)
        return std::string("BLEND_TRANSPARENT");
    if (e == ParticleEmitter::BLEND_ADDITIVE)
        return std::string("BLEND_ADDITIVE");
    if (e == ParticleEmitter::BLEND_MULTIPLIED)
        return std::string("BLEND_MULTIPLIED");
    GP_ERROR("Invalid enumeration value '%d' for enumeration ParticleEmitter::TextureBlending.", e);
    return std::string();
}

PhysicsCollisionObject::CollisionListener::EventType lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(const char* s)
{
    if (strcmp(s, "COLLIDING") == 0)
        return PhysicsCollisionObject::CollisionListener::COLLIDING;
    if (strcmp(s, "NOT_COLLIDING") == 0)
        return PhysicsCollisionObject::CollisionListener::NOT_COLLIDING;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsCollisionObject::CollisionListener::EventType.", s);
    return PhysicsCollisionObject::CollisionListener::COLLIDING;
}

std::string lua_stringFromEnum_PhysicsCollisionObjectCollisionListenerEventType(PhysicsCollisionObject::CollisionListener::EventType e)
{
    if (e == PhysicsCollisionObject::CollisionListener::COLLIDING)
        return std::string("COLLIDING");
    if (e == PhysicsCollisionObject::CollisionListener::NOT_COLLIDING)
        return std::string("NOT_COLLIDING");
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsCollisionObject::CollisionListener::EventType.", e);
    return std::string();
}

PhysicsCollisionObject::Type lua_enumFromString_PhysicsCollisionObjectType(const char* s)
{
    if (strcmp(s, "RIGID_BODY") == 0)
        return PhysicsCollisionObject::RIGID_BODY;
    if (strcmp(s, "CHARACTER") == 0)
        return PhysicsCollisionObject::CHARACTER;
    if (strcmp(s, "GHOST_OBJECT") == 0)
        return PhysicsCollisionObject::GHOST_OBJECT;
    if (strcmp(s, "NONE") == 0)
        return PhysicsCollisionObject::NONE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsCollisionObject::Type.", s);
    return PhysicsCollisionObject::RIGID_BODY;
}

std::string lua_stringFromEnum_PhysicsCollisionObjectType(PhysicsCollisionObject::Type e)
{
    if (e == PhysicsCollisionObject::RIGID_BODY)
        return std::string("RIGID_BODY");
    if (e == PhysicsCollisionObject::CHARACTER)
        return std::string("CHARACTER");
    if (e == PhysicsCollisionObject::GHOST_OBJECT)
        return std::string("GHOST_OBJECT");
    if (e == PhysicsCollisionObject::NONE)
        return std::string("NONE");
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsCollisionObject::Type.", e);
    return std::string();
}

PhysicsCollisionShape::Type lua_enumFromString_PhysicsCollisionShapeType(const char* s)
{
    if (strcmp(s, "SHAPE_BOX") == 0)
        return PhysicsCollisionShape::SHAPE_BOX;
    if (strcmp(s, "SHAPE_SPHERE") == 0)
        return PhysicsCollisionShape::SHAPE_SPHERE;
    if (strcmp(s, "SHAPE_CAPSULE") == 0)
        return PhysicsCollisionShape::SHAPE_CAPSULE;
    if (strcmp(s, "SHAPE_MESH") == 0)
        return PhysicsCollisionShape::SHAPE_MESH;
    if (strcmp(s, "SHAPE_HEIGHTFIELD") == 0)
        return PhysicsCollisionShape::SHAPE_HEIGHTFIELD;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsCollisionShape::Type.", s);
    return PhysicsCollisionShape::SHAPE_BOX;
}

std::string lua_stringFromEnum_PhysicsCollisionShapeType(PhysicsCollisionShape::Type e)
{
    if (e == PhysicsCollisionShape::SHAPE_BOX)
        return std::string("SHAPE_BOX");
    if (e == PhysicsCollisionShape::SHAPE_SPHERE)
        return std::string("SHAPE_SPHERE");
    if (e == PhysicsCollisionShape::SHAPE_CAPSULE)
        return std::string("SHAPE_CAPSULE");
    if (e == PhysicsCollisionShape::SHAPE_MESH)
        return std::string("SHAPE_MESH");
    if (e == PhysicsCollisionShape::SHAPE_HEIGHTFIELD)
        return std::string("SHAPE_HEIGHTFIELD");
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsCollisionShape::Type.", e);
    return std::string();
}

PhysicsController::Listener::EventType lua_enumFromString_PhysicsControllerListenerEventType(const char* s)
{
    if (strcmp(s, "ACTIVATED") == 0)
        return PhysicsController::Listener::ACTIVATED;
    if (strcmp(s, "DEACTIVATED") == 0)
        return PhysicsController::Listener::DEACTIVATED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsController::Listener::EventType.", s);
    return PhysicsController::Listener::ACTIVATED;
}

std::string lua_stringFromEnum_PhysicsControllerListenerEventType(PhysicsController::Listener::EventType e)
{
    if (e == PhysicsController::Listener::ACTIVATED)
        return std::string("ACTIVATED");
    if (e == PhysicsController::Listener::DEACTIVATED)
        return std::string("DEACTIVATED");
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsController::Listener::EventType.", e);
    return std::string();
}

Properties::Type lua_enumFromString_PropertiesType(const char* s)
{
    if (strcmp(s, "NONE") == 0)
        return Properties::NONE;
    if (strcmp(s, "STRING") == 0)
        return Properties::STRING;
    if (strcmp(s, "NUMBER") == 0)
        return Properties::NUMBER;
    if (strcmp(s, "VECTOR2") == 0)
        return Properties::VECTOR2;
    if (strcmp(s, "VECTOR3") == 0)
        return Properties::VECTOR3;
    if (strcmp(s, "VECTOR4") == 0)
        return Properties::VECTOR4;
    if (strcmp(s, "MATRIX") == 0)
        return Properties::MATRIX;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Properties::Type.", s);
    return Properties::NONE;
}

std::string lua_stringFromEnum_PropertiesType(Properties::Type e)
{
    if (e == Properties::NONE)
        return std::string("NONE");
    if (e == Properties::STRING)
        return std::string("STRING");
    if (e == Properties::NUMBER)
        return std::string("NUMBER");
    if (e == Properties::VECTOR2)
        return std::string("VECTOR2");
    if (e == Properties::VECTOR3)
        return std::string("VECTOR3");
    if (e == Properties::VECTOR4)
        return std::string("VECTOR4");
    if (e == Properties::MATRIX)
        return std::string("MATRIX");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Properties::Type.", e);
    return std::string();
}

RenderState::AutoBinding lua_enumFromString_RenderStateAutoBinding(const char* s)
{
    if (strcmp(s, "NONE") == 0)
        return RenderState::NONE;
    if (strcmp(s, "WORLD_MATRIX") == 0)
        return RenderState::WORLD_MATRIX;
    if (strcmp(s, "VIEW_MATRIX") == 0)
        return RenderState::VIEW_MATRIX;
    if (strcmp(s, "PROJECTION_MATRIX") == 0)
        return RenderState::PROJECTION_MATRIX;
    if (strcmp(s, "WORLD_VIEW_MATRIX") == 0)
        return RenderState::WORLD_VIEW_MATRIX;
    if (strcmp(s, "VIEW_PROJECTION_MATRIX") == 0)
        return RenderState::VIEW_PROJECTION_MATRIX;
    if (strcmp(s, "WORLD_VIEW_PROJECTION_MATRIX") == 0)
        return RenderState::WORLD_VIEW_PROJECTION_MATRIX;
    if (strcmp(s, "INVERSE_TRANSPOSE_WORLD_MATRIX") == 0)
        return RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX;
    if (strcmp(s, "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX") == 0)
        return RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    if (strcmp(s, "CAMERA_WORLD_POSITION") == 0)
        return RenderState::CAMERA_WORLD_POSITION;
    if (strcmp(s, "CAMERA_VIEW_POSITION") == 0)
        return RenderState::CAMERA_VIEW_POSITION;
    if (strcmp(s, "MATRIX_PALETTE") == 0)
        return RenderState::MATRIX_PALETTE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration RenderState::AutoBinding.", s);
    return RenderState::NONE;
}

std::string lua_stringFromEnum_RenderStateAutoBinding(RenderState::AutoBinding e)
{
    if (e == RenderState::NONE)
        return std::string("NONE");
    if (e == RenderState::WORLD_MATRIX)
        return std::string("WORLD_MATRIX");
    if (e == RenderState::VIEW_MATRIX)
        return std::string("VIEW_MATRIX");
    if (e == RenderState::PROJECTION_MATRIX)
        return std::string("PROJECTION_MATRIX");
    if (e == RenderState::WORLD_VIEW_MATRIX)
        return std::string("WORLD_VIEW_MATRIX");
    if (e == RenderState::VIEW_PROJECTION_MATRIX)
        return std::string("VIEW_PROJECTION_MATRIX");
    if (e == RenderState::WORLD_VIEW_PROJECTION_MATRIX)
        return std::string("WORLD_VIEW_PROJECTION_MATRIX");
    if (e == RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX)
        return std::string("INVERSE_TRANSPOSE_WORLD_MATRIX");
    if (e == RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
        return std::string("INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");
    if (e == RenderState::CAMERA_WORLD_POSITION)
        return std::string("CAMERA_WORLD_POSITION");
    if (e == RenderState::CAMERA_VIEW_POSITION)
        return std::string("CAMERA_VIEW_POSITION");
    if (e == RenderState::MATRIX_PALETTE)
        return std::string("MATRIX_PALETTE");
    GP_ERROR("Invalid enumeration value '%d' for enumeration RenderState::AutoBinding.", e);
    return std::string();
}

RenderState::Blend lua_enumFromString_RenderStateBlend(const char* s)
{
    if (strcmp(s, "BLEND_ZERO") == 0)
        return RenderState::BLEND_ZERO;
    if (strcmp(s, "BLEND_ONE") == 0)
        return RenderState::BLEND_ONE;
    if (strcmp(s, "BLEND_SRC_COLOR") == 0)
        return RenderState::BLEND_SRC_COLOR;
    if (strcmp(s, "BLEN_ONE_MINUS_SRC_COLOR") == 0)
        return RenderState::BLEN_ONE_MINUS_SRC_COLOR;
    if (strcmp(s, "BLEND_DST_COLOR") == 0)
        return RenderState::BLEND_DST_COLOR;
    if (strcmp(s, "BLEND_ONE_MINUS_DST_COLOR") == 0)
        return RenderState::BLEND_ONE_MINUS_DST_COLOR;
    if (strcmp(s, "BLEND_SRC_ALPHA") == 0)
        return RenderState::BLEND_SRC_ALPHA;
    if (strcmp(s, "BLEND_ONE_MINUS_SRC_ALPHA") == 0)
        return RenderState::BLEND_ONE_MINUS_SRC_ALPHA;
    if (strcmp(s, "BLEND_DST_ALPHA") == 0)
        return RenderState::BLEND_DST_ALPHA;
    if (strcmp(s, "BLEND_ONE_MINUS_DST_ALPHA") == 0)
        return RenderState::BLEND_ONE_MINUS_DST_ALPHA;
    if (strcmp(s, "BLEND_CONSTANT_ALPHA") == 0)
        return RenderState::BLEND_CONSTANT_ALPHA;
    if (strcmp(s, "BLEND_ONE_MINUS_CONSTANT_ALPHA") == 0)
        return RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA;
    if (strcmp(s, "BLEND_SRC_ALPHA_SATURATE") == 0)
        return RenderState::BLEND_SRC_ALPHA_SATURATE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration RenderState::Blend.", s);
    return RenderState::BLEND_ZERO;
}

std::string lua_stringFromEnum_RenderStateBlend(RenderState::Blend e)
{
    if (e == RenderState::BLEND_ZERO)
        return std::string("BLEND_ZERO");
    if (e == RenderState::BLEND_ONE)
        return std::string("BLEND_ONE");
    if (e == RenderState::BLEND_SRC_COLOR)
        return std::string("BLEND_SRC_COLOR");
    if (e == RenderState::BLEN_ONE_MINUS_SRC_COLOR)
        return std::string("BLEN_ONE_MINUS_SRC_COLOR");
    if (e == RenderState::BLEND_DST_COLOR)
        return std::string("BLEND_DST_COLOR");
    if (e == RenderState::BLEND_ONE_MINUS_DST_COLOR)
        return std::string("BLEND_ONE_MINUS_DST_COLOR");
    if (e == RenderState::BLEND_SRC_ALPHA)
        return std::string("BLEND_SRC_ALPHA");
    if (e == RenderState::BLEND_ONE_MINUS_SRC_ALPHA)
        return std::string("BLEND_ONE_MINUS_SRC_ALPHA");
    if (e == RenderState::BLEND_DST_ALPHA)
        return std::string("BLEND_DST_ALPHA");
    if (e == RenderState::BLEND_ONE_MINUS_DST_ALPHA)
        return std::string("BLEND_ONE_MINUS_DST_ALPHA");
    if (e == RenderState::BLEND_CONSTANT_ALPHA)
        return std::string("BLEND_CONSTANT_ALPHA");
    if (e == RenderState::BLEND_ONE_MINUS_CONSTANT_ALPHA)
        return std::string("BLEND_ONE_MINUS_CONSTANT_ALPHA");
    if (e == RenderState::BLEND_SRC_ALPHA_SATURATE)
        return std::string("BLEND_SRC_ALPHA_SATURATE");
    GP_ERROR("Invalid enumeration value '%d' for enumeration RenderState::Blend.", e);
    return std::string();
}

Scene::DebugFlags lua_enumFromString_SceneDebugFlags(const char* s)
{
    if (strcmp(s, "DEBUG_BOXES") == 0)
        return Scene::DEBUG_BOXES;
    if (strcmp(s, "DEBUG_SPHERES") == 0)
        return Scene::DEBUG_SPHERES;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Scene::DebugFlags.", s);
    return Scene::DEBUG_BOXES;
}

std::string lua_stringFromEnum_SceneDebugFlags(Scene::DebugFlags e)
{
    if (e == Scene::DEBUG_BOXES)
        return std::string("DEBUG_BOXES");
    if (e == Scene::DEBUG_SPHERES)
        return std::string("DEBUG_SPHERES");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Scene::DebugFlags.", e);
    return std::string();
}

Texture::Filter lua_enumFromString_TextureFilter(const char* s)
{
    if (strcmp(s, "NEAREST") == 0)
        return Texture::NEAREST;
    if (strcmp(s, "LINEAR") == 0)
        return Texture::LINEAR;
    if (strcmp(s, "NEAREST_MIPMAP_NEAREST") == 0)
        return Texture::NEAREST_MIPMAP_NEAREST;
    if (strcmp(s, "LINEAR_MIPMAP_NEAREST") == 0)
        return Texture::LINEAR_MIPMAP_NEAREST;
    if (strcmp(s, "NEAREST_MIPMAP_LINEAR") == 0)
        return Texture::NEAREST_MIPMAP_LINEAR;
    if (strcmp(s, "LINEAR_MIPMAP_LINEAR") == 0)
        return Texture::LINEAR_MIPMAP_LINEAR;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Texture::Filter.", s);
    return Texture::NEAREST;
}

std::string lua_stringFromEnum_TextureFilter(Texture::Filter e)
{
    if (e == Texture::NEAREST)
        return std::string("NEAREST");
    if (e == Texture::LINEAR)
        return std::string("LINEAR");
    if (e == Texture::NEAREST_MIPMAP_NEAREST)
        return std::string("NEAREST_MIPMAP_NEAREST");
    if (e == Texture::LINEAR_MIPMAP_NEAREST)
        return std::string("LINEAR_MIPMAP_NEAREST");
    if (e == Texture::NEAREST_MIPMAP_LINEAR)
        return std::string("NEAREST_MIPMAP_LINEAR");
    if (e == Texture::LINEAR_MIPMAP_LINEAR)
        return std::string("LINEAR_MIPMAP_LINEAR");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Texture::Filter.", e);
    return std::string();
}

Texture::Format lua_enumFromString_TextureFormat(const char* s)
{
    if (strcmp(s, "RGB") == 0)
        return Texture::RGB;
    if (strcmp(s, "RGBA") == 0)
        return Texture::RGBA;
    if (strcmp(s, "ALPHA") == 0)
        return Texture::ALPHA;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Texture::Format.", s);
    return Texture::RGB;
}

std::string lua_stringFromEnum_TextureFormat(Texture::Format e)
{
    if (e == Texture::RGB)
        return std::string("RGB");
    if (e == Texture::RGBA)
        return std::string("RGBA");
    if (e == Texture::ALPHA)
        return std::string("ALPHA");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Texture::Format.", e);
    return std::string();
}

Texture::Wrap lua_enumFromString_TextureWrap(const char* s)
{
    if (strcmp(s, "REPEAT") == 0)
        return Texture::REPEAT;
    if (strcmp(s, "CLAMP") == 0)
        return Texture::CLAMP;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Texture::Wrap.", s);
    return Texture::REPEAT;
}

std::string lua_stringFromEnum_TextureWrap(Texture::Wrap e)
{
    if (e == Texture::REPEAT)
        return std::string("REPEAT");
    if (e == Texture::CLAMP)
        return std::string("CLAMP");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Texture::Wrap.", e);
    return std::string();
}

Touch::TouchEvent lua_enumFromString_TouchTouchEvent(const char* s)
{
    if (strcmp(s, "TOUCH_PRESS") == 0)
        return Touch::TOUCH_PRESS;
    if (strcmp(s, "TOUCH_RELEASE") == 0)
        return Touch::TOUCH_RELEASE;
    if (strcmp(s, "TOUCH_MOVE") == 0)
        return Touch::TOUCH_MOVE;
    GP_ERROR("Invalid enumeration value '%s' for enumeration Touch::TouchEvent.", s);
    return Touch::TOUCH_PRESS;
}

std::string lua_stringFromEnum_TouchTouchEvent(Touch::TouchEvent e)
{
    if (e == Touch::TOUCH_PRESS)
        return std::string("TOUCH_PRESS");
    if (e == Touch::TOUCH_RELEASE)
        return std::string("TOUCH_RELEASE");
    if (e == Touch::TOUCH_MOVE)
        return std::string("TOUCH_MOVE");
    GP_ERROR("Invalid enumeration value '%d' for enumeration Touch::TouchEvent.", e);
    return std::string();
}

VertexFormat::Usage lua_enumFromString_VertexFormatUsage(const char* s)
{
    if (strcmp(s, "POSITION") == 0)
        return VertexFormat::POSITION;
    if (strcmp(s, "NORMAL") == 0)
        return VertexFormat::NORMAL;
    if (strcmp(s, "COLOR") == 0)
        return VertexFormat::COLOR;
    if (strcmp(s, "TANGENT") == 0)
        return VertexFormat::TANGENT;
    if (strcmp(s, "BINORMAL") == 0)
        return VertexFormat::BINORMAL;
    if (strcmp(s, "BLENDWEIGHTS") == 0)
        return VertexFormat::BLENDWEIGHTS;
    if (strcmp(s, "BLENDINDICES") == 0)
        return VertexFormat::BLENDINDICES;
    if (strcmp(s, "TEXCOORD0") == 0)
        return VertexFormat::TEXCOORD0;
    if (strcmp(s, "TEXCOORD1") == 0)
        return VertexFormat::TEXCOORD1;
    if (strcmp(s, "TEXCOORD2") == 0)
        return VertexFormat::TEXCOORD2;
    if (strcmp(s, "TEXCOORD3") == 0)
        return VertexFormat::TEXCOORD3;
    if (strcmp(s, "TEXCOORD4") == 0)
        return VertexFormat::TEXCOORD4;
    if (strcmp(s, "TEXCOORD5") == 0)
        return VertexFormat::TEXCOORD5;
    if (strcmp(s, "TEXCOORD6") == 0)
        return VertexFormat::TEXCOORD6;
    if (strcmp(s, "TEXCOORD7") == 0)
        return VertexFormat::TEXCOORD7;
    GP_ERROR("Invalid enumeration value '%s' for enumeration VertexFormat::Usage.", s);
    return VertexFormat::POSITION;
}

std::string lua_stringFromEnum_VertexFormatUsage(VertexFormat::Usage e)
{
    if (e == VertexFormat::POSITION)
        return std::string("POSITION");
    if (e == VertexFormat::NORMAL)
        return std::string("NORMAL");
    if (e == VertexFormat::COLOR)
        return std::string("COLOR");
    if (e == VertexFormat::TANGENT)
        return std::string("TANGENT");
    if (e == VertexFormat::BINORMAL)
        return std::string("BINORMAL");
    if (e == VertexFormat::BLENDWEIGHTS)
        return std::string("BLENDWEIGHTS");
    if (e == VertexFormat::BLENDINDICES)
        return std::string("BLENDINDICES");
    if (e == VertexFormat::TEXCOORD0)
        return std::string("TEXCOORD0");
    if (e == VertexFormat::TEXCOORD1)
        return std::string("TEXCOORD1");
    if (e == VertexFormat::TEXCOORD2)
        return std::string("TEXCOORD2");
    if (e == VertexFormat::TEXCOORD3)
        return std::string("TEXCOORD3");
    if (e == VertexFormat::TEXCOORD4)
        return std::string("TEXCOORD4");
    if (e == VertexFormat::TEXCOORD5)
        return std::string("TEXCOORD5");
    if (e == VertexFormat::TEXCOORD6)
        return std::string("TEXCOORD6");
    if (e == VertexFormat::TEXCOORD7)
        return std::string("TEXCOORD7");
    GP_ERROR("Invalid enumeration value '%d' for enumeration VertexFormat::Usage.", e);
    return std::string();
}

}
