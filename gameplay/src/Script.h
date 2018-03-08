#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

    class Message;

/**
 * Defines an script component.
 */
class Script : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;
public:

	enum Type
	{
		TYPE_CPP,
		TYPE_LUA
	};

    /**
     * Constructor.
     */
    Script();

    /**
     * Destructor
     */
    ~Script();

    static std::shared_ptr<Script> loadCppScript(std::string classUrl);

	static std::shared_ptr<Script> loadLuaScript(std::string scriptUrl);

    virtual void onInitialize();

    virtual void onFinalize();

    virtual void onUpdate(float elapsedTime);

    virtual void onMessage(std::shared_ptr<SceneObject> sender, std::shared_ptr<Message> message, void* data);

private:

	Type _type;
	std::string _url;
};

}
