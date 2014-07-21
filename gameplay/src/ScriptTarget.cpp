#include "Base.h"
#include "ScriptController.h"
#include "ScriptTarget.h"

namespace gameplay
{

// TODO: Handle reloading scripts and then case where EventRegistries have events added AFTER a script has been loaded (i.e. need to reload callbacks for the script).
// TODO: Should eventCallbacks store a vector of Event pointers instead of std::string (so we only have to do pointer comparisons instead of string comparisons)? What if Events are destroyed?
// TODO: Should isolated scripts be cached (not currently), or a new script loaded for every ScriptTarget instance?

ScriptTarget::EventRegistry::EventRegistry()
{
}

ScriptTarget::EventRegistry::~EventRegistry()
{
    for (size_t i = 0, count = _events.size(); i < count; ++i)
    {
        SAFE_DELETE(_events[i]);
    }
}

const ScriptTarget::EventRegistry::Event* ScriptTarget::EventRegistry::addEvent(const char* name, const char* args)
{
    GP_ASSERT(name);

    Event* evt = new Event;
    evt->name = name;
    evt->args = args ? args : "";
    evt->registry = this;

    _events.push_back(evt);

    return evt;
}

unsigned int ScriptTarget::EventRegistry::getEventCount() const
{
    return _events.size();
}

const ScriptTarget::EventRegistry::Event* ScriptTarget::EventRegistry::getEvent(unsigned int index) const
{
    GP_ASSERT(index < _events.size());

    return _events[index];
}

ScriptTarget::ScriptTarget() : _scripts(NULL), _events(NULL)
{
}

ScriptTarget::~ScriptTarget()
{
    // Free scripts
    Script* script = _scripts;
    while (script)
    {
        // The Game class is itself a ScriptTarget, so we need to protect against
        // this destructor being called after the Game's destructor, when the
        // ScriptController has already been destroyed.
        Game* game = Game::getInstance();
        if (game)
        {
            ScriptController* sc = game->getScriptController();
            sc->unloadScript(script->id);
        }
        script->id = 0;
        Script* tmp = script;
        script = script->next;
        SAFE_DELETE(script);
    }

    // Free events (we don't own the EventRegistry pointers, so don't free them)
    SAFE_DELETE(_events);
}

void ScriptTarget::registerEvents(EventRegistry* registry)
{
    GP_ASSERT(registry);

    if (!_events)
        _events = new std::vector<EventRegistry*>();

    _events->push_back(registry);
}

int ScriptTarget::addScript(const char* path)
{
    // Load the script into an isolated environment
    ScriptController* sc = Game::getInstance()->getScriptController();
    int id = sc->loadScriptIsolated(path);
    if (id <= 0)
        return id;

    // Attach the script
    Script* script = new Script;
    script->id = id;
    script->path = path;
    if (_scripts)
    {
        Script* last = _scripts;
        while (last->next)
            last = last->next;
        last->next = script;
        script->prev = last;
    }
    else
    {
        _scripts = script;
    }

    // Inspect the loaded script for supported event functions.
    // TODO: We'll need to re-load eventCallbacks when EventRegistries change for this ScriptObject.
    if (_events)
    {
        for (size_t i = 0, registryCount = _events->size(); i < registryCount; ++i)
        {
            std::vector<EventRegistry::Event*>& events = (*_events)[i]->_events;
            for (size_t j = 0, eventCount = events.size(); j < eventCount; ++j)
            {
                EventRegistry::Event* event = events[j];
                if (sc->functionExists(event->name.c_str(), id))
                {
                    script->eventCallbacks.push_back(event);
                }
            }
        }
    }

    return script->id;
}

bool ScriptTarget::removeScript(const char* path)
{
    Script* script = _scripts;
    while (script)
    {
        if (script->path == path)
        {
            removeScript(script);

            return true;
        }
        script = script->next;
    }

    return false;
}

void ScriptTarget::removeScript(Script* script)
{
    GP_ASSERT(script);

    // Link out this script
    Script* next = script->next;
    if (script->prev)
        script->prev->next = script->next;
    if (script->next)
        script->next->prev = script->prev;

    // Unload the script
    Game::getInstance()->getScriptController()->unloadScript(script->id);

    // Free the script object
    SAFE_DELETE(script);
}

void ScriptTarget::clearScripts()
{
    while (_scripts)
    {
        removeScript(_scripts);
    }
}

bool ScriptTarget::hasScriptListener(const EventRegistry::Event* evt) const
{
    Script* script = _scripts;
    while (script)
    {
        // Does this script have a callback implemented for the given event?
        std::vector<const EventRegistry::Event*>& callbacks = script->eventCallbacks;
        for (size_t i = 0, count = callbacks.size(); i < count; ++i)
        {
            if (callbacks[i] == evt)
                return true;
        }
        //std::vector<std::string>::iterator itr = std::find(callbacks.begin(), callbacks.end(), evt->name);
        //if (itr != callbacks.end())
            //return true;
        script = script->next;
    }

    return false;
}

template<> void ScriptTarget::fireScriptEvent<void>(const EventRegistry::Event* evt, ...)
{
    GP_ASSERT(evt);

    va_list list;
    va_start(list, evt);

    // Fire this event for all scripts that support it
    Script* script = _scripts;
    while (script)
    {
        // Is there a callback in this script for this event?
        std::vector<const EventRegistry::Event*>& callbacks = script->eventCallbacks;
        for (size_t i = 0, count = callbacks.size(); i < count; ++i)
        {
            if (callbacks[i] == evt)
            {
                Game::getInstance()->getScriptController()->executeFunction<void>(evt->name.c_str(), evt->args.c_str(), &list, script->id);
            }
        }
        /*
        std::vector<std::string>& callbacks = script->eventCallbacks;
        std::vector<std::string>::iterator itr = std::find(callbacks.begin(), callbacks.end(), evt->name);
        if (itr != callbacks.end())
        {
            Game::getInstance()->getScriptController()->executeFunction<void>(itr->c_str(), evt->args.c_str(), &list, script->id);
        }
        */

        script = script->next;
    }

    va_end(list);
}

template<> bool ScriptTarget::fireScriptEvent<bool>(const EventRegistry::Event* evt, ...)
{
    va_list list;
    va_start(list, evt);

    // Fire this event for all scripts that support it
    Script* script = _scripts;
    while (script)
    {
        // Is there a callback in this script for this event?
        std::vector<const EventRegistry::Event*>& callbacks = script->eventCallbacks;
        for (size_t i = 0, count = callbacks.size(); i < count; ++i)
        {
            if (callbacks[i] == evt)
            {
                // Call the script function
                if (Game::getInstance()->getScriptController()->executeFunction<bool>(evt->name.c_str(), evt->args.c_str(), &list, script->id))
                {
                    va_end(list);
                    return true;
                }
            }
        }
        /*
        std::vector<std::string>& callbacks = script->eventCallbacks;
        std::vector<std::string>::iterator itr = std::find(callbacks.begin(), callbacks.end(), evt->name);
        if (itr != callbacks.end())
        {
            // Call the script function
            if (Game::getInstance()->getScriptController()->executeFunction<bool>(itr->c_str(), evt->args.c_str(), &list, script->id))
            {
                va_end(list);
                return true;
            }
        }
        */

        script = script->next;
    }

    va_end(list);
    return false;
}

}
