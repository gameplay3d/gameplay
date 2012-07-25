#ifndef ENUMBINDING_H
#define ENUMBINDING_H

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

#endif
