#pragma once

#include "IBehaviour.h"

#include <string>

namespace gameplay
{

/**
 * Supports loading library.
 */
class Module
{
public:
    /**
     * Constructor.
     */
    Module(const std::string& libName, void* library);

    /**
     * Destructor.
     */
    ~Module();

    // TODO check if this should be void* and later reinteroret cast or template
    // to avoid returning only IBehaviour*
    IBehaviour* createBehaviour(const std::string& className);

private:

    std::string _libName;
    void* _content;
};

}

