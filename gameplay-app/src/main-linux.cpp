#include "gameplay.h"

using namespace gameplay;


int main(int argc, const char** argv)
{
    Platform* platform = Platform::getPlatform();
    return platform->run();
}
