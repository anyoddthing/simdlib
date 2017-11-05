//
//  ViewController.h
//  VectorialHost
//
//  Created by Daniel Doubleday on 12/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (weak, nonatomic) IBOutlet UITextView *textView;

- (void)log:(NSString*)message;

@end

