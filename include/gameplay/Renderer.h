#pragma once

#include "App.h"

namespace gameplay
{
class GP_API Renderer
{
    friend class App;
public:
    /**
     * Constructor.
     * 
     * @see App::get_renderer() instead.
     */
    Renderer();

    /**
     * Destructor.
     */
	~Renderer();

private:
	void startup();
	void shutdown();
    void update();
    void next_frame();
    void render_frame();
    void present_frame();
    struct Impl;
    std::unique_ptr<Impl> _impl;
};
}
