#import "ViewController.h"

#import <gameplay.h>


@implementation ViewController

- (void)viewDidLoad 
{
    [super viewDidLoad];
    gameplay::Platform* platform = gameplay::getPlatform();
    platform->startup((unsigned long)((__bridge void*)self), 0);
    platform->enterMessagePump();
}


- (void)setRepresentedObject:(id)representedObject 
{
    [super setRepresentedObject:representedObject];
}


@end
