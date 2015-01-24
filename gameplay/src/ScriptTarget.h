#ifndef SCRIPTTARGET_H_
#define SCRIPTTARGET_H_

#include "Script.h"

namespace gameplay
{

/**
 * Macro to indidate the start of script event definitions for a class.
 *
 * This macro should be used at the top of a class declaration. The class
 * should extend ScriptTarget and the lines immediately following this
 * macro should be one or more GP_SCRIPT_EVENT macros, followed by
 * exactly one GP_SCRIPT_EVENTS_END macro.
 *
 * @script{ignore}
 */
#define GP_SCRIPT_EVENTS_START() \
public: \
    class ScriptEvents { \
    public: \
        static ScriptEvents* getInstance() \
        { \
            static ScriptEvents instance; \
            return &instance; \
        } \
        static ScriptTarget::EventRegistry* getRegistry() \
        { \
            static ScriptTarget::EventRegistry registry; \
            return &registry; \
        }

/**
 * Macro to define a single supported script event for a class.
 *
 * This macro should follow exactly one prior GP_SCRIPT_EVENTS_START macro
 * and zero or more other GP_SCRIPT_EVENT macros.
 *
 * @param eventName The name of the script event.
 * @param eventArgs A string of arguments to be passed to the script event, using
 *      the format specified in ScriptController::executeFunction.
 *
 * @script{ignore}
 */
#define GP_SCRIPT_EVENT(eventName, eventArgs) \
        struct SCRIPT_EVENT_ ## eventName \
        { \
            SCRIPT_EVENT_ ## eventName() \
            { \
                _event = ScriptEvents::getRegistry()->addEvent(#eventName, eventArgs); \
            } \
            const ScriptTarget::Event* getEvent() \
            { \
                return _event; \
            } \
        private: \
            const ScriptTarget::Event* _event; \
        }; \
        SCRIPT_EVENT_ ## eventName eventName;

/**
 * Macro to indiate the end of a series of script event defintions.
 *
 * @script{ignore}
 */
#define GP_SCRIPT_EVENTS_END() \
    private: \
        ScriptEvents() { } \
    };

/**
 * Macro used to retrieve a script event object the given class name.
 *
 * @param eventClass The C++ class that contains the specified script event.
 * @param eventName The name of the registered script event to retrieve.
 *
 * @script{ignore}
 */
#define GP_GET_SCRIPT_EVENT(eventClass, eventName) \
    eventClass::ScriptEvents::getInstance()->eventName.getEvent()


/**
 * Registers the defined script events for a ScriptTarget.
 *
 * This macro should be called at the beginning of all constructors of a
 * ScriptTarget child class that contains one or more script event 
 * declarations (via the GP_SCRIPT_EVENT macro).
 *
 * @script{ignore}
 */
#define GP_REGISTER_SCRIPT_EVENTS() \
    ScriptTarget::registerEvents(ScriptEvents::getInstance()->getRegistry())

/**
 * Defines an interface for supporting script callbacks.
 *
 * Classes that extend ScriptTarget can expose script events using the GP_SCRIPT_EVENT macros.
 * Custom events should be registered using these macros at the top of the class definition.
 * Events can be fired by calling the ScriptTarget::fireScriptEvent method, passing the 
 * registered ScriptTarget::Event object and any required parameters.
 *
 * In addition to script events that are explicitly defined by a custom ScriptTarget class,
 * all ScriptTarget scripts implicitly support an "attached" event. This event is called
 * immediately after such a script is attached to a ScriptTarget and it takes a single
 * parameter: the ScriptTarget object.
 */
class ScriptTarget
{
    friend class Game;

public:

    /**
     * Defines a single script event.
     */
    class Event
    {
        friend class ScriptTarget;

    public:

        /**
         * Gets the name of this event.
         *
         * @return The event name.
         */
        const char* getName() const;

        /**
         * Gets the argument string for this event.
         *
         * @return The argument string.
         */
        const char* getArgs() const;

    private:

        /**
         * The event name.
         */
        std::string name;

        /**
         * The event arguments.
         * @see ScriptController::executeFunction
         */
        std::string args;

    };

    /**
     * Script event registry that defines the supported script events
     * for a ScriptTarget.
     *
     * This class should generally only be used via the GP_REGISTER_SCRIPT_EVENTS macro.
     */
    class EventRegistry
    {
        friend class ScriptTarget;

    public:

        /**
         * Constructor.
         */
        EventRegistry();

        /**
         * Destructor.
         */
        ~EventRegistry();

        /**
         * Adds a registered event to the registry.
         *
         * @param name The name of the script event.
         * @param args The argument string for the event.
         *
         * @return The added event.
         * @see ScriptController::executeFunction
         */
        const Event* addEvent(const char* name, const char* args = NULL);

        /**
         * Returns the number of registered script events.
         *
         * @return The number of script events registered.
         */
        unsigned int getEventCount() const;

        /**
         * Returns the event with the given index.
         *
         * @param index The index of the event to retrieve.
         * @return The event for the given index.
         */
        const Event* getEvent(unsigned int index) const;

        /**
         * Returns the event that matches the given name.
         *
         * @param name The name of the event to search for.
         * @return The matching event, or NULL if no such event exists.
         */
        const Event* getEvent(const char* name) const;

    private:

        std::vector<Event*> _events;
    };

    /**
     * Gets the type name identifier for the class that extends ScriptTarget.
     *
     * @return A string describing the type name of the ScriptTarget child
     *      class, as it is defined in the lua bindings (i.e. "Node").
     */
    virtual const char* getTypeName() const = 0;

    /**
     * Attaches a script to this object.
     *
     * Scripts attached to a ScriptTarget are loaded using the PROTECTED scope,
     * which loads scripts into their own protected script environment, allowing
     * variables with the same name to be used without colliding with other scripts.
     *
     * @param path Path to the script.
     * @return A pointer to the successfully loaded script, or NULL if unsuccessful.
     */
    Script* addScript(const char* path);

    /**
     * Removes a previously attached script from this object.
     *
     * @param path The same path that was used to load the script being removed.
     * @return True if a script is successfully removed, false otherwise.
     */
    bool removeScript(const char* path);

    /**
     * Adds the given global script function as a callback for the given event.
     *
     * Individual script callback events registered via this method are expected
     * to be global script functions. Registering individual callbacks in this
     * manner is generally slower than registering a single script to handle script
     * events for an object.
     * 
     * @param event The event to add the callback for.
     * @param function The name of the script function to call when the event is fired; can either be
     *  just the name of a function (if the function's script file has already been loaded), or can be
     *  a URL of the form scriptFile.lua#functionName.
     */
    void addScriptCallback(const Event* event, const char* function);

    /**
     * Removes the given script function as a callback for the given event.
     * 
     * @param event The event to remove the callback for.
     * @param function The name of the script function.
     */
    void removeScriptCallback(const Event* event, const char* function);

    /**
     *  Removes all scripts and callbacks from this object.
     */
    void clearScripts();

    /**
     * Determines if there is a script installed that is listening for the given script
     * event (i.e. has a function callback defined for the given event).
     *
     * @param eventName The script event to check.
     * @return True if there is a listener for the specified event, false otherwise.
     */
    bool hasScriptListener(const char* eventName) const;

    /**
     * Determines if there is a script installed that is listening for the given script
     * event (i.e. has a function callback defined for the given event).
     *
     * @param event The script event to check.
     * @return True if there is a listener for the specified event, false otherwise.
     */
    bool hasScriptListener(const Event* event) const;

    /**
     * Gets the event object for the given event name, if it exists.
     *
     * @param eventName Name of the event.
     * @return The event object for the given name, or NULL if no such event exists.
     */
    const Event* getScriptEvent(const char* eventName) const;

    /**
     * Fires the specified script event, passing the specified arguments.
     *
     * The only supported return types are void and boolean. When a boolean
     * return type is used and there are multiple scripts registered for the
     * given script event, event delegation will stop at the first script
     * that returns a value of true.
     * 
     * @param event The script event to fire, which was returned from EventRegistry::addEvent.
     * @param ... Optional list of arguments to pass to the script event (should match the
     *      script event argument definition).
     *
     * @script{ignore}
     */
    template<typename T> T fireScriptEvent(const Event* event, ...);

protected:

    /**
     * Stores an EventRegistry entry for a ScriptTarget.
     */
    struct RegistryEntry
    {
        /** The event registry. */
        EventRegistry* registry;
        /** The next entry in the registry. */
        RegistryEntry* next;
        /** The previous entry in the registry. */
        RegistryEntry* prev;

        /**
         * Constructor.
         * @param registry The event registry.
         */
        RegistryEntry(EventRegistry* registry) : registry(registry), next(NULL), prev(NULL) { }
    };

    /**
     * Stores a Script that is registered for a ScriptTarget.
     */
    struct ScriptEntry
    {
        /** The script. */
        Script* script;
        /** The next script entry. */
        ScriptEntry* next;
        /** The previous script entry. */
        ScriptEntry* prev;

        /**
         * Constructor.
         *
         * @param script The script to create an entry for.
         */
        ScriptEntry(Script* script) : script(script), next(NULL), prev(NULL) { }
    };

    /**
     * Stores a single registered script callback function.
     */
    struct CallbackFunction
    {
        /** The script the callback belongs to (or NULL if the callback is a global function) */
        Script* script;
        /** The function within the script to call. */
        std::string function;

        /**
         * The callback function to registry script function to.
         * @param script The script.
         * @param function The script function.
         */
        CallbackFunction(Script* script, const char* function) : script(script), function(function) { }
    };

    /**
     * Constructor.
     */
    ScriptTarget();

    /**
     * Destructor.
     */
    virtual ~ScriptTarget();

    /**
     * Removes the specified script.
     *
     * @param entry The script entry to be removed.
     */
    void removeScript(ScriptEntry* entry);

    /**
     * Registers a set of supported script events and event arguments for this ScriptTarget. 
     *
     * The passed in EventRegistry object should contain a list of all script events to
     * be supported by this ScriptTarget, along with their parameter definitions. This
     * registry object will be held onto for the lifetime of the ScriptTarget and it is
     * recommended that it be shared among all ScriptTargets of the same class/type.
     *
     * This should normally be set using the GP_REGISTER_SCRIPT_EVENTS macro in the
     * constructor of the child class.
     *
     * @param registry The EventRegistry containing the list of supported script events.
     */
    void registerEvents(EventRegistry* registry);

    /** Holds the event registries for this script target. */
    RegistryEntry* _scriptRegistries;
    /** Holds the list of scripts referenced by this ScriptTarget. */
    ScriptEntry* _scripts;
    /** Holds the list of callback functions registered for this ScriptTarget. */
    std::map<const Event*, std::vector<CallbackFunction> >* _scriptCallbacks;
};

/**
 * The fire script event template specialization.
 *
 * @param event The event fired.
 * @param ... Optional list of arguments to pass to the script event.
 */
template<typename T> T ScriptTarget::fireScriptEvent(const Event* event, ...)
{
    GP_ERROR("Unsupported return type for template function ScriptTarget::fireScriptEvent.");
}

/** 
 * Template specialization. 
 *
 * @param evt The event fired.
 * @param ... Optional list of arguments to pass to the script event (should match the
 *      script event argument definition).
 */
template<> void ScriptTarget::fireScriptEvent<void>(const Event* evt, ...);

/** 
 * Template specialization.
 *
 * @param evt The event fired.
 * @param ... Optional list of arguments to pass to the script event (should match the
 *      script event argument definition).
 */
template<> bool ScriptTarget::fireScriptEvent<bool>(const Event* evt, ...);

}

#endif
