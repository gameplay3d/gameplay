#import <CoreMotion/CoreMotion.h>
#import "BGPViewController.h"
#import "BGPAppDelegate.h"
#import "PlatformiOS.h"
#include "Base.h"
#include "Game.h"

using namespace gameplay;

@interface BGPAppDelegate ()
{
    UIWindow* window;
    BGPViewController* viewController;
    CMMotionManager* motionManager;
}

@end

@implementation BGPAppDelegate

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    [UIApplication sharedApplication].statusBarHidden = YES;
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

    motionManager = [[CMMotionManager alloc] init];

    if ([motionManager isAccelerometerAvailable])
    {
        motionManager.accelerometerUpdateInterval = 1 / 40.0;    // 40Hz
        [motionManager startAccelerometerUpdates];
    }

    if ([motionManager isGyroAvailable])
    {
        motionManager.gyroUpdateInterval = 1 / 40.0;    // 40Hz
        [motionManager startGyroUpdates];
    }

    [[BGPPlatformManager sharedInstance] setMotionManager:motionManager];

    window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    viewController = [[BGPViewController alloc] init];
    [[BGPPlatformManager sharedInstance] setActiveViewController:viewController];
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    return YES;
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)applicationWillResignActive:(UIApplication*)application
{
    [viewController stopUpdating];
}

- (void)applicationDidEnterBackground:(UIApplication*)application
{
    [viewController stopUpdating];
}

- (void)applicationWillEnterForeground:(UIApplication*)application
{
    [viewController startUpdating];
}

- (void)applicationDidBecomeActive:(UIApplication*)application
{
    [viewController startUpdating];
}

- (void)applicationWillTerminate:(UIApplication*)application
{
    [viewController stopUpdating];

    Game *game = Game::getInstance();

    if (game)
    {
        game->exit();
    }
}

- (void)dealloc
{
    [window setRootViewController:nil];
}

@end
