//  Created by Tobias Ottenweller on 16/04/15.
//  Copyright (c) 2015 Tobias Ottenweller.

#import "ViewController.h"

#import <PebbleKit/PebbleKit.h>

@interface ViewController () <UITextFieldDelegate>

@property (strong, nonatomic) PBWatch *watch;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // setup connection to watch and watch app
    self.watch = [[PBPebbleCentral defaultCentral] lastConnectedWatch];
    uuid_t myAppUUIDbytes;
    NSUUID *myAppUUID = [[NSUUID alloc] initWithUUIDString:@"186126b1-f071-4659-8270-220a8140d095"];
    [myAppUUID getUUIDBytes:myAppUUIDbytes];
    [[PBPebbleCentral defaultCentral] setAppUUID:[NSData dataWithBytes:myAppUUIDbytes length:sizeof(myAppUUIDbytes)]];
}


#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    NSString *text = textField.text;
    NSDictionary *update = @{ @1: text };
    
    [self.watch appMessagesPushUpdate:update
                               onSent:^(PBWatch *watch, NSDictionary *update, NSError *error) {
                                   
                                   if (error) {
                                       NSLog(@"failed sending with error: %@", error);
                                   }
    }];
    return NO;
}

@end
