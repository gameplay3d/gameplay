#import <GameKit/GameKit.h>
#import "BGPViewController.h"
#import "BGPView.h"

#define UIInterfaceOrientationEnum(x) ([x isEqualToString:@"UIInterfaceOrientationPortrait"]?UIInterfaceOrientationPortrait:                        \
                                      ([x isEqualToString:@"UIInterfaceOrientationPortraitUpsideDown"]?UIInterfaceOrientationPortraitUpsideDown:    \
                                      ([x isEqualToString:@"UIInterfaceOrientationLandscapeLeft"]?UIInterfaceOrientationLandscapeLeft:              \
                                        UIInterfaceOrientationLandscapeRight)))

@implementation BGPViewController

- (id)init
{
    return [super init];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - View lifecycle

- (void)loadView
{
    [self setView:[[BGPView alloc] init]];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Fetch the supported orientations array
    NSArray *supportedOrientations = NULL;

    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        supportedOrientations = [[NSBundle mainBundle] infoDictionary][@"UISupportedInterfaceOrientations~ipad"];
    }
    else if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
    {
        supportedOrientations = [[NSBundle mainBundle] infoDictionary][@"UISupportedInterfaceOrientations~iphone"];
    }

    if (supportedOrientations == NULL)
    {
        supportedOrientations = [[NSBundle mainBundle] infoDictionary][@"UISupportedInterfaceOrientations"];
    }

    // If no supported orientations default to v1.0 handling (landscape only)
    if (supportedOrientations == nil)
    {
        return UIInterfaceOrientationIsLandscape(interfaceOrientation);
    }

    for (NSString *s in supportedOrientations)
    {
        if (interfaceOrientation == UIInterfaceOrientationEnum(s))
        {
            return YES;
        }
    }

    return NO;
}

- (void)startUpdating
{
    [[self gamePlayView] startUpdating];
}

- (void)stopUpdating
{
    [[self gamePlayView] stopUpdating];
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController*)gameCenterViewController
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (BGPView*)gamePlayView
{
    return (BGPView*)[self view];
}

@end
