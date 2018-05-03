#pragma once

#include "Serializable.h"
#include "Vector2.h"

namespace gameplay
{

/**
 * Defines user input api.
 */
class Input
{
    friend class Activator;
    
public:

    static const uint32_t kGamepadsMax = 4;
    static const uint32_t kGamepadsAnyIndex = UINT32_MAX;

    /**
     * Defines keys.
     */
    enum class Key
    {
        eNone = 0,
        eEsc,
        eReturn,
        eTab,
        eSpace,
        eBackspace,
        eUp,
        eDown,
        eLeft,
        eRight,
        eInsert,
        eDelete,
        eHome,
        eEnd,
        ePageUp,
        ePageDown,
        ePrint,
        ePlus,
        eMinus,
        eLeftBracket,
        eRightBracket,
        eSemiColon,
        eQuote,
        eComma,
        ePeriod,
        eSlash,
        eBackslash,
        eTilde,
        eF1,
        eF2,
        eF3,
        eF4,
        eF5,
        eF6,
        eF7,
        eF8,
        eF9,
        eF10,
        eF11,
        eF12,
        eNumpad0,
        eNumpad1,
        eNumpad2,
        eNumpad3,
        eNumpad4,
        eNumpad5,
        eNumpad6,
        eNumpad7,
        eNumpad8,
        eNumpad9,
        e0,
        e1,
        e2,
        e3,
        e4,
        e5,
        e6,
        e7,
        e8,
        e9,
        eA,
        eB,
        eC,
        eD,
        eE,
        eF,
        eG,
        eH,
        eI,
        eJ,
        eK,
        eL,
        eM,
        eN,
        eO,
        eP,
        eQ,
        eR,
        eS,
        eT,
        eU,
        eV,
        eW,
        eX,
        eY,
        eZ,
        eGamepadA,
        eGamepadB,
        eGamepadX,
        eGamepadY,
        eGamepadThumbLeft,
        eGamepadThumbRight,
        eGamepadShoulderLeft,
        eGamepadShoulderRight,
        eGamepadUp,
        eGamepadDown,
        eGamepadLeft,
        eGamepadRight,
        eGamepadBack,
        eGamepadStart,
        eGamepadGuide,
        eCount
    };

    /**
     * Defines input key modifiers.
     */
    enum class KeyModifiers : uint8_t
    {
        eNone       = 0,
        eLeftAlt    = 0x01,
        eRightAlt   = 0x02,
        eLeftCtrl   = 0x04,
        eRightCtrl  = 0x08,
        eLeftShift  = 0x10,
        eRightShift = 0x20,
        eLeftMeta   = 0x40,
        eRightMeta  = 0x80,
        eCount
    };

    /**
     * Defines input mouse buttons.
     */
    enum class MouseButton
    {
        eNone = 0,
        eLeft,
        eRight,
        eMiddle,
        eCount
    };

    /**
     * Defines gamepad axes.
     */
    enum class GamepadAxis : uint8_t
    {
        eLeftX = 0,
        eLeftY,
        eLeftZ,
        eRightX,
        eRightY,
        eRightZ,
        eCount
    };

    /**
     * Defines mouse axes.
     */
    enum class MouseAxis
    {
        eX = 0,
        eY,
        eScrollWheel,
        eCount
    };

    /**
     * Defines a configurable input mapping which can bind one or more input sources to a named Axis and/or Actions.
     *
     * Mappings are registered with Input::addMapping. Once registered, the current state of the inputs defined by the mapping
     * can be queried using its name, see Input::getAxis and Input::isAction<up/held/down>.
     *
     * Multiple mappings can use the same name which allows multiple mapping types to share the same name query. For example, horizontal
     * input could be defined by two mappings called 'Horizontal', one of type eMouseAxis and the other eGamepadAxis.
     *
     * When retrieving the state for mapping query, the input system only uses data relevant to the mappings current type. For example, when
     * the type is eMouseAxis only the value of getMouseAxis() is used, any other data defined in the mapping is ignored.
     */
    class Mapping : public Serializable
    {
        friend class Activator;
        friend class Input;

    public:

          /**
         * Defines the input type of an input mapping.
         *
         * Axes types only have a normalized axis value, action types have both a normalized axis value and an action state (up, down and held).
         */
        enum class Type
        {
            eKey = 0,
            eMouseButton,
            eMouseAxis,
            eGamepadAxis,
            eCount
        };

        /**
         * Defines the action.
         *
         * Positive key/mouse button presses will be treated as an axis value of +1, negative as -1.
         */
        enum class Action
        {
            ePositive = 0,
            eNegative,
            eCount
        };

        /**
         * Defines an action profile.
         *
         * Action key/button input sources are registered with an ActionProfile, this allows different
         * input combinations to be bound to a single action.
         */
        enum class ActionProfile
        {
            ePrimary = 0,
            eSecondary,
            eCount
        };

        /**
         * Constructor.
         */
        Mapping();

        /**
         * Gets the name of the mapping.
         *
         * @return The mapping name.
         */
        std::string getName() const;

        /**
         * Sets the name of the mapping.
         *
         * @param name The mapping name.
         */
        void setName(const std::string& name);

        /**
         * Gets the description of the mapping.
         *
         * @return The (non-localized) mapping descrption.
         */
        std::string getDescription() const;

        /**
         * Sets the description of the mapping.
         *
         * @param description The (non-localized) mapping descrption.
         */
        void setDescription(const std::string& description);

        /**
         * Get the mappings type.
         *
         * @return The mapping type.
         */
        Type getType() const;

        /**
         * Sets the mappings type.
         *
         * @param type The mapping type.
         */
        void setType(Type type);

        /**
         * Get whether the input sources axis value will be inverted.
         *
         * @return True if the axis is inverted.
         */
        bool getInverted() const;

        /**
         * Set whether the input sources axis value will be inverted.
         *
         * @param inverted Whether to invert the axis.
         */
        void setInverted(bool inverted);

        /**
         * Get the key for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @return The actions key.
         */
        Key getKeyAction(ActionProfile profile, Action action) const;

        /**
         * Set the key for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         */
        void setKeyAction(ActionProfile profile, Action action, Key key);

        /**
         * Get the key modifiers for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @return The actions key modifier.
         */
        KeyModifiers getKeyActionModifiers(ActionProfile profile, Action action) const;

        /**
         * Set the key modifiers for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         */
        void setKeyActionModifiers(ActionProfile profile, Action action, KeyModifiers modifiers);

        /**
         * Get the mouse button for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @return The actions mouse button.
         */
        MouseButton getMouseButtonAction(ActionProfile profile, Action action) const;

        /**
         * Set the MouseButton for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @param button The mouse button.
         */
        void setMouseButtonAction(ActionProfile profile, Action action, MouseButton button);

        /**
         * Get the mouse axis.
         *
         * @return The mouse axis.
         */
        MouseAxis getMouseAxis() const;

        /**
         * Set the mouse axis.
         *
         * @param axis The mouse axis.
         */
        void setMouseAxis(MouseAxis axis);

        /**
         * Get the gamepad axis.
         *
         * @return The gamepad axis.
         */
        GamepadAxis getGamepadAxis() const;

        /**
         * Get the gamepad axis.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @param axis The gamepad axis.
         */
        void setGamepadAxis(GamepadAxis axis);

        /**
         * Get the gamepad index for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @return The gamepad index.
         */
        uint32_t getKeyActionIndex(ActionProfile profile, Action action) const;

        /**
         * Set the gamepad index for the given action.
         *
         * @param profile The actions profile.
         * @param action The action.
         * @param gamepadIndex The gamepad index.
         */
        void setKeyActionIndex(ActionProfile profile, Action action, uint32_t gamepadIndex);

        /**
         * Gets the gamepad index of the gamepad axis.
         *
         * @return The gamepad index.
         */
        uint32_t getGamepadAxisIndex() const;

        /**
         * Sets the gamepad index of the gamepad axis.
         *
         * @param gamepadIndex The gamepad index.
         */
        void setGamepadAxisIndex(uint32_t gamepadIndex);

    protected:
        
        /**
         * @see Serializable::getClassName
         */
        std::string getClassName();

        /**
         * @see Serializable::onSerialize
         */
        void onSerialize(Serializer* serializer);

        /**
         * @see Serializable::onDeserialize
         */
        void onDeserialize(Serializer* serializer);
        
        /**
         * @see Activator::createObject
         */
        static std::shared_ptr<Serializable> createObject();

        struct ActionData : public Serializable
        {
            ActionData(ActionProfile profile, Action action);
            uint32_t slotIndex;
            uint32_t gamepadIndex;
            KeyModifiers modifiers;
            Action action;
            ActionProfile profile;

            /**
            * @see Serializable::onSerialize
            */
            void onSerialize(Serializer* serializer);

            /**
            * @see Serializable::onDeserialize
            */
            void onDeserialize(Serializer* serializer);
        };

        struct KeyActionData : public ActionData
        {
            KeyActionData(ActionProfile profile, Action action);

            /**
            * @see Serializable::onSerialize
            */
            void onSerialize(Serializer* serializer);

            /**
            * @see Serializable::onDeserialize
            */
            void onDeserialize(Serializer* serializer);

            /**
            * @see Serializable::getClassName
            */
            std::string getClassName();

            /**
            * @see Activator::createObject
            */
            static std::shared_ptr<Serializable> createObject();
        };

        struct MouseButtonActionData : public ActionData
        {
            MouseButtonActionData(ActionProfile profile, Action action);

            /**
            * @see Serializable::onSerialize
            */
            void onSerialize(Serializer* serializer);

            /**
            * @see Serializable::onDeserialize
            */
            void onDeserialize(Serializer* serializer);

            /**
            * @see Serializable::getClassName
            */
            std::string getClassName();

            /**
            * @see Activator::createObject
            */
            static std::shared_ptr<Serializable> createObject();
        };

        typedef std::array<std::shared_ptr<ActionData>, static_cast<uint32_t>(Action::eCount)> Actions;
        typedef std::array<Actions, static_cast<uint32_t>(ActionProfile::eCount)> ActionProfileMap;
        const ActionProfileMap& getKeyActions() const;
        const ActionProfileMap& getMouseButtonActions() const;
        void serialize(Serializer* serializer, const ActionProfileMap& map);
        void deserialize(Serializer* serializer, ActionProfileMap& map);
    private:
        Type _type;
        bool _inverted;
        std::string _name;
        std::string _description;
        MouseAxis _mouseAxis;
        GamepadAxis _gamepadAxis;
        uint32_t _gamepadIndex;
        ActionProfileMap _keyActions;
        ActionProfileMap _mouseButtonActions;
    };

    /**
     * Gets the string name for a key.
     *
     * @param key The key to get the string name for.
     * @return The string name for a key.
     */
    static std::string getKeyName(Key key);

    /**
     * Gets the ascii keycode for a key.
     *
     * @param key The key to get the ascii keycode for.
     * @param modifiers The key modifiers applied.
     * @return The ascii keycode for a key.
     */
    static char getKeyCode(Key key, KeyModifiers modifiers = KeyModifiers::eNone);

    /**
     * Gets the string names for the key modifiers.
     *
     * @param modifiers The key modifiers to get the string names for.
     * @return The space-delimited string names for the key modiifers.
     */
    static std::string getKeyModifierNames(KeyModifiers modifiers);

    /**
     * Gets the string name for a mouse button.
     *
     * @param mouseButton The mouse button to get the string name for.
     * @return The string name for a mouse button.
     */
    static std::string getMouseButtonName(MouseButton mouseButton);

    /**
     * Gets the string name for a mouse axis.
     *
     * @param mouseAxis The mouse axis to get the string name for.
     * @return The string name for a mouse axis.
     */
    static std::string getMouseAxisName(MouseAxis mouseAxis);

    /**
     * Gets the string name for a gamepad axis.
     *
     * @param mouseAxis The gamepad axis to get the string name for.
     * @return The string name for a gamepad axis.
     */
    static std::string getGamepadAxisName(GamepadAxis gamepadAxis);

    /**
     * Get if the given key is held.
     *
     * A key will be in a held state for the duration of it being pressed.
     *
     * @param key The key.
     * @param modifiers The keys modifiers.
     * @param gamepadIndex The gamepad index, only applies to gamepad keys.
     * @return True if held.
     */
    static bool isKeyHeld(Key key, KeyModifiers modifiers = KeyModifiers::eNone, uint32_t gamepadIndex = kGamepadsAnyIndex);

    /**
     * Get if the given key is down.
     *
     * A key will be only be in a down state on the first frame of it being pressed.
     *
     * @param key The key.
     * @param modifiers The keys modifiers.
     * @param gamepadIndex The gamepad index, only applies to gamepad keys.
     * @return True if down.
     */
    static bool isKeyDown(Key key, KeyModifiers modifiers = KeyModifiers::eNone, uint32_t gamepadIndex = kGamepadsAnyIndex);

    /**
     * Get if the given key is up.
     *
     * A key will be only be in an up state on the last frame that it was being held.
     *
     * @param key The key.
     * @param modifiers The keys modifiers.
     * @param gamepadIndex The gamepad index, only applies to gamepad keys.
     * @return True if up.
     */
    static bool isKeyUp(Key key, KeyModifiers modifiers = KeyModifiers::eNone, uint32_t gamepadIndex = kGamepadsAnyIndex);

    /**
     * Get if the given mouse button is held.
     *
     * A mouse button will be in a held state for the duration of it being pressed.
     *
     * @param button The mouse button.
     * @return True if held.
     */
    static bool isMouseButtonHeld(MouseButton button);

    /**
     * Get if the given mouse button is down.
     *
     * A mouse button will be only be in a down state on the first frame of it being pressed.
     *
     * @param button The mouse button.
     * @return True if down.
     */
    static bool isMouseButtonDown(MouseButton button);

    /**
     * Get if the given mouse button is up.
     *
     * A mouse button will be only be in an up state on the last frame that it was being held.
     *
     * @param button The mouse button.
     * @return True if up.
     */
    static bool isMouseButtonUp(MouseButton button);

    /**
     * Get if the mouse position has been locked to its last known location.
     *
     * @return True if locked.
     */
    static bool isMouseLocked();

    /**
     * Set if the mouse position should be locked to its last known location.
     *
     * @param lock True to lock.
     */
    static void setMouseLocked(bool lock);

    /**
     * Gets the normalized mouse scroll value.
     *
     * @return The mouse scroll value.
     */
    static float getMouseScroll();

    /**
     * Gets the mouse position in screen coordinates.
     *
     * @return The mouse position.
     */
    static const Vector2& getMousePosition();

    /**
     * Gets the normalized value of the given mouse axis.
     *
     * @param mouseAxis The mouse axis.
     * @return The mouse axis value.
     */
    static float getMouseAxis(MouseAxis mouseAxis);

    /**
     * Gets the normalized value of the given gamepad axis.
     *
     * @param gamepadAxis The gamepad axis.
     * @param gamepadIndex The gamepad index.
     * @return The gamepad axis value.
     */
    static float getGamepadAxis(GamepadAxis gamepadAxis, uint32_t gamepadIndex = kGamepadsAnyIndex);

    /**
     * Get if the given action mapping is held.
     *
     * A action will be in a held state for the duration of it being pressed.
     *
     * @param mappingName The name of the mapping.
     * @return True if held.
     */
    static bool isActionHeld(const std::string& mappingName);

    /**
     * Get if the given action mapping is down.
     *
     * A action will be only be in a down state on the first frame of it being pressed.
     *
     * @param mappingName The name of the mapping.
     * @return True if down.
     */
    static bool isActionDown(const std::string& mappingName);

    /**
     * Get if the given action mapping is up.
     *
     * A action will be only be in an up state on the last frame that it was being held.
     *
     * @param mappingName The name of the mapping.
     * @return True if up.
     */
    static bool isActionUp(const std::string& mappingName);

    /**
     * Get the normalized axis value of the given mapping.
     *
     * @param mappingName The name of the mapping.
     * @return The axis value.
     */
    static float getAxis(const std::string& mappingName);

    /**
     * Creates a configurable input mapping, this must be added via Input::addMapping before it
     * can be used to query input state.
     *
     * @return An input mapping.
     */
    static std::shared_ptr<Mapping> createMapping();

    /**
     * Add the given input mapping that will allow its name to be used to query input state.
     *
     * @param mapping An input mapping.
     * @return True if added, false if not added because already registered.
     */
    static bool addMapping(const std::shared_ptr<Mapping>& mapping);

    /**
     * Remove all mappings with the given name.
     *
     * @param name The name of the mappings to remove.
     * @return True if mappings with this name were removed.
     */
    static bool removeMappings(const std::string& name);

    /**
     * Remove the given mapping only, other mappings with the same name will not be removed.
     *
     * @param mapping The mapping to move.
     * @return True if removed, false if it was not removed because it had never been registered.
     */
    static bool removeMapping(const std::shared_ptr<Mapping>& mapping);

    ////////////////////////////////////////////////////////////////////////////////////////
    // Used only by input providers (Platform/Editor)
    static Input* getInput();
    void initialize();
    void update();
    void postGamepadConnectionEvent(uint32_t gamepadIndex, bool connected);
    void postGamepadAxisEvent(uint32_t gamepadIndex, GamepadAxis axis, int value);
    void postMouseMotionEvent(int mx, int my);
    void postMouseWheelEvent(int value);
    void postMousePressEvent(int mx, int my, MouseButton button, bool down);
    void postKeyCharEvent(char chr);
    void postKeyPressEvent(Key key, KeyModifiers modifiers, bool down, uint32_t gamepadIndex = kGamepadsAnyIndex);
protected:
    /**
     * @see Activator::enumToString
     */
    static std::string enumToString(const std::string& enumName, int value);

    /**
     * @see Activator::enumParse
     */
    static int enumParse(const std::string& enumName, const std::string& str);
private:
    class Mappings : public Serializable
    {
    public:
        static std::shared_ptr<Serializable> createObject();
        bool add(const std::shared_ptr<Mapping>& action);
        bool remove(const std::shared_ptr<Mapping>& action);
        bool remove(const std::string& name);
        std::string getClassName();
        void onSerialize(Serializer* serializer);
        void onDeserialize(Serializer* serializer);
        std::vector<std::shared_ptr<Mapping>> mappings;
    };

    struct MappingState
    {
        bool up;
        bool down;
        bool held;
        float axis;
    };

    class ActionState
    {
    public:
        ActionState();
        void initialize(int slotCount);
        void update();
        void postEvent(uint32_t slot, bool down, KeyModifiers modifiers = KeyModifiers::eNone);
        bool isActive(uint32_t slot, KeyModifiers modifiers = KeyModifiers::eNone) const;
        bool isUp(uint32_t slot, KeyModifiers modifiers = KeyModifiers::eNone) const;
        bool isDown(uint32_t slot, KeyModifiers modifiers = KeyModifiers::eNone) const;
        bool isHeld(uint32_t slot, KeyModifiers modifiers = KeyModifiers::eNone) const;
    private:
        struct ActionSlot
        {
            enum class Bits
            {
                eUp,
                eDown,
                eHeld,
                eCount
            };
            ActionSlot();
            KeyModifiers modifiers;
            std::bitset<static_cast<uint32_t>(Bits::eCount)> bits;
        };
        bool test(uint32_t slot, KeyModifiers modifiers, ActionSlot::Bits bit) const;
        bool dirty;
        std::vector<ActionSlot> actionSlots;
    };

    struct GamepadState
    {
        GamepadState();
        bool connected;
        int index;
        ActionState buttons;
        std::array<float, static_cast<int>(GamepadAxis::eCount)> axes;
    };

    Input();
    ~Input();
    bool isKey(Key key, KeyModifiers modifiers, uint32_t gamepadIndex, bool up, bool down, bool held) const;
    bool isKey(Key key, KeyModifiers modifiers, const ActionState& state, bool up, bool down, bool held) const;
    float getMouseAxisValue(MouseAxis mouseAxis) const;
    float getGamepadAxisValue(GamepadAxis gamepadAxis, uint32_t gamepadIndex) const;
    void getActionState(const Mapping& mapping, MappingState& state) const;
    void getActionState(Mapping::Action action, const Mapping& mapping, bool& up, bool& down, bool& held) const;
    MappingState getMappingState(const std::string& name) const;

    std::shared_ptr<Mappings> _mappings;
    ActionState _keys;
    ActionState _mouseButtons;
    Vector2 _mousePosition;
    Vector2 _previousMousePosition;
    bool _mouseLocked;
    float _mouseScroll;
    std::array<GamepadState, kGamepadsMax> _gamepads;
};

}

GP_ENABLE_BITWISE_OPERATORS(gameplay::Input::KeyModifiers);
