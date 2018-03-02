#include "gameplay.h"

using namespace gameplay;

int main(int argc, char** argv)
{
    Platform* platform = Platform::getPlatform();
    return platform->run();
}
