#pragma once

#include "Audio.h"

namespace gameplay
{

/**
 * Defines an abstract audio system.
 */
class AudioOpenAL : public Audio
{
public:

    /**
     * Constructor.
     */
    AudioOpenAL();

    /**
     * Destructor.
     */
    ~AudioOpenAL();

    /**
     * @see Audio::initialize
     */
    void initialize(unsigned long window, unsigned long connection = 0);
};

}
