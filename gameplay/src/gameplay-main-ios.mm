#ifdef __APPLE__

#import <UIKit/UIKit.h>
#include "gameplay.h"

using namespace gameplay;

// UIScreen bounds are provided as if device was in portrait mode
// Gameplay defaults to landscape
extern const int WINDOW_WIDTH  = [[UIScreen mainScreen] bounds].size.height;
extern const int WINDOW_HEIGHT = [[UIScreen mainScreen] bounds].size.width;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{    
    Game* game = Game::getInstance();
    assert(game != NULL);
    Platform* platform = Platform::create(game);
    int result = platform->enterMessagePump();
	delete platform;
    return result;
}

#endif
