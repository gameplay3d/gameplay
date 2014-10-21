#import <CoreMotion/CoreMotion.h>

@class BGPViewController;

namespace gameplay
{

extern double getMachTimeInMilliseconds();

}

@interface BGPPlatformManager : NSObject

@property (nonatomic, strong) BGPViewController* activeViewController;
@property (nonatomic, strong) CMMotionManager* motionManager;

+ (BGPPlatformManager*)sharedInstance;

@end
