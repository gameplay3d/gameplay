#pragma once

#include "App.h"

namespace gameplay
{
class GP_API UI
{
    friend class App;
public:
    // todo: manage styles, fonts, etc.

private:
	UI();
	~UI();
	void startup();
	void shutdown();
    void update();
    struct Impl;
    Impl* _impl = nullptr;
};
}
