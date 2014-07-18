#include "Base.h"
#include "ScriptController.h"
#include "ScriptTarget.h"

namespace gameplay
{

ScriptTarget::EventRegistry::EventRegistry()
{
}

const ScriptTarget::EventRegistry::Event* ScriptTarget::EventRegistry::addEvent(const char* name, const char* args)
{
    GP_ASSERT(name);

    Event evt;
    evt.name = name;
    evt.args = args ? args : "";
    evt.registry = this;

    _events.push_back(evt);

    return &_events.back();
}

unsigned int ScriptTarget::EventRegistry::getEventCount() const
{
    return _events.size();
}

const ScriptTarget::EventRegistry::Event* ScriptTarget::EventRegistry::getEvent(unsigned int index) const
{
    GP_ASSERT(index < _events.size());

    return &_events[index];
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
        Game::getInstance()->getScriptController()->unloadScript(script->id);
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

    // TODO: Inspect the loaded script for supported event functions

    return script->id;
}

bool ScriptTarget::removeScript(const char* path)
{
    Script* script = _scripts;
    while (script)
    {
        if (script->path == path)
        {
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

            return true;
        }
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
        std::vector<std::string>& callbacks = script->eventCallbacks;
        std::vector<std::string>::iterator itr = std::find(callbacks.begin(), callbacks.end(), evt->name);
        if (itr != callbacks.end())
        {
            Game::getInstance()->getScriptController()->executeFunction<void>(itr->c_str(), evt->args.c_str(), &list, script->id);
        }

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

        script = script->next;
    }

    va_end(list);
    return false;
}

}
