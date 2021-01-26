#pragma once

#include "App.h"

namespace gameplay
{
class GP_API Renderer
{
    friend class App;
public:
    // todo

private:
	Renderer();
	~Renderer();
	void startup();
	void shutdown();
};
}
