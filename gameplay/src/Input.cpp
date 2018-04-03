#include "Base.h"
#include "Input.h"
#include "SerializerJson.h"

namespace gameplay
{

static std::string __keyModifierNames[] =
{
    "None",
    "LeftAlt",
    "RightAlt",
    "LeftCtrl",
    "RightCtrl",
    "LeftShift",
    "RightShift",
    "LeftMeta",
    "RightMeta"
};

static std::string __mouseButtonNames[] =
{
    "None",
    "LeftMouseButton",
    "RightMouseButton",
    "MiddleMouseButton"
};

static std::string __mouseAxisNames[] =
{
    "X",
    "Y",
    "ScrollWheel"
};

static std::string __gamepadAxisNames[] =
{
    "LeftX",
    "LeftY",
    "LeftZ",
    "RightX",
    "RightY",
    "RightZ"
};

static std::string __keyNames[] =
{
    "None",
    "Esc",
    "Return",
    "Tab",
    "Space",
    "Backspace",
    "Up",
    "Down",
    "Left",
    "Right",
    "Insert",
    "Delete",
    "Home",
    "End",
    "PageUp",
    "PageDown",
    "Print",
    "Plus",
    "Minus",
    "LeftBracket",
    "RightBracket",
    "Semicolon",
    "Quote",
    "Comma",
    "Period",
    "Slash",
    "Backslash",
    "Tilde",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "NumPad0",
    "NumPad1",
    "NumPad2",
    "NumPad3",
    "NumPad4",
    "NumPad5",
    "NumPad6",
    "NumPad7",
    "NumPad8",
    "NumPad9",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "GamepadA",
    "GamepadB",
    "GamepadX",
    "GamepadY",
    "GamepadThumbL",
    "GamepadThumbR",
    "GamepadShoulderL",
    "GamepadShoulderR",
    "GamepadUp",
    "GamepadDown",
    "GamepadLeft",
    "GamepadRight",
    "GamepadBack",
    "GamepadStart",
    "GamepadGuide",
};

std::string Input::getKeyName(Key key)
{
    GP_ASSERT(key < Key::eCount);
    return __keyNames[static_cast<uint32_t>(key)];
}

std::string Input::getMouseButtonName(MouseButton mouseButton)
{
    GP_ASSERT(mouseButton < MouseButton::eCount);
    return __mouseButtonNames[static_cast<uint32_t>(mouseButton)];
}

std::string Input::getKeyModifierNames(KeyModifiers modifiers)
{
    std::string names;
    uint8_t keyModifiers = static_cast<uint8_t>(modifiers);
    for (uint8_t modifierMask = 1,  modifierIndex = 1; modifierMask < static_cast<uint32_t>(KeyModifiers::eCount); modifierMask <<= 1, ++modifierIndex)
    {
        if((modifierMask & keyModifiers) == modifierMask)
        {
            if(!names.empty())
            {
                names += " ";
            }
            names += __keyModifierNames[modifierIndex];
        }
    }
    return names;
}

std::string Input::getMouseAxisName(MouseAxis mouseAxis)
{
    return __mouseAxisNames[static_cast<uint32_t>(mouseAxis)];
}

std::string Input::getGamepadAxisName(GamepadAxis gamepadAxis)
{
    return __gamepadAxisNames[static_cast<uint32_t>(gamepadAxis)];
}

bool isKeyboard(Input::Key key)
{
    return key <= Input::Key::eZ;
}

bool isGamepad(Input::Key key)
{
    return key < Input::Key::eCount && !isKeyboard(key);
}

bool useAnyGamepad(uint32_t gamepadIndex)
{
    return gamepadIndex == Input::kGamepadsAnyIndex;
}

bool isValidIndex(int index, size_t size)
{
    return index >= 0 && index < size;
}

char Input::getKeyCode(Input::Key key, Input::KeyModifiers modifiers)
{
    const bool isAscii = (Key::e0 <= key && key <= Key::eZ) || (Key::eEsc  <= key && key <= Key::eMinus);
    if (!isAscii)
    {
        return '\0';
    }
    const bool isNumber = (Key::e0 <= key && key <= Key::e9);
    if (isNumber)
    {
        return '0' + char(static_cast<uint32_t>(key) - static_cast<uint32_t>(Key::e0));
    }
    const bool isChar = (Key::eA <= key && key <= Key::eZ);
    if (isChar)
    {
        bool shift = bool(modifiers & Input::KeyModifiers::eLeftShift | KeyModifiers::eRightShift);
        return (shift ? 'A' : 'a') + char(static_cast<uint32_t>(key) - static_cast<uint32_t>(Key::eA));
    }

    switch (key)
    {
    case Key::eEsc:
        return 0x1b;
    case Key::eReturn:
        return '\n';
    case Key::eTab:
        return '\t';
    case Key::eSpace:
        return ' ';
    case Key::eBackspace:
        return 0x08;
    case Key::ePlus:
        return '+';
    case Key::eMinus:
        return '-';
    default: 
        break;
    }
    return '\0';
}

bool Input::isKeyHeld(Key key, KeyModifiers modifiers, uint32_t gamepadIndex)
{
    return getInput()->isKey(key, modifiers, gamepadIndex, false, false, true);
}

bool Input::isKeyDown(Key key, KeyModifiers modifiers, uint32_t gamepadIndex)
{
    return getInput()->isKey(key, modifiers, gamepadIndex, false, true, false);
}

bool Input::isKeyUp(Key key, KeyModifiers modifiers, uint32_t gamepadIndex)
{
    return getInput()->isKey(key, modifiers, gamepadIndex, true, false, false);
}

bool Input::isActionHeld(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).held;
}

bool Input::isActionDown(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).down;
}

bool Input::isActionUp(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).up;
}

float Input::getAxis(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).axis;
}

bool Input::isMouseButtonHeld(MouseButton button)
{
    return getInput()->_mouseButtons.isHeld(static_cast<uint32_t>(button));
}

bool Input::isMouseButtonDown(MouseButton button)
{
    return getInput()->_mouseButtons.isDown(static_cast<uint32_t>(button));
}

bool Input::isMouseButtonUp(MouseButton button)
{
    return getInput()->_mouseButtons.isUp(static_cast<uint32_t>(button));
}

const Vector2& Input::getMousePosition()
{
    return getInput()->_mousePosition;
}

float Input::getMouseAxis(MouseAxis mouseAxis)
{
    return getInput()->getMouseAxisValue(mouseAxis);
}

float Input::getGamepadAxis(GamepadAxis gamepadAxis, uint32_t gamepadIndex)
{
    return getInput()->getGamepadAxisValue(gamepadAxis, gamepadIndex);
}

float Input::getMouseScroll()
{
    return getInput()->_mouseScroll;
}

bool Input::isMouseLocked()
{
    return getInput()->_mouseLocked;
}

void Input::setMouseLocked(bool lock)
{
    getInput()->_mouseLocked = lock;
}

std::shared_ptr<Input::Mapping> Input::createMapping()
{
    return std::make_shared<Mapping>();
}

bool Input::addMapping(const std::shared_ptr<Mapping>& mapping)
{
    return getInput()->_mappings->add(mapping);
}

bool Input::removeMapping(const std::shared_ptr<Mapping>& mapping)
{
    return getInput()->_mappings->remove(mapping);
}

bool Input::removeMappings(const std::string& name)
{
    return getInput()->_mappings->remove(name);
}

Input::Input() :
    _mouseLocked(false),
    _mouseScroll(0)
{
}

Input::~Input()
{
}

Input* Input::getInput()
{
    static Input input;
    return &input;
}

void Input::initialize()
{
    if (Serializer* reader = Serializer::createReader(GP_ENGINE_INPUT))
    {
        std::shared_ptr<Serializable> mappings = reader->readObject(nullptr);
        if (mappings)
        {
            _mappings = std::static_pointer_cast<Mappings>(mappings);
        }
        else
        {
            _mappings = std::make_shared<Mappings>();
        }
        reader->close();
        GP_SAFE_DELETE(reader);
    }
    else
    {
        _mappings = std::make_shared<Mappings>();
        Serializer* writer = SerializerJson::createWriter(GP_ENGINE_INPUT);
        writer->writeObject(nullptr, _mappings);
        writer->close();
        GP_SAFE_DELETE(writer);
    }
    _keys.initialize(static_cast<uint32_t>(Key::eCount));
    _mouseButtons.initialize(static_cast<uint32_t>(MouseButton::eCount));
    for (int i = 0; i < _gamepads.size(); ++i)
    {
        GamepadState& gamepad = _gamepads[i];
        gamepad.index = i;
        gamepad.buttons.initialize(static_cast<uint32_t>(Key::eCount));
    }
}

void Input::update()
{
    _keys.update();
    _mouseButtons.update();
    for (GamepadState& gamepad : _gamepads)
    {
        gamepad.buttons.update();
    }
    _mouseScroll = 0;
    _previousMousePosition = _mousePosition;
}

float Input::getGamepadAxisValue(GamepadAxis axis, uint32_t gamepadIndex) const
{
    const uint32_t gamepadAxis = static_cast<uint32_t>(axis);
    if (gamepadAxis >= static_cast<uint32_t>(GamepadAxis::eCount))
    {
        return 0.0f;
    }
    if (useAnyGamepad(gamepadIndex))
    {
        for(const GamepadState& gamepad : _gamepads)
        {
            const float value = gamepad.axes[gamepadAxis];
            const bool isActive = value != 0.0f;
            if(isActive)
            {
                return value;
            }
        }
    }
    else if (isValidIndex(gamepadIndex, _gamepads.size()))
    {
        return _gamepads[gamepadIndex].axes[gamepadAxis];
    }
    return 0.0f;
}

float Input::getMouseAxisValue(MouseAxis mouseAxis) const
{
    const Vector2 mouseDelta = _mousePosition - _previousMousePosition;
    switch (mouseAxis)
    {
    case MouseAxis::eX:
        return mouseDelta.x > 0 ? 1.0f : mouseDelta.x < 0 ? - 1.0f : 0.0f;
    case MouseAxis::eY:
        return mouseDelta.y < 0 ? 1.0f : mouseDelta.y > 0 ? - 1.0f : 0.0f;
    case MouseAxis::eScrollWheel:
        return _mouseScroll;
    }
    return 0.0f;
}

bool Input::isKey(Key key, KeyModifiers modifiers, const ActionState& state, bool up, bool down, bool held) const
{
    const uint32_t slot = static_cast<uint32_t>(key);
    if (up)
    {
        return state.isUp(slot, modifiers);
    }
    else if (down)
    {
        return state.isDown(slot, modifiers);
    }
    else if (held)
    {
        return state.isHeld(slot, modifiers);
    }
    return false;
}

bool Input::isKey(Key key, KeyModifiers modifiers, uint32_t gamepadIndex, bool up, bool down, bool held) const
{
    if (isKeyboard(key))
    {
        return isKey(key, modifiers, _keys, up, down, held);
    }
    else
    {
        if (gamepadIndex == kGamepadsAnyIndex)
        {
            for (const GamepadState& gamePad : _gamepads)
            {
                if(isKey(key, modifiers, gamePad.buttons, up, down, held))
                {
                    return true;
                }
            }
        }
        else if (gamepadIndex < kGamepadsMax)
        {
            return isKey(key, modifiers, _gamepads[gamepadIndex].buttons, up, down, held);
        }
    }
    return false;
}

void Input::getActionState(Mapping::Action action, const Mapping& mapping, bool& up, bool& down, bool& held) const
{
    up = false;
    down = false;
    held = false;
    const uint32_t actionIndex = static_cast<uint32_t>(action);
    for (uint32_t profileIndex = 0; profileIndex < static_cast<uint32_t>(Mapping::ActionProfile::eCount); ++profileIndex)
    {
        const bool mouse = mapping.getType() == Mapping::Type::eMouseButton;
        const Mapping::ActionProfileMap& map = mouse ? mapping.getMouseButtonActions() : mapping.getKeyActions();
        const Mapping::ActionData& data = map[profileIndex][actionIndex];
        if (data.slotIndex == 0 && data.modifiers == KeyModifiers::eNone)
        {
            continue;
        }
        const ActionState* deviceState = nullptr;
        const Key key = static_cast<Key>(data.slotIndex);
        if (mouse)
        {
            deviceState = &_mouseButtons;
        }
        else if(isKeyboard(key))
        {
            deviceState = &_keys;
        }
        else if (isGamepad(key))
        {
            if (useAnyGamepad(data.gamepadIndex))
            {
                for(int i = 0; i < _gamepads.size(); ++i)
                {
                    const ActionState& gamepadState = _gamepads[i].buttons;
                    if(gamepadState.isActive(data.slotIndex))
                    {
                        deviceState = &gamepadState;
                        break;
                    }
                }
            }
            else if (isValidIndex(data.gamepadIndex, _gamepads.size()))
            {
                 deviceState = &_gamepads[data.gamepadIndex].buttons;
            }
        }
        if (!deviceState)
        {
            continue;
        }
        if (deviceState->isActive(data.slotIndex, data.modifiers))
        {
            up = deviceState->isUp(data.slotIndex, data.modifiers);
            down = deviceState->isDown(data.slotIndex, data.modifiers);
            held = deviceState->isHeld(data.slotIndex, data.modifiers);
            break;
        }
    }
}

void Input::getActionState(const Mapping& mapping, MappingState& state) const
{
    bool positiveUp = false;
    bool positiveDown = false;
    bool positiveHeld = false;
    getActionState(Mapping::Action::ePositive, mapping, positiveUp, positiveDown, positiveHeld);
    bool negativeUp = false;
    bool negativeDown = false;
    bool negativeHeld = false;
    getActionState(Mapping::Action::eNegative, mapping, negativeUp, negativeDown, negativeHeld);
    const bool neutral = positiveHeld && negativeHeld;
    state.held = positiveHeld || negativeHeld;
    state.up = !state.held && (positiveUp || negativeUp);
    state.down = !neutral && (positiveDown || negativeDown);
    if (!neutral && state.held)
    {
        state.axis = positiveHeld ? 1.0f : -1.0f;
    }
}

Input::MappingState Input::getMappingState(const std::string& name) const
{
    MappingState state;
    state.axis = 0.0f;
    state.up = false;
    state.down = false;
    state.held = false;
    for (const std::shared_ptr<Mapping>& mapping : _mappings->mappings)
    {
        if (mapping->getName() == name)
        {
            switch (mapping->getType())
            {
            case Mapping::Type::eGamepadAxis:
                state.axis = getGamepadAxisValue(mapping->getGamepadAxis(), mapping->getGamepadAxisIndex());
                break;
            case Mapping::Type::eMouseAxis:
                state.axis = getMouseAxisValue(mapping->getMouseAxis());
                break;
            case Mapping::Type::eKey:
            case Mapping::Type::eMouseButton:
                getActionState(*mapping, state);
                break;
            }
            if (mapping->getInverted())
            {
                state.axis *= -1.0f;
            }
            const bool isInputActive = state.axis != 0.0f || state.down || state.down || state.held;
            if (isInputActive)
            {
                break;
            }
        }
    }
    return state;
}

void Input::postGamepadConnectionEvent(uint32_t gamepadIndex, bool connected)
{
    _gamepads[gamepadIndex].connected = connected;
}

void Input::postGamepadAxisEvent(uint32_t gamepadIndex, GamepadAxis axis, int value)
{
    value *= axis == GamepadAxis::eLeftY || axis == GamepadAxis::eRightY ? -1 : 1;
    _gamepads[gamepadIndex].axes[static_cast<uint32_t>(axis)] = (1.0f / static_cast<float>(std::numeric_limits<int16_t>::max())) * static_cast<float>(value);
}

void Input::postMouseMotionEvent(int mx, int my)
{
    if (!_mouseLocked)
    {
        _mousePosition.x = mx;
        _mousePosition.y = my;
    }
}

void Input::postMouseWheelEvent(int value)
{
    if (!_mouseLocked)
    {
        _mouseScroll = value;
    }
}

void Input::postMousePressEvent(int mx, int my, MouseButton button, bool down)
{
    _mouseButtons.postEvent(static_cast<uint32_t>(button), down);
}

void Input::postKeyCharEvent(char)
{
}

void Input::postKeyPressEvent(Key key, KeyModifiers modifiers, bool down, uint32_t gamepadIndex)
{
    const uint32_t slot = static_cast<uint32_t>(key);
    if(isKeyboard(key))
    {
        _keys.postEvent(slot, down, modifiers);
    }
    else if(gamepadIndex < kGamepadsMax)
    {
        _gamepads[gamepadIndex].buttons.postEvent(slot, down);
    }
}

Input::GamepadState::GamepadState() :
    connected(false)
{
}

Input::ActionState::ActionState() :
    dirty(false)
{
}

void Input::ActionState::initialize(int buttonCount)
{
    actionSlots.resize(buttonCount);
}

void Input::ActionState::update()
{
    if(dirty)
    {
        for(ActionSlot& slot : actionSlots)
        {
            if (slot.bits.any())
            {
                slot.bits.reset(static_cast<uint32_t>(ActionSlot::Bits::eDown));
                slot.bits.reset(static_cast<uint32_t>(ActionSlot::Bits::eUp));
                slot.modifiers = slot.bits.test(static_cast<uint32_t>(ActionSlot::Bits::eHeld)) ? slot.modifiers : KeyModifiers::eNone;
            }
        }
        dirty = false;
    }
}

bool isModifierOnlyButton(uint32_t button)
{
    return button == 0;
}

bool isModifierMatch(uint32_t slot, Input::KeyModifiers modifiers , Input::KeyModifiers stateModifiers)
{
    const bool areModifiersActive = (modifiers & stateModifiers) == modifiers;
    const bool areOnlyModifiersActive = modifiers == stateModifiers;
    return isModifierOnlyButton(slot) ? areModifiersActive : areOnlyModifiersActive;
}

void Input::ActionState::postEvent(uint32_t button, bool down, KeyModifiers modifiers)
{
    ActionSlot& slot = actionSlots[button];
    const bool wasHeld = slot.bits.test(static_cast<uint32_t>(ActionSlot::Bits::eHeld));
    const bool isReleasing = wasHeld && !down;
    const bool hasModifiersRemaining = modifiers != KeyModifiers::eNone;
    const bool isModifierUpdate = isModifierOnlyButton(button) && isReleasing && hasModifiersRemaining;
    if(isModifierUpdate)
    {
        slot.modifiers = modifiers;
    }
    else
    {
        dirty = true;
        slot.bits.set(static_cast<uint32_t>(ActionSlot::Bits::eDown), !wasHeld && down);
        slot.bits.set(static_cast<uint32_t>(ActionSlot::Bits::eHeld), down);
        slot.bits.set(static_cast<uint32_t>(ActionSlot::Bits::eUp), wasHeld && !down);
        slot.modifiers = down ? modifiers : slot.modifiers;
    }
}

bool Input::ActionState::test(uint32_t slot, KeyModifiers modifiers, ActionSlot::Bits bit) const
{
    const ActionSlot& actionSlotstate = actionSlots[slot];
    return actionSlotstate.bits.test(static_cast<uint32_t>(bit)) && isModifierMatch(slot, modifiers, actionSlotstate.modifiers);
}

bool Input::ActionState::isActive(uint32_t slot, KeyModifiers modifiers) const
{
    const ActionSlot& actionSlotstate = actionSlots[slot];
    return actionSlotstate.bits.any() && isModifierMatch(slot, modifiers, actionSlotstate.modifiers);
}

bool Input::ActionState::isUp(uint32_t slot, KeyModifiers modifiers) const
{
    return test(slot, modifiers, ActionSlot::Bits::eUp);
}

bool Input::ActionState::isDown(uint32_t slot, KeyModifiers modifiers) const
{
    return test(slot, modifiers, ActionSlot::Bits::eDown);
}

bool Input::ActionState::isHeld(uint32_t slot, KeyModifiers modifiers) const
{
    return test(slot, modifiers, ActionSlot::Bits::eHeld);
}

Input::ActionState::ActionSlot::ActionSlot()
    : modifiers(KeyModifiers::eNone)
{
}

std::string Input::Mappings::getClassName()
{
    return "gameplay::Input::Mappings";
}

void Input::Mappings::onSerialize(Serializer* serializer)
{
    serializer->writeInt("size", mappings.size(), 0);
    for (const std::shared_ptr<Mapping>& mapping : mappings)
    {
        mapping->onSerialize(serializer);
    }
}

void Input::Mappings::onDeserialize(Serializer* serializer)
{
    int size = serializer->readInt("size", 0);
    for (int i = 0; i < size; ++i)
    {
        std::shared_ptr<Mapping> mapping = createMapping();
        mapping->onDeserialize(serializer);
    }
}

bool Input::Mappings::add(const std::shared_ptr<Mapping>& mapping)
{
    auto itr = std::find(mappings.begin(), mappings.end(), mapping);
    if (itr == mappings.end())
    {
        mappings.push_back(mapping);
        return true;
    }
    return false;
}

bool Input::Mappings::remove(const std::string& name)
{
    const size_t count = mappings.size();
    auto itr = mappings.begin();
    while (itr != mappings.end())
    {
        if ((*itr)->getName() == name)
        {
            itr = mappings.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
    return count != mappings.size();
}

bool Input::Mappings::remove(const std::shared_ptr<Mapping>& mapping)
{
    auto itr = std::find(mappings.begin(), mappings.end(), mapping);
    if (itr != mappings.end())
    {
        mappings.erase(itr);
        return true;
    }
    return false;
}

Input::Mapping::Mapping() :
    _inverted(false),
    _gamepadIndex(kGamepadsAnyIndex)
{
}

std::string Input::Mapping::getClassName()
{
    return "gameplay::Input::Mapping";
}

void Input::Mapping::serialize(Serializer* serializer, ActionProfileMap& actionProfiles)
{
    for (Actions& actions : actionProfiles)
    {
        for (ActionData& action : actions)
        {
            serializer->writeInt("slotIndex", action.slotIndex, 0);
            serializer->writeInt("gamepadIndex", action.gamepadIndex, 0);
            serializer->writeEnum("modifiers", "gameplay::Input::KeyModifiers", static_cast<int>(action.modifiers), 0);
        }
    }
}
void Input::Mapping::deserialize(Serializer* serializer, ActionProfileMap& actionProfiles)
{
    for (Actions& actions : actionProfiles)
    {
        for (ActionData& action : actions)
        {
            action.slotIndex = serializer->readInt("slotIndex", 0);
            action.gamepadIndex = serializer->readInt("gamepadIndex", 0);
            action.modifiers = static_cast<KeyModifiers>(serializer->readEnum("modifiers", "gameplay::Input::KeyModifiers", 0));
        }
    }
}

void Input::Mapping::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("type", "gameplay::Input::Mapping::Type", static_cast<int>(_type), 0);
    serializer->writeBool("inverted", _inverted, false);
    serializer->writeString("name", _name.c_str(), "");
    serializer->writeString("description", _description.c_str(), "");
    serializer->writeEnum("mouseAxis", "gameplay::Input::MouseAxis", static_cast<int>(_mouseAxis), 0);
    serializer->writeEnum("gamepadAxis", "gameplay::Input::GamepadAxis", static_cast<int>(_gamepadAxis), 0);
    serializer->writeInt("gamepadIndex", _gamepadIndex, 0);
    serialize(serializer, _keyActions);
    serialize(serializer,_mouseButtonActions);
}

void Input::Mapping::onDeserialize(Serializer* serializer)
{
    _type = static_cast<Type>(serializer->readEnum("type", "gameplay::Input::Mapping::Type", 0));
    _inverted = serializer->readBool("inverted", false);
    serializer->readString("name", _name, "");
    serializer->readString("description", _description, "");
    _mouseAxis = static_cast<MouseAxis>(serializer->readEnum("mouseAxis", "gameplay::Input::MouseAxis", 0));
    _gamepadAxis = static_cast<GamepadAxis>(serializer->readEnum("gamepadAxis", "gameplay::Input::GamepadAxis", 0));
    _gamepadIndex = serializer->readInt("gamepadIndex", 0);
    deserialize(serializer, _keyActions);
    deserialize(serializer,_mouseButtonActions);
}

std::string Input::Mapping::getName() const
{
    return _name;
}

void Input::Mapping::setName(const std::string& name)
{
    _name = name;
}

std::string Input::Mapping::getDescription() const
{
    return _description;
}


void Input::Mapping::setDescription(const std::string& description)
{
    _description = description;
}

Input::Mapping::Type Input::Mapping::getType() const
{
    return _type;
}

void Input::Mapping::setType(Type type)
{
    _type = type;
}

bool Input::Mapping::getInverted() const
{
    return _inverted;
}

void Input::Mapping::setInverted(bool inverted)
{
    _inverted = inverted;
}

Input::Key Input::Mapping::getKeyAction(ActionProfile profile, Action action) const
{
    return static_cast<Key>(_keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].slotIndex);
}

void Input::Mapping::setKeyAction(ActionProfile profile, Action action, Key key)
{
    _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].slotIndex = static_cast<uint32_t>(key);
}

Input::KeyModifiers Input::Mapping::getKeyActionModifiers(ActionProfile profile, Action action) const
{
    return _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].modifiers;
}

void Input::Mapping::setKeyActionModifiers(ActionProfile profile, Action action, KeyModifiers modifiers)
{
    _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].modifiers = modifiers;
}

Input::MouseButton Input::Mapping::getMouseButtonAction(ActionProfile profile, Action action) const
{
    return static_cast<MouseButton>(_mouseButtonActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].slotIndex);
}

void Input::Mapping::setMouseButtonAction(ActionProfile profile, Action action, MouseButton button)
{
    _mouseButtonActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].slotIndex = static_cast<uint32_t>(button);
}

Input::MouseAxis Input::Mapping::getMouseAxis() const
{
    return _mouseAxis;
}

void Input::Mapping::setMouseAxis(MouseAxis axis)
{
    _mouseAxis = axis;
}

Input::GamepadAxis Input::Mapping::getGamepadAxis() const
{
    return _gamepadAxis;
}

void Input::Mapping::setGamepadAxis(GamepadAxis axis)
{
    _gamepadAxis = axis;
}

uint32_t Input::Mapping::getKeyActionIndex(ActionProfile profile, Action action) const
{
    return _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].gamepadIndex;
}

void Input::Mapping::setKeyActionIndex(ActionProfile profile, Action action, uint32_t gamepadIndex)
{
    _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)].gamepadIndex = gamepadIndex;
}

uint32_t Input::Mapping::getGamepadAxisIndex() const
{
    return _gamepadIndex;
}

void Input::Mapping::setGamepadAxisIndex(uint32_t gamepadIndex)
{
    _gamepadIndex = gamepadIndex;
}

const Input::Mapping::ActionProfileMap& Input::Mapping::getKeyActions() const
{
    return _keyActions;
}

const Input::Mapping::ActionProfileMap& Input::Mapping::getMouseButtonActions() const
{
    return _mouseButtonActions;
}

Input::Mapping::ActionData::ActionData() :
    slotIndex(0),
    gamepadIndex(0),
    modifiers(KeyModifiers::eNone)
{
}

}
