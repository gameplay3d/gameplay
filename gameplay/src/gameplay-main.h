#ifndef GAMEPLAYMAIN__H
#define GAMEPLAYMAIN__H

#ifdef __QNX__
#include "gameplay-main-qnx.h"
#elif WIN32
#include "gameplay-main-win32.h"
#endif

#endif