#pragma once

#include "App.h"

namespace gameplay
{
class GP_API Renderer
{
    friend class App;
public:

private:
	Renderer();
	~Renderer();
	void startup();
	void shutdown();
    void update();
    void next_frame();
    void render_frame();
    void present_frame();
    struct Impl;
    Impl* _impl = nullptr;
};
}
