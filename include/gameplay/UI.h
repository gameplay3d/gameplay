#pragma once

#include "App.h"

namespace gameplay
{
class GP_API UI
{
    friend class App;
public:
    // todo

private:
	UI();
	~UI();
	void startup();
	void shutdown();
};
}
