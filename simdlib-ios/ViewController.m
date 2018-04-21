//
//  ViewController.m
//  VectorialHost
//
//  Created by Daniel Doubleday on 12/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#import "ViewController.h"
#import "TestController.h"

ViewController* myViewController;
void logToTextView(const char* str)
{
    id nsstr = [NSString stringWithCString:str encoding:NSASCIIStringEncoding];
    if (myViewController)
    {
        [myViewController log:nsstr];
    }
}

@interface ViewController ()

@property TestController* testController;

@end

@implementation ViewController

- (IBAction)startButton_TouchUpInside:(id)sender {
    [self.textView setText:@""];
    [self.testController runTests];
}

- (IBAction)benchButton_TouchUpInside:(id)sender {
    [self.textView setText:@""];
    [self.testController runBenchmarks];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    myViewController = self;
    [self.textView setText:@""];
    self.testController = [[TestController alloc] initWithLogger:logToTextView];
}

- (void)log:(NSString*)message
{
    dispatch_async(dispatch_get_main_queue(), ^{
        NSAttributedString* attr = [[NSAttributedString alloc] initWithString:message attributes:@{
            NSFontAttributeName: [UIFont fontWithName:@"Courier" size:10]
        }];
        [[self.textView textStorage] appendAttributedString:attr];
        [self.textView scrollRangeToVisible:NSMakeRange([[self.textView text] length], 0)];
    });
}
@end
