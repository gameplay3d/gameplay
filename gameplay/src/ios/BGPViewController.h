#import <GameKit/GameKit.h>
#import <UIKit/UIKit.h>

@class BGPView;

@interface BGPViewController : UIViewController

- (void)startUpdating;

- (void)stopUpdating;

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController*)gameCenterViewController;

- (BGPView*)gamePlayView;

@end
