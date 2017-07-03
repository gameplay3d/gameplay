#pragma once

#include "Base.h"

/**
 * Represents a script binding for an enumeration.
 */
struct EnumBinding
{
    vector<string> values;
    vector<string> scopePath;
    string include;
};
