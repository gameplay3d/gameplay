#import <UIKit/UIKit.h>
#include "Gesture.h"

@interface BGPView : UIView

@property (readonly, nonatomic, getter=isUpdating) BOOL updating;
@property (readonly, nonatomic, getter=getContext) EAGLContext* context;
@property (readonly, nonatomic) BOOL vsync;

- (instancetype)initWithFrame:(CGRect)frame;

- (void)startGame;

- (void)startUpdating;

- (void)stopUpdating;

- (void)update:(id)sender;

- (void)setSwapInterval:(NSInteger)interval;

- (int)swapInterval;

- (void)swapBuffers;

- (BOOL)showKeyboard;

- (BOOL)dismissKeyboard;

- (void)registerGesture:(gameplay::Gesture::GestureEvent)evt;

- (void)unregisterGesture:(gameplay::Gesture::GestureEvent)evt;

- (bool)isGestureRegistered:(gameplay::Gesture::GestureEvent)evt;

@end
