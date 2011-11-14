/*
 * PlatformMacOSX.h
 */

#ifndef PLATFORMMACOSX_H_
#define PLATFORMMACOSX_H_

#ifdef __OBJC__

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>


@interface PlatformMacOSX : NSObject <UIApplicationDelegate>
{
    Game _game;
    
    NSTimer*  _timer;
    NSDate* mDate;
    FLOAT mStartTime;
}

- (void)run;
- (void)frame:(id)sender;

@property (retain) NSTimer* mTimer;
@property (nonatomic) double mLastFrameTime;
@property (nonatomic) double mStartTime;

@end

#endif

#endif
