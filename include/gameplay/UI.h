#pragma once

#include "App.h"

namespace gameplay
{
class GP_API UI
{
    friend class App;
public:
    /**
     * Constructor.
     *
     * @see Windowing::create_ui() instead.
     */
    UI();

    /**
     * Destructor.
     */
	~UI();

private:
	void startup();
	void shutdown();
    void update();
    struct Impl;
    std::unique_ptr<Impl> _impl;
};
}
