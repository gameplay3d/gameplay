#include "gameplay.h"

using namespace gameplay;


extern int __argc;
extern char** __argv;

int main(int argc, const char** argv)
{
    __argc = argc;
    __argv = argv;
    
    Platform* platform = Platform::getPlatform();
    return platform->run();
}
