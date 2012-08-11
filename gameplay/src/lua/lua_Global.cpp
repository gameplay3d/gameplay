#include "ScriptController.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_lua_Global()
{
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Button");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "CheckBox");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Container");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Control");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Form");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Joint");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Joystick");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Label");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "MaterialParameter");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Node");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "RadioButton");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Slider");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "TextBox");
    ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Transform");
    ScriptUtil::setGlobalHierarchyPair("Button", "CheckBox");
    ScriptUtil::setGlobalHierarchyPair("Button", "RadioButton");
    ScriptUtil::setGlobalHierarchyPair("Container", "Form");
    ScriptUtil::setGlobalHierarchyPair("Control", "Button");
    ScriptUtil::setGlobalHierarchyPair("Control", "CheckBox");
    ScriptUtil::setGlobalHierarchyPair("Control", "Container");
    ScriptUtil::setGlobalHierarchyPair("Control", "Form");
    ScriptUtil::setGlobalHierarchyPair("Control", "Joystick");
    ScriptUtil::setGlobalHierarchyPair("Control", "Label");
    ScriptUtil::setGlobalHierarchyPair("Control", "RadioButton");
    ScriptUtil::setGlobalHierarchyPair("Control", "Slider");
    ScriptUtil::setGlobalHierarchyPair("Control", "TextBox");
    ScriptUtil::setGlobalHierarchyPair("Label", "Button");
    ScriptUtil::setGlobalHierarchyPair("Label", "CheckBox");
    ScriptUtil::setGlobalHierarchyPair("Label", "RadioButton");
    ScriptUtil::setGlobalHierarchyPair("Label", "Slider");
    ScriptUtil::setGlobalHierarchyPair("Label", "TextBox");
    ScriptUtil::setGlobalHierarchyPair("Layout", "AbsoluteLayout");
    ScriptUtil::setGlobalHierarchyPair("Layout", "FlowLayout");
    ScriptUtil::setGlobalHierarchyPair("Layout", "VerticalLayout");
    ScriptUtil::setGlobalHierarchyPair("Node", "Joint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsCharacter");
    ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsGhostObject");
    ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsRigidBody");
    ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsFixedConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsGenericConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsHingeConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsSocketConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsSpringConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsGenericConstraint", "PhysicsFixedConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsGenericConstraint", "PhysicsSpringConstraint");
    ScriptUtil::setGlobalHierarchyPair("PhysicsGhostObject", "PhysicsCharacter");
    ScriptUtil::setGlobalHierarchyPair("Ref", "AIAgent");
    ScriptUtil::setGlobalHierarchyPair("Ref", "AIState");
    ScriptUtil::setGlobalHierarchyPair("Ref", "AbsoluteLayout");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Animation");
    ScriptUtil::setGlobalHierarchyPair("Ref", "AnimationClip");
    ScriptUtil::setGlobalHierarchyPair("Ref", "AudioBuffer");
    ScriptUtil::setGlobalHierarchyPair("Ref", "AudioSource");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Bundle");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Button");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Camera");
    ScriptUtil::setGlobalHierarchyPair("Ref", "CheckBox");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Container");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Control");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Curve");
    ScriptUtil::setGlobalHierarchyPair("Ref", "DepthStencilTarget");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Effect");
    ScriptUtil::setGlobalHierarchyPair("Ref", "FlowLayout");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Font");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Form");
    ScriptUtil::setGlobalHierarchyPair("Ref", "FrameBuffer");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Image");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Joint");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Joystick");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Label");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Layout");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Light");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Material");
    ScriptUtil::setGlobalHierarchyPair("Ref", "MaterialParameter");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Mesh");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Model");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Node");
    ScriptUtil::setGlobalHierarchyPair("Ref", "ParticleEmitter");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Pass");
    ScriptUtil::setGlobalHierarchyPair("Ref", "PhysicsCollisionShape");
    ScriptUtil::setGlobalHierarchyPair("Ref", "RadioButton");
    ScriptUtil::setGlobalHierarchyPair("Ref", "RenderState");
    ScriptUtil::setGlobalHierarchyPair("Ref", "RenderState::StateBlock");
    ScriptUtil::setGlobalHierarchyPair("Ref", "RenderTarget");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Scene");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Slider");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Technique");
    ScriptUtil::setGlobalHierarchyPair("Ref", "TextBox");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Texture");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Texture::Sampler");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Theme");
    ScriptUtil::setGlobalHierarchyPair("Ref", "Theme::ThemeImage");
    ScriptUtil::setGlobalHierarchyPair("Ref", "VertexAttributeBinding");
    ScriptUtil::setGlobalHierarchyPair("Ref", "VerticalLayout");
    ScriptUtil::setGlobalHierarchyPair("RenderState", "Material");
    ScriptUtil::setGlobalHierarchyPair("RenderState", "Pass");
    ScriptUtil::setGlobalHierarchyPair("RenderState", "Technique");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "AIAgent");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "AIState");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Button");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "CheckBox");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Container");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Control");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Form");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Joint");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Joystick");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Label");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Node");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "PhysicsController");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "RadioButton");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Slider");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "TextBox");
    ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Transform");
    ScriptUtil::setGlobalHierarchyPair("Transform", "Joint");
    ScriptUtil::setGlobalHierarchyPair("Transform", "Node");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "AudioListener");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "AudioSource");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "Camera");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "MeshSkin");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "PhysicsCharacter");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "PhysicsGhostObject");
    ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "PhysicsRigidBody");
    ScriptUtil::addStringFromEnumConversionFunction(&gameplay::lua_stringFromEnumGlobal);

    // Register enumeration AIMessage::ParameterType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AIMessage");
        ScriptUtil::registerConstantString("UNDEFINED", "UNDEFINED", scopePath);
        ScriptUtil::registerConstantString("INTEGER", "INTEGER", scopePath);
        ScriptUtil::registerConstantString("LONG", "LONG", scopePath);
        ScriptUtil::registerConstantString("FLOAT", "FLOAT", scopePath);
        ScriptUtil::registerConstantString("DOUBLE", "DOUBLE", scopePath);
        ScriptUtil::registerConstantString("BOOLEAN", "BOOLEAN", scopePath);
        ScriptUtil::registerConstantString("STRING", "STRING", scopePath);
    }

    // Register enumeration AnimationClip::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AnimationClip");
        scopePath.push_back("Listener");
        ScriptUtil::registerConstantString("BEGIN", "BEGIN", scopePath);
        ScriptUtil::registerConstantString("END", "END", scopePath);
        ScriptUtil::registerConstantString("TIME", "TIME", scopePath);
    }

    // Register enumeration AudioSource::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AudioSource");
        ScriptUtil::registerConstantString("INITIAL", "INITIAL", scopePath);
        ScriptUtil::registerConstantString("PLAYING", "PLAYING", scopePath);
        ScriptUtil::registerConstantString("PAUSED", "PAUSED", scopePath);
        ScriptUtil::registerConstantString("STOPPED", "STOPPED", scopePath);
    }

    // Register enumeration Camera::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Camera");
        ScriptUtil::registerConstantString("PERSPECTIVE", "PERSPECTIVE", scopePath);
        ScriptUtil::registerConstantString("ORTHOGRAPHIC", "ORTHOGRAPHIC", scopePath);
    }

    // Register enumeration Container::Scroll.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Container");
        ScriptUtil::registerConstantString("SCROLL_NONE", "SCROLL_NONE", scopePath);
        ScriptUtil::registerConstantString("SCROLL_HORIZONTAL", "SCROLL_HORIZONTAL", scopePath);
        ScriptUtil::registerConstantString("SCROLL_VERTICAL", "SCROLL_VERTICAL", scopePath);
        ScriptUtil::registerConstantString("SCROLL_BOTH", "SCROLL_BOTH", scopePath);
    }

    // Register enumeration Control::Alignment.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        ScriptUtil::registerConstantString("ALIGN_LEFT", "ALIGN_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_HCENTER", "ALIGN_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_RIGHT", "ALIGN_RIGHT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP", "ALIGN_TOP", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER", "ALIGN_VCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM", "ALIGN_BOTTOM", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP_LEFT", "ALIGN_TOP_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER_LEFT", "ALIGN_VCENTER_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM_LEFT", "ALIGN_BOTTOM_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP_HCENTER", "ALIGN_TOP_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER_HCENTER", "ALIGN_VCENTER_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM_HCENTER", "ALIGN_BOTTOM_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP_RIGHT", "ALIGN_TOP_RIGHT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER_RIGHT", "ALIGN_VCENTER_RIGHT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM_RIGHT", "ALIGN_BOTTOM_RIGHT", scopePath);
    }

    // Register enumeration Control::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        scopePath.push_back("Listener");
        ScriptUtil::registerConstantString("PRESS", "PRESS", scopePath);
        ScriptUtil::registerConstantString("RELEASE", "RELEASE", scopePath);
        ScriptUtil::registerConstantString("CLICK", "CLICK", scopePath);
        ScriptUtil::registerConstantString("VALUE_CHANGED", "VALUE_CHANGED", scopePath);
        ScriptUtil::registerConstantString("TEXT_CHANGED", "TEXT_CHANGED", scopePath);
        ScriptUtil::registerConstantString("MIDDLE_CLICK", "MIDDLE_CLICK", scopePath);
        ScriptUtil::registerConstantString("RIGHT_CLICK", "RIGHT_CLICK", scopePath);
    }

    // Register enumeration Control::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        ScriptUtil::registerConstantString("NORMAL", "NORMAL", scopePath);
        ScriptUtil::registerConstantString("FOCUS", "FOCUS", scopePath);
        ScriptUtil::registerConstantString("ACTIVE", "ACTIVE", scopePath);
        ScriptUtil::registerConstantString("DISABLED", "DISABLED", scopePath);
    }

    // Register enumeration Curve::InterpolationType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Curve");
        ScriptUtil::registerConstantString("BEZIER", "BEZIER", scopePath);
        ScriptUtil::registerConstantString("BSPLINE", "BSPLINE", scopePath);
        ScriptUtil::registerConstantString("FLAT", "FLAT", scopePath);
        ScriptUtil::registerConstantString("HERMITE", "HERMITE", scopePath);
        ScriptUtil::registerConstantString("LINEAR", "LINEAR", scopePath);
        ScriptUtil::registerConstantString("SMOOTH", "SMOOTH", scopePath);
        ScriptUtil::registerConstantString("STEP", "STEP", scopePath);
        ScriptUtil::registerConstantString("QUADRATIC_IN", "QUADRATIC_IN", scopePath);
        ScriptUtil::registerConstantString("QUADRATIC_OUT", "QUADRATIC_OUT", scopePath);
        ScriptUtil::registerConstantString("QUADRATIC_IN_OUT", "QUADRATIC_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("QUADRATIC_OUT_IN", "QUADRATIC_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("CUBIC_IN", "CUBIC_IN", scopePath);
        ScriptUtil::registerConstantString("CUBIC_OUT", "CUBIC_OUT", scopePath);
        ScriptUtil::registerConstantString("CUBIC_IN_OUT", "CUBIC_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("CUBIC_OUT_IN", "CUBIC_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("QUARTIC_IN", "QUARTIC_IN", scopePath);
        ScriptUtil::registerConstantString("QUARTIC_OUT", "QUARTIC_OUT", scopePath);
        ScriptUtil::registerConstantString("QUARTIC_IN_OUT", "QUARTIC_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("QUARTIC_OUT_IN", "QUARTIC_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("QUINTIC_IN", "QUINTIC_IN", scopePath);
        ScriptUtil::registerConstantString("QUINTIC_OUT", "QUINTIC_OUT", scopePath);
        ScriptUtil::registerConstantString("QUINTIC_IN_OUT", "QUINTIC_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("QUINTIC_OUT_IN", "QUINTIC_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("SINE_IN", "SINE_IN", scopePath);
        ScriptUtil::registerConstantString("SINE_OUT", "SINE_OUT", scopePath);
        ScriptUtil::registerConstantString("SINE_IN_OUT", "SINE_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("SINE_OUT_IN", "SINE_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("EXPONENTIAL_IN", "EXPONENTIAL_IN", scopePath);
        ScriptUtil::registerConstantString("EXPONENTIAL_OUT", "EXPONENTIAL_OUT", scopePath);
        ScriptUtil::registerConstantString("EXPONENTIAL_IN_OUT", "EXPONENTIAL_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("EXPONENTIAL_OUT_IN", "EXPONENTIAL_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("CIRCULAR_IN", "CIRCULAR_IN", scopePath);
        ScriptUtil::registerConstantString("CIRCULAR_OUT", "CIRCULAR_OUT", scopePath);
        ScriptUtil::registerConstantString("CIRCULAR_IN_OUT", "CIRCULAR_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("CIRCULAR_OUT_IN", "CIRCULAR_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("ELASTIC_IN", "ELASTIC_IN", scopePath);
        ScriptUtil::registerConstantString("ELASTIC_OUT", "ELASTIC_OUT", scopePath);
        ScriptUtil::registerConstantString("ELASTIC_IN_OUT", "ELASTIC_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("ELASTIC_OUT_IN", "ELASTIC_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("OVERSHOOT_IN", "OVERSHOOT_IN", scopePath);
        ScriptUtil::registerConstantString("OVERSHOOT_OUT", "OVERSHOOT_OUT", scopePath);
        ScriptUtil::registerConstantString("OVERSHOOT_IN_OUT", "OVERSHOOT_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("OVERSHOOT_OUT_IN", "OVERSHOOT_OUT_IN", scopePath);
        ScriptUtil::registerConstantString("BOUNCE_IN", "BOUNCE_IN", scopePath);
        ScriptUtil::registerConstantString("BOUNCE_OUT", "BOUNCE_OUT", scopePath);
        ScriptUtil::registerConstantString("BOUNCE_IN_OUT", "BOUNCE_IN_OUT", scopePath);
        ScriptUtil::registerConstantString("BOUNCE_OUT_IN", "BOUNCE_OUT_IN", scopePath);
    }

    // Register enumeration DepthStencilTarget::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("DepthStencilTarget");
        ScriptUtil::registerConstantString("DEPTH", "DEPTH", scopePath);
        ScriptUtil::registerConstantString("DEPTH_STENCIL", "DEPTH_STENCIL", scopePath);
    }

    // Register enumeration Font::Justify.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Font");
        ScriptUtil::registerConstantString("ALIGN_LEFT", "ALIGN_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_HCENTER", "ALIGN_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_RIGHT", "ALIGN_RIGHT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP", "ALIGN_TOP", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER", "ALIGN_VCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM", "ALIGN_BOTTOM", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP_LEFT", "ALIGN_TOP_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER_LEFT", "ALIGN_VCENTER_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM_LEFT", "ALIGN_BOTTOM_LEFT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP_HCENTER", "ALIGN_TOP_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER_HCENTER", "ALIGN_VCENTER_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM_HCENTER", "ALIGN_BOTTOM_HCENTER", scopePath);
        ScriptUtil::registerConstantString("ALIGN_TOP_RIGHT", "ALIGN_TOP_RIGHT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_VCENTER_RIGHT", "ALIGN_VCENTER_RIGHT", scopePath);
        ScriptUtil::registerConstantString("ALIGN_BOTTOM_RIGHT", "ALIGN_BOTTOM_RIGHT", scopePath);
    }

    // Register enumeration Font::Style.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Font");
        ScriptUtil::registerConstantString("PLAIN", "PLAIN", scopePath);
        ScriptUtil::registerConstantString("BOLD", "BOLD", scopePath);
        ScriptUtil::registerConstantString("ITALIC", "ITALIC", scopePath);
        ScriptUtil::registerConstantString("BOLD_ITALIC", "BOLD_ITALIC", scopePath);
    }

    // Register enumeration Game::ClearFlags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Game");
        ScriptUtil::registerConstantString("CLEAR_COLOR", "CLEAR_COLOR", scopePath);
        ScriptUtil::registerConstantString("CLEAR_DEPTH", "CLEAR_DEPTH", scopePath);
        ScriptUtil::registerConstantString("CLEAR_STENCIL", "CLEAR_STENCIL", scopePath);
        ScriptUtil::registerConstantString("CLEAR_COLOR_DEPTH", "CLEAR_COLOR_DEPTH", scopePath);
        ScriptUtil::registerConstantString("CLEAR_COLOR_STENCIL", "CLEAR_COLOR_STENCIL", scopePath);
        ScriptUtil::registerConstantString("CLEAR_DEPTH_STENCIL", "CLEAR_DEPTH_STENCIL", scopePath);
        ScriptUtil::registerConstantString("CLEAR_COLOR_DEPTH_STENCIL", "CLEAR_COLOR_DEPTH_STENCIL", scopePath);
    }

    // Register enumeration Game::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Game");
        ScriptUtil::registerConstantString("UNINITIALIZED", "UNINITIALIZED", scopePath);
        ScriptUtil::registerConstantString("RUNNING", "RUNNING", scopePath);
        ScriptUtil::registerConstantString("PAUSED", "PAUSED", scopePath);
    }

    // Register enumeration Gamepad::ButtonState.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gamepad");
        ScriptUtil::registerConstantString("BUTTON_PRESSED", "BUTTON_PRESSED", scopePath);
        ScriptUtil::registerConstantString("BUTTON_RELEASED", "BUTTON_RELEASED", scopePath);
    }

    // Register enumeration Gamepad::GamepadEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gamepad");
        ScriptUtil::registerConstantString("CONNECTED_EVENT", "CONNECTED_EVENT", scopePath);
        ScriptUtil::registerConstantString("DISCONNECTED_EVENT", "DISCONNECTED_EVENT", scopePath);
    }

    // Register enumeration Image::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Image");
        ScriptUtil::registerConstantString("RGB", "RGB", scopePath);
        ScriptUtil::registerConstantString("RGBA", "RGBA", scopePath);
    }

    // Register enumeration Keyboard::Key.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Keyboard");
        ScriptUtil::registerConstantString("KEY_NONE", "KEY_NONE", scopePath);
        ScriptUtil::registerConstantString("KEY_PAUSE", "KEY_PAUSE", scopePath);
        ScriptUtil::registerConstantString("KEY_SCROLL_LOCK", "KEY_SCROLL_LOCK", scopePath);
        ScriptUtil::registerConstantString("KEY_PRINT", "KEY_PRINT", scopePath);
        ScriptUtil::registerConstantString("KEY_SYSREQ", "KEY_SYSREQ", scopePath);
        ScriptUtil::registerConstantString("KEY_BREAK", "KEY_BREAK", scopePath);
        ScriptUtil::registerConstantString("KEY_ESCAPE", "KEY_ESCAPE", scopePath);
        ScriptUtil::registerConstantString("KEY_BACKSPACE", "KEY_BACKSPACE", scopePath);
        ScriptUtil::registerConstantString("KEY_TAB", "KEY_TAB", scopePath);
        ScriptUtil::registerConstantString("KEY_BACK_TAB", "KEY_BACK_TAB", scopePath);
        ScriptUtil::registerConstantString("KEY_RETURN", "KEY_RETURN", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPS_LOCK", "KEY_CAPS_LOCK", scopePath);
        ScriptUtil::registerConstantString("KEY_SHIFT", "KEY_SHIFT", scopePath);
        ScriptUtil::registerConstantString("KEY_CTRL", "KEY_CTRL", scopePath);
        ScriptUtil::registerConstantString("KEY_ALT", "KEY_ALT", scopePath);
        ScriptUtil::registerConstantString("KEY_MENU", "KEY_MENU", scopePath);
        ScriptUtil::registerConstantString("KEY_HYPER", "KEY_HYPER", scopePath);
        ScriptUtil::registerConstantString("KEY_INSERT", "KEY_INSERT", scopePath);
        ScriptUtil::registerConstantString("KEY_HOME", "KEY_HOME", scopePath);
        ScriptUtil::registerConstantString("KEY_PG_UP", "KEY_PG_UP", scopePath);
        ScriptUtil::registerConstantString("KEY_DELETE", "KEY_DELETE", scopePath);
        ScriptUtil::registerConstantString("KEY_END", "KEY_END", scopePath);
        ScriptUtil::registerConstantString("KEY_PG_DOWN", "KEY_PG_DOWN", scopePath);
        ScriptUtil::registerConstantString("KEY_LEFT_ARROW", "KEY_LEFT_ARROW", scopePath);
        ScriptUtil::registerConstantString("KEY_RIGHT_ARROW", "KEY_RIGHT_ARROW", scopePath);
        ScriptUtil::registerConstantString("KEY_UP_ARROW", "KEY_UP_ARROW", scopePath);
        ScriptUtil::registerConstantString("KEY_DOWN_ARROW", "KEY_DOWN_ARROW", scopePath);
        ScriptUtil::registerConstantString("KEY_NUM_LOCK", "KEY_NUM_LOCK", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_PLUS", "KEY_KP_PLUS", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_MINUS", "KEY_KP_MINUS", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_MULTIPLY", "KEY_KP_MULTIPLY", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_DIVIDE", "KEY_KP_DIVIDE", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_ENTER", "KEY_KP_ENTER", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_HOME", "KEY_KP_HOME", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_UP", "KEY_KP_UP", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_PG_UP", "KEY_KP_PG_UP", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_LEFT", "KEY_KP_LEFT", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_FIVE", "KEY_KP_FIVE", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_RIGHT", "KEY_KP_RIGHT", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_END", "KEY_KP_END", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_DOWN", "KEY_KP_DOWN", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_PG_DOWN", "KEY_KP_PG_DOWN", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_INSERT", "KEY_KP_INSERT", scopePath);
        ScriptUtil::registerConstantString("KEY_KP_DELETE", "KEY_KP_DELETE", scopePath);
        ScriptUtil::registerConstantString("KEY_F1", "KEY_F1", scopePath);
        ScriptUtil::registerConstantString("KEY_F2", "KEY_F2", scopePath);
        ScriptUtil::registerConstantString("KEY_F3", "KEY_F3", scopePath);
        ScriptUtil::registerConstantString("KEY_F4", "KEY_F4", scopePath);
        ScriptUtil::registerConstantString("KEY_F5", "KEY_F5", scopePath);
        ScriptUtil::registerConstantString("KEY_F6", "KEY_F6", scopePath);
        ScriptUtil::registerConstantString("KEY_F7", "KEY_F7", scopePath);
        ScriptUtil::registerConstantString("KEY_F8", "KEY_F8", scopePath);
        ScriptUtil::registerConstantString("KEY_F9", "KEY_F9", scopePath);
        ScriptUtil::registerConstantString("KEY_F10", "KEY_F10", scopePath);
        ScriptUtil::registerConstantString("KEY_F11", "KEY_F11", scopePath);
        ScriptUtil::registerConstantString("KEY_F12", "KEY_F12", scopePath);
        ScriptUtil::registerConstantString("KEY_SPACE", "KEY_SPACE", scopePath);
        ScriptUtil::registerConstantString("KEY_EXCLAM", "KEY_EXCLAM", scopePath);
        ScriptUtil::registerConstantString("KEY_QUOTE", "KEY_QUOTE", scopePath);
        ScriptUtil::registerConstantString("KEY_NUMBER", "KEY_NUMBER", scopePath);
        ScriptUtil::registerConstantString("KEY_DOLLAR", "KEY_DOLLAR", scopePath);
        ScriptUtil::registerConstantString("KEY_PERCENT", "KEY_PERCENT", scopePath);
        ScriptUtil::registerConstantString("KEY_CIRCUMFLEX", "KEY_CIRCUMFLEX", scopePath);
        ScriptUtil::registerConstantString("KEY_AMPERSAND", "KEY_AMPERSAND", scopePath);
        ScriptUtil::registerConstantString("KEY_APOSTROPHE", "KEY_APOSTROPHE", scopePath);
        ScriptUtil::registerConstantString("KEY_LEFT_PARENTHESIS", "KEY_LEFT_PARENTHESIS", scopePath);
        ScriptUtil::registerConstantString("KEY_RIGHT_PARENTHESIS", "KEY_RIGHT_PARENTHESIS", scopePath);
        ScriptUtil::registerConstantString("KEY_ASTERISK", "KEY_ASTERISK", scopePath);
        ScriptUtil::registerConstantString("KEY_PLUS", "KEY_PLUS", scopePath);
        ScriptUtil::registerConstantString("KEY_COMMA", "KEY_COMMA", scopePath);
        ScriptUtil::registerConstantString("KEY_MINUS", "KEY_MINUS", scopePath);
        ScriptUtil::registerConstantString("KEY_PERIOD", "KEY_PERIOD", scopePath);
        ScriptUtil::registerConstantString("KEY_SLASH", "KEY_SLASH", scopePath);
        ScriptUtil::registerConstantString("KEY_ZERO", "KEY_ZERO", scopePath);
        ScriptUtil::registerConstantString("KEY_ONE", "KEY_ONE", scopePath);
        ScriptUtil::registerConstantString("KEY_TWO", "KEY_TWO", scopePath);
        ScriptUtil::registerConstantString("KEY_THREE", "KEY_THREE", scopePath);
        ScriptUtil::registerConstantString("KEY_FOUR", "KEY_FOUR", scopePath);
        ScriptUtil::registerConstantString("KEY_FIVE", "KEY_FIVE", scopePath);
        ScriptUtil::registerConstantString("KEY_SIX", "KEY_SIX", scopePath);
        ScriptUtil::registerConstantString("KEY_SEVEN", "KEY_SEVEN", scopePath);
        ScriptUtil::registerConstantString("KEY_EIGHT", "KEY_EIGHT", scopePath);
        ScriptUtil::registerConstantString("KEY_NINE", "KEY_NINE", scopePath);
        ScriptUtil::registerConstantString("KEY_COLON", "KEY_COLON", scopePath);
        ScriptUtil::registerConstantString("KEY_SEMICOLON", "KEY_SEMICOLON", scopePath);
        ScriptUtil::registerConstantString("KEY_LESS_THAN", "KEY_LESS_THAN", scopePath);
        ScriptUtil::registerConstantString("KEY_EQUAL", "KEY_EQUAL", scopePath);
        ScriptUtil::registerConstantString("KEY_GREATER_THAN", "KEY_GREATER_THAN", scopePath);
        ScriptUtil::registerConstantString("KEY_QUESTION", "KEY_QUESTION", scopePath);
        ScriptUtil::registerConstantString("KEY_AT", "KEY_AT", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_A", "KEY_CAPITAL_A", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_B", "KEY_CAPITAL_B", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_C", "KEY_CAPITAL_C", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_D", "KEY_CAPITAL_D", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_E", "KEY_CAPITAL_E", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_F", "KEY_CAPITAL_F", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_G", "KEY_CAPITAL_G", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_H", "KEY_CAPITAL_H", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_I", "KEY_CAPITAL_I", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_J", "KEY_CAPITAL_J", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_K", "KEY_CAPITAL_K", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_L", "KEY_CAPITAL_L", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_M", "KEY_CAPITAL_M", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_N", "KEY_CAPITAL_N", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_O", "KEY_CAPITAL_O", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_P", "KEY_CAPITAL_P", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_Q", "KEY_CAPITAL_Q", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_R", "KEY_CAPITAL_R", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_S", "KEY_CAPITAL_S", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_T", "KEY_CAPITAL_T", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_U", "KEY_CAPITAL_U", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_V", "KEY_CAPITAL_V", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_W", "KEY_CAPITAL_W", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_X", "KEY_CAPITAL_X", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_Y", "KEY_CAPITAL_Y", scopePath);
        ScriptUtil::registerConstantString("KEY_CAPITAL_Z", "KEY_CAPITAL_Z", scopePath);
        ScriptUtil::registerConstantString("KEY_LEFT_BRACKET", "KEY_LEFT_BRACKET", scopePath);
        ScriptUtil::registerConstantString("KEY_BACK_SLASH", "KEY_BACK_SLASH", scopePath);
        ScriptUtil::registerConstantString("KEY_RIGHT_BRACKET", "KEY_RIGHT_BRACKET", scopePath);
        ScriptUtil::registerConstantString("KEY_UNDERSCORE", "KEY_UNDERSCORE", scopePath);
        ScriptUtil::registerConstantString("KEY_GRAVE", "KEY_GRAVE", scopePath);
        ScriptUtil::registerConstantString("KEY_A", "KEY_A", scopePath);
        ScriptUtil::registerConstantString("KEY_B", "KEY_B", scopePath);
        ScriptUtil::registerConstantString("KEY_C", "KEY_C", scopePath);
        ScriptUtil::registerConstantString("KEY_D", "KEY_D", scopePath);
        ScriptUtil::registerConstantString("KEY_E", "KEY_E", scopePath);
        ScriptUtil::registerConstantString("KEY_F", "KEY_F", scopePath);
        ScriptUtil::registerConstantString("KEY_G", "KEY_G", scopePath);
        ScriptUtil::registerConstantString("KEY_H", "KEY_H", scopePath);
        ScriptUtil::registerConstantString("KEY_I", "KEY_I", scopePath);
        ScriptUtil::registerConstantString("KEY_J", "KEY_J", scopePath);
        ScriptUtil::registerConstantString("KEY_K", "KEY_K", scopePath);
        ScriptUtil::registerConstantString("KEY_L", "KEY_L", scopePath);
        ScriptUtil::registerConstantString("KEY_M", "KEY_M", scopePath);
        ScriptUtil::registerConstantString("KEY_N", "KEY_N", scopePath);
        ScriptUtil::registerConstantString("KEY_O", "KEY_O", scopePath);
        ScriptUtil::registerConstantString("KEY_P", "KEY_P", scopePath);
        ScriptUtil::registerConstantString("KEY_Q", "KEY_Q", scopePath);
        ScriptUtil::registerConstantString("KEY_R", "KEY_R", scopePath);
        ScriptUtil::registerConstantString("KEY_S", "KEY_S", scopePath);
        ScriptUtil::registerConstantString("KEY_T", "KEY_T", scopePath);
        ScriptUtil::registerConstantString("KEY_U", "KEY_U", scopePath);
        ScriptUtil::registerConstantString("KEY_V", "KEY_V", scopePath);
        ScriptUtil::registerConstantString("KEY_W", "KEY_W", scopePath);
        ScriptUtil::registerConstantString("KEY_X", "KEY_X", scopePath);
        ScriptUtil::registerConstantString("KEY_Y", "KEY_Y", scopePath);
        ScriptUtil::registerConstantString("KEY_Z", "KEY_Z", scopePath);
        ScriptUtil::registerConstantString("KEY_LEFT_BRACE", "KEY_LEFT_BRACE", scopePath);
        ScriptUtil::registerConstantString("KEY_BAR", "KEY_BAR", scopePath);
        ScriptUtil::registerConstantString("KEY_RIGHT_BRACE", "KEY_RIGHT_BRACE", scopePath);
        ScriptUtil::registerConstantString("KEY_TILDE", "KEY_TILDE", scopePath);
        ScriptUtil::registerConstantString("KEY_EURO", "KEY_EURO", scopePath);
        ScriptUtil::registerConstantString("KEY_POUND", "KEY_POUND", scopePath);
        ScriptUtil::registerConstantString("KEY_YEN", "KEY_YEN", scopePath);
        ScriptUtil::registerConstantString("KEY_MIDDLE_DOT", "KEY_MIDDLE_DOT", scopePath);
        ScriptUtil::registerConstantString("KEY_SEARCH", "KEY_SEARCH", scopePath);
    }

    // Register enumeration Keyboard::KeyEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Keyboard");
        ScriptUtil::registerConstantString("KEY_PRESS", "KEY_PRESS", scopePath);
        ScriptUtil::registerConstantString("KEY_RELEASE", "KEY_RELEASE", scopePath);
        ScriptUtil::registerConstantString("KEY_CHAR", "KEY_CHAR", scopePath);
    }

    // Register enumeration Layout::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Layout");
        ScriptUtil::registerConstantString("LAYOUT_FLOW", "LAYOUT_FLOW", scopePath);
        ScriptUtil::registerConstantString("LAYOUT_VERTICAL", "LAYOUT_VERTICAL", scopePath);
        ScriptUtil::registerConstantString("LAYOUT_ABSOLUTE", "LAYOUT_ABSOLUTE", scopePath);
        ScriptUtil::registerConstantString("LAYOUT_SCROLL", "LAYOUT_SCROLL", scopePath);
    }

    // Register enumeration Light::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Light");
        ScriptUtil::registerConstantString("DIRECTIONAL", "DIRECTIONAL", scopePath);
        ScriptUtil::registerConstantString("POINT", "POINT", scopePath);
        ScriptUtil::registerConstantString("SPOT", "SPOT", scopePath);
    }

    // Register enumeration Mesh::IndexFormat.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mesh");
        ScriptUtil::registerConstantString("INDEX8", "INDEX8", scopePath);
        ScriptUtil::registerConstantString("INDEX16", "INDEX16", scopePath);
        ScriptUtil::registerConstantString("INDEX32", "INDEX32", scopePath);
    }

    // Register enumeration Mesh::PrimitiveType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mesh");
        ScriptUtil::registerConstantString("TRIANGLES", "TRIANGLES", scopePath);
        ScriptUtil::registerConstantString("TRIANGLE_STRIP", "TRIANGLE_STRIP", scopePath);
        ScriptUtil::registerConstantString("LINES", "LINES", scopePath);
        ScriptUtil::registerConstantString("LINE_STRIP", "LINE_STRIP", scopePath);
        ScriptUtil::registerConstantString("POINTS", "POINTS", scopePath);
    }

    // Register enumeration Mouse::MouseEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mouse");
        ScriptUtil::registerConstantString("MOUSE_PRESS_LEFT_BUTTON", "MOUSE_PRESS_LEFT_BUTTON", scopePath);
        ScriptUtil::registerConstantString("MOUSE_RELEASE_LEFT_BUTTON", "MOUSE_RELEASE_LEFT_BUTTON", scopePath);
        ScriptUtil::registerConstantString("MOUSE_PRESS_MIDDLE_BUTTON", "MOUSE_PRESS_MIDDLE_BUTTON", scopePath);
        ScriptUtil::registerConstantString("MOUSE_RELEASE_MIDDLE_BUTTON", "MOUSE_RELEASE_MIDDLE_BUTTON", scopePath);
        ScriptUtil::registerConstantString("MOUSE_PRESS_RIGHT_BUTTON", "MOUSE_PRESS_RIGHT_BUTTON", scopePath);
        ScriptUtil::registerConstantString("MOUSE_RELEASE_RIGHT_BUTTON", "MOUSE_RELEASE_RIGHT_BUTTON", scopePath);
        ScriptUtil::registerConstantString("MOUSE_MOVE", "MOUSE_MOVE", scopePath);
        ScriptUtil::registerConstantString("MOUSE_WHEEL", "MOUSE_WHEEL", scopePath);
    }

    // Register enumeration Node::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Node");
        ScriptUtil::registerConstantString("NODE", "NODE", scopePath);
        ScriptUtil::registerConstantString("JOINT", "JOINT", scopePath);
    }

    // Register enumeration ParticleEmitter::TextureBlending.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("ParticleEmitter");
        ScriptUtil::registerConstantString("BLEND_OPAQUE", "BLEND_OPAQUE", scopePath);
        ScriptUtil::registerConstantString("BLEND_TRANSPARENT", "BLEND_TRANSPARENT", scopePath);
        ScriptUtil::registerConstantString("BLEND_ADDITIVE", "BLEND_ADDITIVE", scopePath);
        ScriptUtil::registerConstantString("BLEND_MULTIPLIED", "BLEND_MULTIPLIED", scopePath);
    }

    // Register enumeration PhysicsCollisionObject::CollisionListener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionObject");
        scopePath.push_back("CollisionListener");
        ScriptUtil::registerConstantString("COLLIDING", "COLLIDING", scopePath);
        ScriptUtil::registerConstantString("NOT_COLLIDING", "NOT_COLLIDING", scopePath);
    }

    // Register enumeration PhysicsCollisionObject::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionObject");
        ScriptUtil::registerConstantString("RIGID_BODY", "RIGID_BODY", scopePath);
        ScriptUtil::registerConstantString("CHARACTER", "CHARACTER", scopePath);
        ScriptUtil::registerConstantString("GHOST_OBJECT", "GHOST_OBJECT", scopePath);
        ScriptUtil::registerConstantString("NONE", "NONE", scopePath);
    }

    // Register enumeration PhysicsCollisionShape::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionShape");
        ScriptUtil::registerConstantString("SHAPE_BOX", "SHAPE_BOX", scopePath);
        ScriptUtil::registerConstantString("SHAPE_SPHERE", "SHAPE_SPHERE", scopePath);
        ScriptUtil::registerConstantString("SHAPE_CAPSULE", "SHAPE_CAPSULE", scopePath);
        ScriptUtil::registerConstantString("SHAPE_MESH", "SHAPE_MESH", scopePath);
        ScriptUtil::registerConstantString("SHAPE_HEIGHTFIELD", "SHAPE_HEIGHTFIELD", scopePath);
    }

    // Register enumeration PhysicsController::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsController");
        scopePath.push_back("Listener");
        ScriptUtil::registerConstantString("ACTIVATED", "ACTIVATED", scopePath);
        ScriptUtil::registerConstantString("DEACTIVATED", "DEACTIVATED", scopePath);
    }

    // Register enumeration Properties::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Properties");
        ScriptUtil::registerConstantString("NONE", "NONE", scopePath);
        ScriptUtil::registerConstantString("STRING", "STRING", scopePath);
        ScriptUtil::registerConstantString("NUMBER", "NUMBER", scopePath);
        ScriptUtil::registerConstantString("VECTOR2", "VECTOR2", scopePath);
        ScriptUtil::registerConstantString("VECTOR3", "VECTOR3", scopePath);
        ScriptUtil::registerConstantString("VECTOR4", "VECTOR4", scopePath);
        ScriptUtil::registerConstantString("MATRIX", "MATRIX", scopePath);
    }

    // Register enumeration RenderState::AutoBinding.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        ScriptUtil::registerConstantString("NONE", "NONE", scopePath);
        ScriptUtil::registerConstantString("WORLD_MATRIX", "WORLD_MATRIX", scopePath);
        ScriptUtil::registerConstantString("VIEW_MATRIX", "VIEW_MATRIX", scopePath);
        ScriptUtil::registerConstantString("PROJECTION_MATRIX", "PROJECTION_MATRIX", scopePath);
        ScriptUtil::registerConstantString("WORLD_VIEW_MATRIX", "WORLD_VIEW_MATRIX", scopePath);
        ScriptUtil::registerConstantString("VIEW_PROJECTION_MATRIX", "VIEW_PROJECTION_MATRIX", scopePath);
        ScriptUtil::registerConstantString("WORLD_VIEW_PROJECTION_MATRIX", "WORLD_VIEW_PROJECTION_MATRIX", scopePath);
        ScriptUtil::registerConstantString("INVERSE_TRANSPOSE_WORLD_MATRIX", "INVERSE_TRANSPOSE_WORLD_MATRIX", scopePath);
        ScriptUtil::registerConstantString("INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX", scopePath);
        ScriptUtil::registerConstantString("CAMERA_WORLD_POSITION", "CAMERA_WORLD_POSITION", scopePath);
        ScriptUtil::registerConstantString("CAMERA_VIEW_POSITION", "CAMERA_VIEW_POSITION", scopePath);
        ScriptUtil::registerConstantString("MATRIX_PALETTE", "MATRIX_PALETTE", scopePath);
    }

    // Register enumeration RenderState::Blend.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        ScriptUtil::registerConstantString("BLEND_ZERO", "BLEND_ZERO", scopePath);
        ScriptUtil::registerConstantString("BLEND_ONE", "BLEND_ONE", scopePath);
        ScriptUtil::registerConstantString("BLEND_SRC_COLOR", "BLEND_SRC_COLOR", scopePath);
        ScriptUtil::registerConstantString("BLEN_ONE_MINUS_SRC_COLOR", "BLEN_ONE_MINUS_SRC_COLOR", scopePath);
        ScriptUtil::registerConstantString("BLEND_DST_COLOR", "BLEND_DST_COLOR", scopePath);
        ScriptUtil::registerConstantString("BLEND_ONE_MINUS_DST_COLOR", "BLEND_ONE_MINUS_DST_COLOR", scopePath);
        ScriptUtil::registerConstantString("BLEND_SRC_ALPHA", "BLEND_SRC_ALPHA", scopePath);
        ScriptUtil::registerConstantString("BLEND_ONE_MINUS_SRC_ALPHA", "BLEND_ONE_MINUS_SRC_ALPHA", scopePath);
        ScriptUtil::registerConstantString("BLEND_DST_ALPHA", "BLEND_DST_ALPHA", scopePath);
        ScriptUtil::registerConstantString("BLEND_ONE_MINUS_DST_ALPHA", "BLEND_ONE_MINUS_DST_ALPHA", scopePath);
        ScriptUtil::registerConstantString("BLEND_CONSTANT_ALPHA", "BLEND_CONSTANT_ALPHA", scopePath);
        ScriptUtil::registerConstantString("BLEND_ONE_MINUS_CONSTANT_ALPHA", "BLEND_ONE_MINUS_CONSTANT_ALPHA", scopePath);
        ScriptUtil::registerConstantString("BLEND_SRC_ALPHA_SATURATE", "BLEND_SRC_ALPHA_SATURATE", scopePath);
    }

    // Register enumeration Scene::DebugFlags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Scene");
        ScriptUtil::registerConstantString("DEBUG_BOXES", "DEBUG_BOXES", scopePath);
        ScriptUtil::registerConstantString("DEBUG_SPHERES", "DEBUG_SPHERES", scopePath);
    }

    // Register enumeration Texture::Filter.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        ScriptUtil::registerConstantString("NEAREST", "NEAREST", scopePath);
        ScriptUtil::registerConstantString("LINEAR", "LINEAR", scopePath);
        ScriptUtil::registerConstantString("NEAREST_MIPMAP_NEAREST", "NEAREST_MIPMAP_NEAREST", scopePath);
        ScriptUtil::registerConstantString("LINEAR_MIPMAP_NEAREST", "LINEAR_MIPMAP_NEAREST", scopePath);
        ScriptUtil::registerConstantString("NEAREST_MIPMAP_LINEAR", "NEAREST_MIPMAP_LINEAR", scopePath);
        ScriptUtil::registerConstantString("LINEAR_MIPMAP_LINEAR", "LINEAR_MIPMAP_LINEAR", scopePath);
    }

    // Register enumeration Texture::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        ScriptUtil::registerConstantString("RGB", "RGB", scopePath);
        ScriptUtil::registerConstantString("RGBA", "RGBA", scopePath);
        ScriptUtil::registerConstantString("ALPHA", "ALPHA", scopePath);
    }

    // Register enumeration Texture::Wrap.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        ScriptUtil::registerConstantString("REPEAT", "REPEAT", scopePath);
        ScriptUtil::registerConstantString("CLAMP", "CLAMP", scopePath);
    }

    // Register enumeration Touch::TouchEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Touch");
        ScriptUtil::registerConstantString("TOUCH_PRESS", "TOUCH_PRESS", scopePath);
        ScriptUtil::registerConstantString("TOUCH_RELEASE", "TOUCH_RELEASE", scopePath);
        ScriptUtil::registerConstantString("TOUCH_MOVE", "TOUCH_MOVE", scopePath);
    }

    // Register enumeration VertexFormat::Usage.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("VertexFormat");
        ScriptUtil::registerConstantString("POSITION", "POSITION", scopePath);
        ScriptUtil::registerConstantString("NORMAL", "NORMAL", scopePath);
        ScriptUtil::registerConstantString("COLOR", "COLOR", scopePath);
        ScriptUtil::registerConstantString("TANGENT", "TANGENT", scopePath);
        ScriptUtil::registerConstantString("BINORMAL", "BINORMAL", scopePath);
        ScriptUtil::registerConstantString("BLENDWEIGHTS", "BLENDWEIGHTS", scopePath);
        ScriptUtil::registerConstantString("BLENDINDICES", "BLENDINDICES", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD0", "TEXCOORD0", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD1", "TEXCOORD1", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD2", "TEXCOORD2", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD3", "TEXCOORD3", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD4", "TEXCOORD4", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD5", "TEXCOORD5", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD6", "TEXCOORD6", scopePath);
        ScriptUtil::registerConstantString("TEXCOORD7", "TEXCOORD7", scopePath);
    }
}

static const char* enumStringEmpty = "";

const char* lua_stringFromEnumGlobal(std::string& enumname, unsigned int value)
{
    if (enumname == "AIMessage::ParameterType")
        return lua_stringFromEnum_AIMessageParameterType((AIMessage::ParameterType)value);
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
