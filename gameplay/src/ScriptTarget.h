#ifndef SCRIPTTARGET_H_
#define SCRIPTTARGET_H_

#include "Base.h"

namespace gameplay
{

/**
 * Defines an interface for supporting script callbacks.
 */
class ScriptTarget
{
public:

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

protected:

    /**
     * Hidden constructor.
     */
    ScriptTarget();

    /**
     * Destructor.
     */
    virtual ~ScriptTarget();

    /**
     * Registers the given event with the given script parameter string ({@link ScriptController::executeFunction})
     * as a supported event for this script target.
     * 
     * @param eventName The name of the event.
     * @param argsString The argument string for the event.
     */
    void registerScriptEvent(const std::string& eventName, const char* argsString = NULL);

    /**
     * Fires the event with the given event name and the given arguments.
     * 
     * @param eventName The name of the event.
     */
    template<typename T> T fireScriptEvent(const char* eventName, ...);

    /** Stores an attached script. */
    struct Script
    {
        int id;
        std::string path;
        Script* next;
        Script* prev;
        Script() : id(0), next(NULL), prev(NULL) { }
    };

    /** Used to store a script callbacks for given event. */
    struct Callback
    {
        /** Constructor. */
        Callback(const std::string& string);

        /** The script that this callback belongs to. */
        Script* scriptId;

        /** Holds the script callback function. */
        std::string function;
    };

    /** Holds the registered events for this script target. */
    std::map<std::string, std::string> _events;
    /** Holds the callbacks for this script target's events. */
    std::map<std::string, std::vector<Callback>*> _callbacks;
    /** Holds the list of scripts referenced by this ScriptTarget. */
    Script* _scripts;
};

template<typename T> T ScriptTarget::fireScriptEvent(const char* eventName, ...)
{
    GP_ERROR("Unsupported return type!");
}

/** Template specialization. */
template<> void ScriptTarget::fireScriptEvent<void>(const char* eventName, ...);
/** Template specialization. */
template<> bool ScriptTarget::fireScriptEvent<bool>(const char* eventName, ...);

}

#endif
