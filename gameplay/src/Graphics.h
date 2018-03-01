#pragma once

#include "vulkan/vulkan.h"

namespace gameplay
{

class Graphics
{
    friend class Game;

public:

    /**
     * Constructor.
     */
    Graphics();

    /**
     * Destructor.
     */
    ~Graphics();

    void onInitialize();

    void onFinalize();

    void onResize(size_t width, size_t height);

    void onRender(float elapsedTime);

    void onPause();

    void onResume();
	
};

}
