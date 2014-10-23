#import <CoreMotion/CoreMotion.h>

@class BGPViewController;

@interface BGPPlatform : NSObject

+ (void)setActiveViewController:(BGPViewController*)viewController;
+ (BGPViewController*)activeViewController;

+ (void)setMotionManager:(CMMotionManager*)motionManager;
+ (CMMotionManager*)motionManager;

+ (void)shutdownGame;

@end
