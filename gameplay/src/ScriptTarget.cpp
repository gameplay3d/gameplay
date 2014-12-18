#include "Base.h"
#include "ScriptController.h"
#include "ScriptTarget.h"

namespace gameplay
{

extern void splitURL(const std::string& url, std::string* file, std::string* id);

ScriptTarget::ScriptTarget()
{
}

ScriptTarget::~ScriptTarget()
{
    std::map<std::string, std::vector<Callback>* >::iterator iter = _callbacks.begin();
    for (; iter != _callbacks.end(); iter++)
    {
        SAFE_DELETE(iter->second);
    }
}

template<> void ScriptTarget::fireScriptEvent<void>(const char* eventName, ...)
{
    va_list list;
    va_start(list, eventName);

    std::map<std::string, std::vector<Callback>* >::iterator iter = _callbacks.find(eventName);
    if (iter != _callbacks.end() && iter->second != NULL)
    {
        ScriptController* sc = Game::getInstance()->getScriptController();

        if (_events[eventName].size() > 0)
        {
            for (unsigned int i = 0; i < iter->second->size(); i++)
            {
                sc->executeFunction<void>((*iter->second)[i].function.c_str(), _events[eventName].c_str(), &list);
            }
        }
        else
        {
            for (unsigned int i = 0; i < iter->second->size(); i++)
            {
                sc->executeFunction<void>((*iter->second)[i].function.c_str(), _events[eventName].c_str());
            }
        }
    }

    va_end(list);
}

template<> bool ScriptTarget::fireScriptEvent<bool>(const char* eventName, ...)
{
    va_list list;
    va_start(list, eventName);

    std::map<std::string, std::vector<Callback>* >::iterator iter = _callbacks.find(eventName);
    if (iter != _callbacks.end() && iter->second)
    {
        ScriptController* sc = Game::getInstance()->getScriptController();

        if (_events[eventName].size() > 0)
        {
            for (unsigned int i = 0; i < iter->second->size(); i++)
            {
                if (sc->executeFunction<bool>((*iter->second)[i].function.c_str(), _events[eventName].c_str(), &list))
                {
                    va_end(list);
                    return true;
                }
            }
        }
        else
        {
            for (unsigned int i = 0; i < iter->second->size(); i++)
            {
                if (sc->executeFunction<bool>((*iter->second)[i].function.c_str(), _events[eventName].c_str()))
                {
                    va_end(list);
                    return true;
                }
            }
        }
    }

    va_end(list);
    return false;
}

void ScriptTarget::addScriptCallback(const std::string& eventName, const std::string& function)
{
    std::map<std::string, std::vector<Callback>* >::iterator iter = _callbacks.find(eventName);
    if (iter != _callbacks.end())
    {
        if (!iter->second)
            iter->second = new std::vector<Callback>();

        // Add the function to the list of callbacks.
        std::string functionName = Game::getInstance()->getScriptController()->loadUrl(function.c_str());
        iter->second->push_back(Callback(functionName));
    }
    else
    {
        GP_ERROR("Attempting to add a script callback for unsupported event '%s'.", eventName.c_str());
    }
}

void ScriptTarget::removeScriptCallback(const std::string& eventName, const std::string& function)
{
    std::map<std::string, std::vector<Callback>* >::iterator iter = _callbacks.find(eventName);
    if (iter != _callbacks.end())
    {
        if (!iter->second)
            return;

        std::string file;
        std::string id;
        splitURL(function, &file, &id);

        // Make sure the function isn't empty.
        if (id.size() <= 0)
            return;

        // Remove the function from the list of callbacks.
        for (unsigned int i = 0; i < iter->second->size(); i++)
        {
            if ((*iter->second)[i].function == id)
            {
                iter->second->erase(iter->second->begin() + i);
                return;
            }
        }
    }
    else
    {
        GP_ERROR("Attempting to remove a script callback for unsupported event '%s'.", eventName.c_str());
    }
}

void ScriptTarget::addScriptEvent(const std::string& eventName, const char* argsString)
{
    _events[eventName] = (argsString ? argsString : "");
    _callbacks[eventName] = NULL;
}

ScriptTarget::Callback::Callback(const std::string& function) : function(function)
{
}

}
