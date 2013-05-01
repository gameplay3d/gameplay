#include "ScriptController.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_lua_Global()
{
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Button");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "CheckBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Container");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Control");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Form");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "ImageControl");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Joint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Joystick");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Label");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "MaterialParameter");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Node");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "RadioButton");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Slider");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "TextBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("AnimationTarget", "Transform");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Button", "CheckBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Button", "ImageControl");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Button", "RadioButton");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Container", "Form");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "Button");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "CheckBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "Container");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "Form");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "ImageControl");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "Joystick");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "Label");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "RadioButton");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "Slider");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Control", "TextBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Label", "Button");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Label", "CheckBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Label", "ImageControl");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Label", "RadioButton");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Label", "Slider");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Label", "TextBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Layout", "AbsoluteLayout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Layout", "FlowLayout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Layout", "VerticalLayout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Node", "Joint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsCharacter");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsGhostObject");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsRigidBody");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsVehicle");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject", "PhysicsVehicleWheel");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsFixedConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsGenericConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsHingeConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsSocketConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsConstraint", "PhysicsSpringConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsGenericConstraint", "PhysicsFixedConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsGenericConstraint", "PhysicsSpringConstraint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsGhostObject", "PhysicsCharacter");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "AIAgent");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "AIState");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "AbsoluteLayout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Animation");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "AnimationClip");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "AudioBuffer");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "AudioSource");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Bundle");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Button");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Camera");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "CheckBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Container");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Control");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Curve");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "DepthStencilTarget");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Effect");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "FlowLayout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Font");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Form");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "FrameBuffer");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "HeightField");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Image");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "ImageControl");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Joint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Joystick");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Label");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Layout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Light");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Material");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "MaterialParameter");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Mesh");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Model");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Node");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "ParticleEmitter");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Pass");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "PhysicsCollisionShape");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "RadioButton");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "RenderState");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "RenderState::StateBlock");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "RenderTarget");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Scene");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Slider");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Technique");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Terrain");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "TextBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Texture");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Texture::Sampler");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Theme");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Theme::ThemeImage");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "VertexAttributeBinding");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "VerticalLayout");
    gameplay::ScriptUtil::setGlobalHierarchyPair("RenderState", "Material");
    gameplay::ScriptUtil::setGlobalHierarchyPair("RenderState", "Pass");
    gameplay::ScriptUtil::setGlobalHierarchyPair("RenderState", "Technique");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "AIAgent");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "AIState");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Button");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "CheckBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Container");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Control");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Form");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "ImageControl");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Joint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Joystick");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Label");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Node");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "PhysicsController");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "RadioButton");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Slider");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "TextBox");
    gameplay::ScriptUtil::setGlobalHierarchyPair("ScriptTarget", "Transform");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform", "Joint");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform", "Node");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "AudioListener");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "AudioSource");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "Camera");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "MeshSkin");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "PhysicsCharacter");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "PhysicsGhostObject");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "PhysicsRigidBody");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Transform::Listener", "Terrain");
    gameplay::ScriptUtil::addStringFromEnumConversionFunction(&gameplay::lua_stringFromEnumGlobal);

    // Register enumeration AIMessage::ParameterType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AIMessage");
        gameplay::ScriptUtil::registerConstantString("UNDEFINED", "UNDEFINED", scopePath);
        gameplay::ScriptUtil::registerConstantString("INTEGER", "INTEGER", scopePath);
        gameplay::ScriptUtil::registerConstantString("LONG", "LONG", scopePath);
        gameplay::ScriptUtil::registerConstantString("FLOAT", "FLOAT", scopePath);
        gameplay::ScriptUtil::registerConstantString("DOUBLE", "DOUBLE", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOOLEAN", "BOOLEAN", scopePath);
        gameplay::ScriptUtil::registerConstantString("STRING", "STRING", scopePath);
    }

    // Register enumeration AnimationClip::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AnimationClip");
        scopePath.push_back("Listener");
        gameplay::ScriptUtil::registerConstantString("BEGIN", "BEGIN", scopePath);
        gameplay::ScriptUtil::registerConstantString("END", "END", scopePath);
        gameplay::ScriptUtil::registerConstantString("TIME", "TIME", scopePath);
    }

    // Register enumeration AudioSource::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("AudioSource");
        gameplay::ScriptUtil::registerConstantString("INITIAL", "INITIAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("PLAYING", "PLAYING", scopePath);
        gameplay::ScriptUtil::registerConstantString("PAUSED", "PAUSED", scopePath);
        gameplay::ScriptUtil::registerConstantString("STOPPED", "STOPPED", scopePath);
    }

    // Register enumeration Camera::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Camera");
        gameplay::ScriptUtil::registerConstantString("PERSPECTIVE", "PERSPECTIVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("ORTHOGRAPHIC", "ORTHOGRAPHIC", scopePath);
    }

    // Register enumeration Container::Scroll.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Container");
        gameplay::ScriptUtil::registerConstantString("SCROLL_NONE", "SCROLL_NONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("SCROLL_HORIZONTAL", "SCROLL_HORIZONTAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("SCROLL_VERTICAL", "SCROLL_VERTICAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("SCROLL_BOTH", "SCROLL_BOTH", scopePath);
    }

    // Register enumeration Control::Alignment.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        gameplay::ScriptUtil::registerConstantString("ALIGN_LEFT", "ALIGN_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_HCENTER", "ALIGN_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_RIGHT", "ALIGN_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP", "ALIGN_TOP", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER", "ALIGN_VCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM", "ALIGN_BOTTOM", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP_LEFT", "ALIGN_TOP_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER_LEFT", "ALIGN_VCENTER_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM_LEFT", "ALIGN_BOTTOM_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP_HCENTER", "ALIGN_TOP_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER_HCENTER", "ALIGN_VCENTER_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM_HCENTER", "ALIGN_BOTTOM_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP_RIGHT", "ALIGN_TOP_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER_RIGHT", "ALIGN_VCENTER_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM_RIGHT", "ALIGN_BOTTOM_RIGHT", scopePath);
    }

    // Register enumeration Control::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        scopePath.push_back("Listener");
        gameplay::ScriptUtil::registerConstantString("PRESS", "PRESS", scopePath);
        gameplay::ScriptUtil::registerConstantString("RELEASE", "RELEASE", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLICK", "CLICK", scopePath);
        gameplay::ScriptUtil::registerConstantString("VALUE_CHANGED", "VALUE_CHANGED", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXT_CHANGED", "TEXT_CHANGED", scopePath);
        gameplay::ScriptUtil::registerConstantString("MIDDLE_CLICK", "MIDDLE_CLICK", scopePath);
        gameplay::ScriptUtil::registerConstantString("RIGHT_CLICK", "RIGHT_CLICK", scopePath);
        gameplay::ScriptUtil::registerConstantString("ENTER", "ENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("LEAVE", "LEAVE", scopePath);
    }

    // Register enumeration Control::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Control");
        gameplay::ScriptUtil::registerConstantString("NORMAL", "NORMAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("FOCUS", "FOCUS", scopePath);
        gameplay::ScriptUtil::registerConstantString("ACTIVE", "ACTIVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("DISABLED", "DISABLED", scopePath);
        gameplay::ScriptUtil::registerConstantString("HOVER", "HOVER", scopePath);
    }

    // Register enumeration Curve::InterpolationType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Curve");
        gameplay::ScriptUtil::registerConstantString("BEZIER", "BEZIER", scopePath);
        gameplay::ScriptUtil::registerConstantString("BSPLINE", "BSPLINE", scopePath);
        gameplay::ScriptUtil::registerConstantString("FLAT", "FLAT", scopePath);
        gameplay::ScriptUtil::registerConstantString("HERMITE", "HERMITE", scopePath);
        gameplay::ScriptUtil::registerConstantString("LINEAR", "LINEAR", scopePath);
        gameplay::ScriptUtil::registerConstantString("SMOOTH", "SMOOTH", scopePath);
        gameplay::ScriptUtil::registerConstantString("STEP", "STEP", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUADRATIC_IN", "QUADRATIC_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUADRATIC_OUT", "QUADRATIC_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUADRATIC_IN_OUT", "QUADRATIC_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUADRATIC_OUT_IN", "QUADRATIC_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("CUBIC_IN", "CUBIC_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("CUBIC_OUT", "CUBIC_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("CUBIC_IN_OUT", "CUBIC_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("CUBIC_OUT_IN", "CUBIC_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUARTIC_IN", "QUARTIC_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUARTIC_OUT", "QUARTIC_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUARTIC_IN_OUT", "QUARTIC_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUARTIC_OUT_IN", "QUARTIC_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUINTIC_IN", "QUINTIC_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUINTIC_OUT", "QUINTIC_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUINTIC_IN_OUT", "QUINTIC_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("QUINTIC_OUT_IN", "QUINTIC_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("SINE_IN", "SINE_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("SINE_OUT", "SINE_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("SINE_IN_OUT", "SINE_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("SINE_OUT_IN", "SINE_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("EXPONENTIAL_IN", "EXPONENTIAL_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("EXPONENTIAL_OUT", "EXPONENTIAL_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("EXPONENTIAL_IN_OUT", "EXPONENTIAL_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("EXPONENTIAL_OUT_IN", "EXPONENTIAL_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("CIRCULAR_IN", "CIRCULAR_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("CIRCULAR_OUT", "CIRCULAR_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("CIRCULAR_IN_OUT", "CIRCULAR_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("CIRCULAR_OUT_IN", "CIRCULAR_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("ELASTIC_IN", "ELASTIC_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("ELASTIC_OUT", "ELASTIC_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ELASTIC_IN_OUT", "ELASTIC_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ELASTIC_OUT_IN", "ELASTIC_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("OVERSHOOT_IN", "OVERSHOOT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("OVERSHOOT_OUT", "OVERSHOOT_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("OVERSHOOT_IN_OUT", "OVERSHOOT_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("OVERSHOOT_OUT_IN", "OVERSHOOT_OUT_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOUNCE_IN", "BOUNCE_IN", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOUNCE_OUT", "BOUNCE_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOUNCE_IN_OUT", "BOUNCE_IN_OUT", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOUNCE_OUT_IN", "BOUNCE_OUT_IN", scopePath);
    }

    // Register enumeration DepthStencilTarget::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("DepthStencilTarget");
        gameplay::ScriptUtil::registerConstantString("DEPTH", "DEPTH", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_STENCIL", "DEPTH_STENCIL", scopePath);
    }

    // Register enumeration Font::Justify.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Font");
        gameplay::ScriptUtil::registerConstantString("ALIGN_LEFT", "ALIGN_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_HCENTER", "ALIGN_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_RIGHT", "ALIGN_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP", "ALIGN_TOP", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER", "ALIGN_VCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM", "ALIGN_BOTTOM", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP_LEFT", "ALIGN_TOP_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER_LEFT", "ALIGN_VCENTER_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM_LEFT", "ALIGN_BOTTOM_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP_HCENTER", "ALIGN_TOP_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER_HCENTER", "ALIGN_VCENTER_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM_HCENTER", "ALIGN_BOTTOM_HCENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_TOP_RIGHT", "ALIGN_TOP_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_VCENTER_RIGHT", "ALIGN_VCENTER_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALIGN_BOTTOM_RIGHT", "ALIGN_BOTTOM_RIGHT", scopePath);
    }

    // Register enumeration Font::Style.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Font");
        gameplay::ScriptUtil::registerConstantString("PLAIN", "PLAIN", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOLD", "BOLD", scopePath);
        gameplay::ScriptUtil::registerConstantString("ITALIC", "ITALIC", scopePath);
        gameplay::ScriptUtil::registerConstantString("BOLD_ITALIC", "BOLD_ITALIC", scopePath);
    }

    // Register enumeration Game::ClearFlags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Game");
        gameplay::ScriptUtil::registerConstantString("CLEAR_COLOR", "CLEAR_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLEAR_DEPTH", "CLEAR_DEPTH", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLEAR_STENCIL", "CLEAR_STENCIL", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLEAR_COLOR_DEPTH", "CLEAR_COLOR_DEPTH", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLEAR_COLOR_STENCIL", "CLEAR_COLOR_STENCIL", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLEAR_DEPTH_STENCIL", "CLEAR_DEPTH_STENCIL", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLEAR_COLOR_DEPTH_STENCIL", "CLEAR_COLOR_DEPTH_STENCIL", scopePath);
    }

    // Register enumeration Game::State.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Game");
        gameplay::ScriptUtil::registerConstantString("UNINITIALIZED", "UNINITIALIZED", scopePath);
        gameplay::ScriptUtil::registerConstantString("RUNNING", "RUNNING", scopePath);
        gameplay::ScriptUtil::registerConstantString("PAUSED", "PAUSED", scopePath);
    }

    // Register enumeration Gamepad::ButtonMapping.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gamepad");
        gameplay::ScriptUtil::registerConstantString("BUTTON_A", "BUTTON_A", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_B", "BUTTON_B", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_C", "BUTTON_C", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_X", "BUTTON_X", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_Y", "BUTTON_Y", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_Z", "BUTTON_Z", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_MENU1", "BUTTON_MENU1", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_MENU2", "BUTTON_MENU2", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_MENU3", "BUTTON_MENU3", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_MENU4", "BUTTON_MENU4", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_L1", "BUTTON_L1", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_L2", "BUTTON_L2", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_L3", "BUTTON_L3", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_R1", "BUTTON_R1", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_R2", "BUTTON_R2", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_R3", "BUTTON_R3", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_UP", "BUTTON_UP", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_DOWN", "BUTTON_DOWN", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_LEFT", "BUTTON_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_RIGHT", "BUTTON_RIGHT", scopePath);
    }

    // Register enumeration Gamepad::GamepadEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gamepad");
        gameplay::ScriptUtil::registerConstantString("CONNECTED_EVENT", "CONNECTED_EVENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("DISCONNECTED_EVENT", "DISCONNECTED_EVENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("BUTTON_EVENT", "BUTTON_EVENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("JOYSTICK_EVENT", "JOYSTICK_EVENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("TRIGGER_EVENT", "TRIGGER_EVENT", scopePath);
    }

    // Register enumeration Gesture::GestureEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Gesture");
        gameplay::ScriptUtil::registerConstantString("GESTURE_TAP", "GESTURE_TAP", scopePath);
        gameplay::ScriptUtil::registerConstantString("GESTURE_SWIPE", "GESTURE_SWIPE", scopePath);
        gameplay::ScriptUtil::registerConstantString("GESTURE_PINCH", "GESTURE_PINCH", scopePath);
        gameplay::ScriptUtil::registerConstantString("GESTURE_ANY_SUPPORTED", "GESTURE_ANY_SUPPORTED", scopePath);
    }

    // Register enumeration Image::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Image");
        gameplay::ScriptUtil::registerConstantString("RGB", "RGB", scopePath);
        gameplay::ScriptUtil::registerConstantString("RGBA", "RGBA", scopePath);
    }

    // Register enumeration Keyboard::Key.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Keyboard");
        gameplay::ScriptUtil::registerConstantString("KEY_NONE", "KEY_NONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PAUSE", "KEY_PAUSE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SCROLL_LOCK", "KEY_SCROLL_LOCK", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PRINT", "KEY_PRINT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SYSREQ", "KEY_SYSREQ", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_BREAK", "KEY_BREAK", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_ESCAPE", "KEY_ESCAPE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_BACKSPACE", "KEY_BACKSPACE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_TAB", "KEY_TAB", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_BACK_TAB", "KEY_BACK_TAB", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_RETURN", "KEY_RETURN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPS_LOCK", "KEY_CAPS_LOCK", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SHIFT", "KEY_SHIFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CTRL", "KEY_CTRL", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_ALT", "KEY_ALT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_MENU", "KEY_MENU", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_HYPER", "KEY_HYPER", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_INSERT", "KEY_INSERT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_HOME", "KEY_HOME", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PG_UP", "KEY_PG_UP", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_DELETE", "KEY_DELETE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_END", "KEY_END", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PG_DOWN", "KEY_PG_DOWN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_LEFT_ARROW", "KEY_LEFT_ARROW", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_RIGHT_ARROW", "KEY_RIGHT_ARROW", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_UP_ARROW", "KEY_UP_ARROW", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_DOWN_ARROW", "KEY_DOWN_ARROW", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_NUM_LOCK", "KEY_NUM_LOCK", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_PLUS", "KEY_KP_PLUS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_MINUS", "KEY_KP_MINUS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_MULTIPLY", "KEY_KP_MULTIPLY", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_DIVIDE", "KEY_KP_DIVIDE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_ENTER", "KEY_KP_ENTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_HOME", "KEY_KP_HOME", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_UP", "KEY_KP_UP", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_PG_UP", "KEY_KP_PG_UP", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_LEFT", "KEY_KP_LEFT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_FIVE", "KEY_KP_FIVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_RIGHT", "KEY_KP_RIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_END", "KEY_KP_END", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_DOWN", "KEY_KP_DOWN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_PG_DOWN", "KEY_KP_PG_DOWN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_INSERT", "KEY_KP_INSERT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_KP_DELETE", "KEY_KP_DELETE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F1", "KEY_F1", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F2", "KEY_F2", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F3", "KEY_F3", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F4", "KEY_F4", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F5", "KEY_F5", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F6", "KEY_F6", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F7", "KEY_F7", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F8", "KEY_F8", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F9", "KEY_F9", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F10", "KEY_F10", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F11", "KEY_F11", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F12", "KEY_F12", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SPACE", "KEY_SPACE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_EXCLAM", "KEY_EXCLAM", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_QUOTE", "KEY_QUOTE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_NUMBER", "KEY_NUMBER", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_DOLLAR", "KEY_DOLLAR", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PERCENT", "KEY_PERCENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CIRCUMFLEX", "KEY_CIRCUMFLEX", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_AMPERSAND", "KEY_AMPERSAND", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_APOSTROPHE", "KEY_APOSTROPHE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_LEFT_PARENTHESIS", "KEY_LEFT_PARENTHESIS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_RIGHT_PARENTHESIS", "KEY_RIGHT_PARENTHESIS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_ASTERISK", "KEY_ASTERISK", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PLUS", "KEY_PLUS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_COMMA", "KEY_COMMA", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_MINUS", "KEY_MINUS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_PERIOD", "KEY_PERIOD", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SLASH", "KEY_SLASH", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_ZERO", "KEY_ZERO", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_ONE", "KEY_ONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_TWO", "KEY_TWO", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_THREE", "KEY_THREE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_FOUR", "KEY_FOUR", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_FIVE", "KEY_FIVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SIX", "KEY_SIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SEVEN", "KEY_SEVEN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_EIGHT", "KEY_EIGHT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_NINE", "KEY_NINE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_COLON", "KEY_COLON", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SEMICOLON", "KEY_SEMICOLON", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_LESS_THAN", "KEY_LESS_THAN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_EQUAL", "KEY_EQUAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_GREATER_THAN", "KEY_GREATER_THAN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_QUESTION", "KEY_QUESTION", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_AT", "KEY_AT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_A", "KEY_CAPITAL_A", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_B", "KEY_CAPITAL_B", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_C", "KEY_CAPITAL_C", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_D", "KEY_CAPITAL_D", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_E", "KEY_CAPITAL_E", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_F", "KEY_CAPITAL_F", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_G", "KEY_CAPITAL_G", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_H", "KEY_CAPITAL_H", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_I", "KEY_CAPITAL_I", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_J", "KEY_CAPITAL_J", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_K", "KEY_CAPITAL_K", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_L", "KEY_CAPITAL_L", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_M", "KEY_CAPITAL_M", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_N", "KEY_CAPITAL_N", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_O", "KEY_CAPITAL_O", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_P", "KEY_CAPITAL_P", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_Q", "KEY_CAPITAL_Q", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_R", "KEY_CAPITAL_R", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_S", "KEY_CAPITAL_S", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_T", "KEY_CAPITAL_T", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_U", "KEY_CAPITAL_U", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_V", "KEY_CAPITAL_V", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_W", "KEY_CAPITAL_W", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_X", "KEY_CAPITAL_X", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_Y", "KEY_CAPITAL_Y", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CAPITAL_Z", "KEY_CAPITAL_Z", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_LEFT_BRACKET", "KEY_LEFT_BRACKET", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_BACK_SLASH", "KEY_BACK_SLASH", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_RIGHT_BRACKET", "KEY_RIGHT_BRACKET", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_UNDERSCORE", "KEY_UNDERSCORE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_GRAVE", "KEY_GRAVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_A", "KEY_A", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_B", "KEY_B", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_C", "KEY_C", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_D", "KEY_D", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_E", "KEY_E", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_F", "KEY_F", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_G", "KEY_G", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_H", "KEY_H", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_I", "KEY_I", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_J", "KEY_J", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_K", "KEY_K", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_L", "KEY_L", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_M", "KEY_M", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_N", "KEY_N", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_O", "KEY_O", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_P", "KEY_P", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_Q", "KEY_Q", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_R", "KEY_R", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_S", "KEY_S", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_T", "KEY_T", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_U", "KEY_U", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_V", "KEY_V", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_W", "KEY_W", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_X", "KEY_X", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_Y", "KEY_Y", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_Z", "KEY_Z", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_LEFT_BRACE", "KEY_LEFT_BRACE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_BAR", "KEY_BAR", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_RIGHT_BRACE", "KEY_RIGHT_BRACE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_TILDE", "KEY_TILDE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_EURO", "KEY_EURO", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_POUND", "KEY_POUND", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_YEN", "KEY_YEN", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_MIDDLE_DOT", "KEY_MIDDLE_DOT", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_SEARCH", "KEY_SEARCH", scopePath);
    }

    // Register enumeration Keyboard::KeyEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Keyboard");
        gameplay::ScriptUtil::registerConstantString("KEY_PRESS", "KEY_PRESS", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_RELEASE", "KEY_RELEASE", scopePath);
        gameplay::ScriptUtil::registerConstantString("KEY_CHAR", "KEY_CHAR", scopePath);
    }

    // Register enumeration Layout::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Layout");
        gameplay::ScriptUtil::registerConstantString("LAYOUT_FLOW", "LAYOUT_FLOW", scopePath);
        gameplay::ScriptUtil::registerConstantString("LAYOUT_VERTICAL", "LAYOUT_VERTICAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("LAYOUT_ABSOLUTE", "LAYOUT_ABSOLUTE", scopePath);
    }

    // Register enumeration Light::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Light");
        gameplay::ScriptUtil::registerConstantString("DIRECTIONAL", "DIRECTIONAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("POINT", "POINT", scopePath);
        gameplay::ScriptUtil::registerConstantString("SPOT", "SPOT", scopePath);
    }

    // Register enumeration Logger::Level.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Logger");
        gameplay::ScriptUtil::registerConstantString("LEVEL_INFO", "LEVEL_INFO", scopePath);
        gameplay::ScriptUtil::registerConstantString("LEVEL_WARN", "LEVEL_WARN", scopePath);
        gameplay::ScriptUtil::registerConstantString("LEVEL_ERROR", "LEVEL_ERROR", scopePath);
    }

    // Register enumeration Mesh::IndexFormat.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mesh");
        gameplay::ScriptUtil::registerConstantString("INDEX8", "INDEX8", scopePath);
        gameplay::ScriptUtil::registerConstantString("INDEX16", "INDEX16", scopePath);
        gameplay::ScriptUtil::registerConstantString("INDEX32", "INDEX32", scopePath);
    }

    // Register enumeration Mesh::PrimitiveType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mesh");
        gameplay::ScriptUtil::registerConstantString("TRIANGLES", "TRIANGLES", scopePath);
        gameplay::ScriptUtil::registerConstantString("TRIANGLE_STRIP", "TRIANGLE_STRIP", scopePath);
        gameplay::ScriptUtil::registerConstantString("LINES", "LINES", scopePath);
        gameplay::ScriptUtil::registerConstantString("LINE_STRIP", "LINE_STRIP", scopePath);
        gameplay::ScriptUtil::registerConstantString("POINTS", "POINTS", scopePath);
    }

    // Register enumeration Mouse::MouseEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Mouse");
        gameplay::ScriptUtil::registerConstantString("MOUSE_PRESS_LEFT_BUTTON", "MOUSE_PRESS_LEFT_BUTTON", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_RELEASE_LEFT_BUTTON", "MOUSE_RELEASE_LEFT_BUTTON", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_PRESS_MIDDLE_BUTTON", "MOUSE_PRESS_MIDDLE_BUTTON", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_RELEASE_MIDDLE_BUTTON", "MOUSE_RELEASE_MIDDLE_BUTTON", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_PRESS_RIGHT_BUTTON", "MOUSE_PRESS_RIGHT_BUTTON", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_RELEASE_RIGHT_BUTTON", "MOUSE_RELEASE_RIGHT_BUTTON", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_MOVE", "MOUSE_MOVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("MOUSE_WHEEL", "MOUSE_WHEEL", scopePath);
    }

    // Register enumeration Node::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Node");
        gameplay::ScriptUtil::registerConstantString("NODE", "NODE", scopePath);
        gameplay::ScriptUtil::registerConstantString("JOINT", "JOINT", scopePath);
    }

    // Register enumeration ParticleEmitter::TextureBlending.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("ParticleEmitter");
        gameplay::ScriptUtil::registerConstantString("BLEND_OPAQUE", "BLEND_OPAQUE", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_TRANSPARENT", "BLEND_TRANSPARENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ADDITIVE", "BLEND_ADDITIVE", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_MULTIPLIED", "BLEND_MULTIPLIED", scopePath);
    }

    // Register enumeration PhysicsCollisionObject::CollisionListener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionObject");
        scopePath.push_back("CollisionListener");
        gameplay::ScriptUtil::registerConstantString("COLLIDING", "COLLIDING", scopePath);
        gameplay::ScriptUtil::registerConstantString("NOT_COLLIDING", "NOT_COLLIDING", scopePath);
    }

    // Register enumeration PhysicsCollisionObject::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionObject");
        gameplay::ScriptUtil::registerConstantString("RIGID_BODY", "RIGID_BODY", scopePath);
        gameplay::ScriptUtil::registerConstantString("CHARACTER", "CHARACTER", scopePath);
        gameplay::ScriptUtil::registerConstantString("GHOST_OBJECT", "GHOST_OBJECT", scopePath);
        gameplay::ScriptUtil::registerConstantString("VEHICLE", "VEHICLE", scopePath);
        gameplay::ScriptUtil::registerConstantString("VEHICLE_WHEEL", "VEHICLE_WHEEL", scopePath);
        gameplay::ScriptUtil::registerConstantString("NONE", "NONE", scopePath);
    }

    // Register enumeration PhysicsCollisionShape::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsCollisionShape");
        gameplay::ScriptUtil::registerConstantString("SHAPE_NONE", "SHAPE_NONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("SHAPE_BOX", "SHAPE_BOX", scopePath);
        gameplay::ScriptUtil::registerConstantString("SHAPE_SPHERE", "SHAPE_SPHERE", scopePath);
        gameplay::ScriptUtil::registerConstantString("SHAPE_CAPSULE", "SHAPE_CAPSULE", scopePath);
        gameplay::ScriptUtil::registerConstantString("SHAPE_MESH", "SHAPE_MESH", scopePath);
        gameplay::ScriptUtil::registerConstantString("SHAPE_HEIGHTFIELD", "SHAPE_HEIGHTFIELD", scopePath);
    }

    // Register enumeration PhysicsController::Listener::EventType.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("PhysicsController");
        scopePath.push_back("Listener");
        gameplay::ScriptUtil::registerConstantString("ACTIVATED", "ACTIVATED", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEACTIVATED", "DEACTIVATED", scopePath);
    }

    // Register enumeration Properties::Type.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Properties");
        gameplay::ScriptUtil::registerConstantString("NONE", "NONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("STRING", "STRING", scopePath);
        gameplay::ScriptUtil::registerConstantString("NUMBER", "NUMBER", scopePath);
        gameplay::ScriptUtil::registerConstantString("VECTOR2", "VECTOR2", scopePath);
        gameplay::ScriptUtil::registerConstantString("VECTOR3", "VECTOR3", scopePath);
        gameplay::ScriptUtil::registerConstantString("VECTOR4", "VECTOR4", scopePath);
        gameplay::ScriptUtil::registerConstantString("MATRIX", "MATRIX", scopePath);
    }

    // Register enumeration RenderState::AutoBinding.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        gameplay::ScriptUtil::registerConstantString("NONE", "NONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("WORLD_MATRIX", "WORLD_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("VIEW_MATRIX", "VIEW_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("PROJECTION_MATRIX", "PROJECTION_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("WORLD_VIEW_MATRIX", "WORLD_VIEW_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("VIEW_PROJECTION_MATRIX", "VIEW_PROJECTION_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("WORLD_VIEW_PROJECTION_MATRIX", "WORLD_VIEW_PROJECTION_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("INVERSE_TRANSPOSE_WORLD_MATRIX", "INVERSE_TRANSPOSE_WORLD_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX", scopePath);
        gameplay::ScriptUtil::registerConstantString("CAMERA_WORLD_POSITION", "CAMERA_WORLD_POSITION", scopePath);
        gameplay::ScriptUtil::registerConstantString("CAMERA_VIEW_POSITION", "CAMERA_VIEW_POSITION", scopePath);
        gameplay::ScriptUtil::registerConstantString("MATRIX_PALETTE", "MATRIX_PALETTE", scopePath);
        gameplay::ScriptUtil::registerConstantString("SCENE_AMBIENT_COLOR", "SCENE_AMBIENT_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("SCENE_LIGHT_COLOR", "SCENE_LIGHT_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("SCENE_LIGHT_DIRECTION", "SCENE_LIGHT_DIRECTION", scopePath);
    }

    // Register enumeration RenderState::Blend.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        gameplay::ScriptUtil::registerConstantString("BLEND_ZERO", "BLEND_ZERO", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ONE", "BLEND_ONE", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_SRC_COLOR", "BLEND_SRC_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ONE_MINUS_SRC_COLOR", "BLEND_ONE_MINUS_SRC_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_DST_COLOR", "BLEND_DST_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ONE_MINUS_DST_COLOR", "BLEND_ONE_MINUS_DST_COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_SRC_ALPHA", "BLEND_SRC_ALPHA", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ONE_MINUS_SRC_ALPHA", "BLEND_ONE_MINUS_SRC_ALPHA", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_DST_ALPHA", "BLEND_DST_ALPHA", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ONE_MINUS_DST_ALPHA", "BLEND_ONE_MINUS_DST_ALPHA", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_CONSTANT_ALPHA", "BLEND_CONSTANT_ALPHA", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_ONE_MINUS_CONSTANT_ALPHA", "BLEND_ONE_MINUS_CONSTANT_ALPHA", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLEND_SRC_ALPHA_SATURATE", "BLEND_SRC_ALPHA_SATURATE", scopePath);
    }

    // Register enumeration RenderState::CullFaceSide.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        gameplay::ScriptUtil::registerConstantString("CULL_FACE_SIDE_BACK", "CULL_FACE_SIDE_BACK", scopePath);
        gameplay::ScriptUtil::registerConstantString("CULL_FACE_SIDE_FRONT", "CULL_FACE_SIDE_FRONT", scopePath);
        gameplay::ScriptUtil::registerConstantString("CULL_FACE_SIDE_FRONT_AND_BACK", "CULL_FACE_SIDE_FRONT_AND_BACK", scopePath);
    }

    // Register enumeration RenderState::DepthFunction.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("RenderState");
        gameplay::ScriptUtil::registerConstantString("DEPTH_NEVER", "DEPTH_NEVER", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_LESS", "DEPTH_LESS", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_EQUAL", "DEPTH_EQUAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_LEQUAL", "DEPTH_LEQUAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_GREATER", "DEPTH_GREATER", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_NOTEQUAL", "DEPTH_NOTEQUAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_GEQUAL", "DEPTH_GEQUAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEPTH_ALWAYS", "DEPTH_ALWAYS", scopePath);
    }

    // Register enumeration Scene::DebugFlags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Scene");
        gameplay::ScriptUtil::registerConstantString("DEBUG_BOXES", "DEBUG_BOXES", scopePath);
        gameplay::ScriptUtil::registerConstantString("DEBUG_SPHERES", "DEBUG_SPHERES", scopePath);
    }

    // Register enumeration Terrain::Flags.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Terrain");
        gameplay::ScriptUtil::registerConstantString("DEBUG_PATCHES", "DEBUG_PATCHES", scopePath);
        gameplay::ScriptUtil::registerConstantString("FRUSTUM_CULLING", "FRUSTUM_CULLING", scopePath);
        gameplay::ScriptUtil::registerConstantString("LEVEL_OF_DETAIL", "LEVEL_OF_DETAIL", scopePath);
    }

    // Register enumeration Texture::Filter.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        gameplay::ScriptUtil::registerConstantString("NEAREST", "NEAREST", scopePath);
        gameplay::ScriptUtil::registerConstantString("LINEAR", "LINEAR", scopePath);
        gameplay::ScriptUtil::registerConstantString("NEAREST_MIPMAP_NEAREST", "NEAREST_MIPMAP_NEAREST", scopePath);
        gameplay::ScriptUtil::registerConstantString("LINEAR_MIPMAP_NEAREST", "LINEAR_MIPMAP_NEAREST", scopePath);
        gameplay::ScriptUtil::registerConstantString("NEAREST_MIPMAP_LINEAR", "NEAREST_MIPMAP_LINEAR", scopePath);
        gameplay::ScriptUtil::registerConstantString("LINEAR_MIPMAP_LINEAR", "LINEAR_MIPMAP_LINEAR", scopePath);
    }

    // Register enumeration Texture::Format.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        gameplay::ScriptUtil::registerConstantString("UNKNOWN", "UNKNOWN", scopePath);
        gameplay::ScriptUtil::registerConstantString("RGB", "RGB", scopePath);
        gameplay::ScriptUtil::registerConstantString("RGBA", "RGBA", scopePath);
        gameplay::ScriptUtil::registerConstantString("ALPHA", "ALPHA", scopePath);
    }

    // Register enumeration Texture::Wrap.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Texture");
        gameplay::ScriptUtil::registerConstantString("REPEAT", "REPEAT", scopePath);
        gameplay::ScriptUtil::registerConstantString("CLAMP", "CLAMP", scopePath);
    }

    // Register enumeration Touch::TouchEvent.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("Touch");
        gameplay::ScriptUtil::registerConstantString("TOUCH_PRESS", "TOUCH_PRESS", scopePath);
        gameplay::ScriptUtil::registerConstantString("TOUCH_RELEASE", "TOUCH_RELEASE", scopePath);
        gameplay::ScriptUtil::registerConstantString("TOUCH_MOVE", "TOUCH_MOVE", scopePath);
    }

    // Register enumeration VertexFormat::Usage.
    {
        std::vector<std::string> scopePath;
        scopePath.push_back("VertexFormat");
        gameplay::ScriptUtil::registerConstantString("POSITION", "POSITION", scopePath);
        gameplay::ScriptUtil::registerConstantString("NORMAL", "NORMAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("COLOR", "COLOR", scopePath);
        gameplay::ScriptUtil::registerConstantString("TANGENT", "TANGENT", scopePath);
        gameplay::ScriptUtil::registerConstantString("BINORMAL", "BINORMAL", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLENDWEIGHTS", "BLENDWEIGHTS", scopePath);
        gameplay::ScriptUtil::registerConstantString("BLENDINDICES", "BLENDINDICES", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD0", "TEXCOORD0", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD1", "TEXCOORD1", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD2", "TEXCOORD2", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD3", "TEXCOORD3", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD4", "TEXCOORD4", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD5", "TEXCOORD5", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD6", "TEXCOORD6", scopePath);
        gameplay::ScriptUtil::registerConstantString("TEXCOORD7", "TEXCOORD7", scopePath);
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
    if (enumname == "Gamepad::ButtonMapping")
        return lua_stringFromEnum_GamepadButtonMapping((Gamepad::ButtonMapping)value);
    if (enumname == "Gamepad::GamepadEvent")
        return lua_stringFromEnum_GamepadGamepadEvent((Gamepad::GamepadEvent)value);
    if (enumname == "Gesture::GestureEvent")
        return lua_stringFromEnum_GestureGestureEvent((Gesture::GestureEvent)value);
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
    if (enumname == "Logger::Level")
        return lua_stringFromEnum_LoggerLevel((Logger::Level)value);
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
    if (enumname == "RenderState::CullFaceSide")
        return lua_stringFromEnum_RenderStateCullFaceSide((RenderState::CullFaceSide)value);
    if (enumname == "RenderState::DepthFunction")
        return lua_stringFromEnum_RenderStateDepthFunction((RenderState::DepthFunction)value);
    if (enumname == "Scene::DebugFlags")
        return lua_stringFromEnum_SceneDebugFlags((Scene::DebugFlags)value);
    if (enumname == "Terrain::Flags")
        return lua_stringFromEnum_TerrainFlags((Terrain::Flags)value);
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
