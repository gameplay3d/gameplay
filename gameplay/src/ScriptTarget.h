#ifndef SCRIPTTARGET_H_
#define SCRIPTTARGET_H_

#include "Base.h"

namespace gameplay
{

/**
 * Signals the start of script event declarations for a ScriptTarget implementation.
 *
 * This macro should be followed by one or more calls to GP_SCRIPT_EVENT.
 *
 * It is recommended that these macros be used at the top of the source file that
 * contains the implemtation of a ScriptTarget child class.
 *
 * @script{ignore}
 */
#define GP_SCRIPT_EVENTS() \
    static gameplay::ScriptTarget::EventRegistry __eventRegistry

/**
 * Defines a script event of the given name and adds it to the event registry for
 * the current scope.
 *
 * A call to GP_SCRIPT_EVENT_START must exist before any calls to GP_SCRIPT_EVENT
 * are made. This macro will define a constant in the current code scope that is
 * named SCRIPT_EVENT_eventName, which should be used when calling 
 * ScriptTarget::fireScriptEvent from a ScriptTarget child class.  For this reason,
 * these macros should exist outside the definition of the class, preferably at
 * the top of the source file.
 *
 * @param eventName Name of the event (no quotes).
 * @param eventArgs Parmeters for this script event.
 *
 * @see ScriptController::executeFunction
 *
 * @script{ignore}
 */
#define GP_SCRIPT_EVENT(eventName, eventArgs) \
    static const gameplay::ScriptTarget::EventRegistry::Event* SCRIPT_EVENT_ ## eventName = __eventRegistry.addEvent(#eventName, eventArgs)

/**
 * Registers the defined script events for a ScriptTarget.
 *
 * This macro should be called in the constructor of a ScriptTarget
 * child class implementation. It requires that GP_SCRIPT_EVENT
 * macros be defined (normally at the top of the compilation unit).
 *
 * @script{ignore}
 */
#define GP_REGISTER_SCRIPT_EVENTS() \
    ScriptTarget::registerEvents(&__eventRegistry)

/**
 * Defines an interface for supporting script callbacks.
 */
class ScriptTarget
{
public:

    /**
     * Script event registry that defines the supported script events
     * for a ScriptTarget.
     *
     * This class should generally only be used via the GP_REGISTER_SCRIPT_EVENTS macro.
     *
     * @script{ignore}
     */
    class EventRegistry
    {
    public:

        /**
         * Defines a single script event.
         *
         * @script{ignore}
         */
        struct Event
        {
            /**
             * The event name.
             */
            std::string name;

            /**
             * The event arguments.
             *
             * @see ScriptController::executeFunction
             */
            std::string args;

            /**
             * The EventRegistry this event belongs to.
             */
            EventRegistry* registry;
        };

        /**
         * Creates an empty event registry.
         */
        EventRegistry();

        /**
         * Adds a registered event to the registry.
         *
         * @param name The name of the script event.
         * @param args The argument string for the event.
         *
         * @return The added event.
         *
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
         *
         * @return The event for the given index.
         */
        const Event* getEvent(unsigned int index) const;

    private:

        std::vector<Event> _events;
    };

    /**
     * Attaches a script to this object.
     *
     * @param path Path to the script.
     * @return The ID of the successfully loaded script, or zero if unsuccessful.
     */
    int addScript(const char* path);

    /**
     * Removes a previously attached script from this object.
     *
     * @param path The same path that was used to load the script.
     * @return True if a script is successfully removed, false otherwise.
     */
    bool removeScript(const char* path);

    /**
     * Determines if there is a script installed that is listening for the given script
     * event (i.e. has a function callback defined for the given event).
     *
     * @param evt The script event to check.
     *
     * @return True if there is a listener for the specified event, false otherwise.
     */
    bool hasScriptListener(const EventRegistry::Event* evt) const;

protected:

    /**
     * Constructor.
     */
    ScriptTarget();

    /**
     * Destructor.
     */
    virtual ~ScriptTarget();

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

    /**
     * Fires the specified script event, passing the specified arguments.
     *
     * The only supported return types are void and boolean. When a boolean
     * return type is used and there are multiple scripts registered for the
     * given script event, event delegation will stop at the first script
     * that returns a value of true.
     * 
     * @param evt The script event to fire, which was returned from EventRegistry::addEvent.
     * @param ... Optional list of arguments to pass to the script event (should match the
     *      script event argument definition).
     */
    template<typename T> T fireScriptEvent(const EventRegistry::Event* evt, ...);

    /** Stores an attached script. */
    struct Script
    {
        // The ID of the script
        int id;

        // The path the script was loaded from
        std::string path;

        // Event callback functions available to be called for this script
        std::vector<std::string> eventCallbacks;

        // Linked list info
        Script* next;
        Script* prev;

        Script() : id(0), next(NULL), prev(NULL) { }
    };

    /** Holds the registered events for this script target. */
    std::vector<EventRegistry*>* _events;
    /** Holds the list of scripts referenced by this ScriptTarget. */
    Script* _scripts;
};

template<typename T> T ScriptTarget::fireScriptEvent(const EventRegistry::Event* evt, ...)
{
    GP_ERROR("Unsupported return type for template function ScriptTarget::fireScriptEvent.");
}

/** Template specialization. */
template<> void ScriptTarget::fireScriptEvent<void>(const EventRegistry::Event* evt, ...);
/** Template specialization. */
template<> bool ScriptTarget::fireScriptEvent<bool>(const EventRegistry::Event* evt, ...);

}

#endif
