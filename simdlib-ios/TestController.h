//
//  TestController.h
//  VectorialHost
//
//  Created by Daniel Doubleday on 16/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#ifndef TestController_h
#define TestController_h

typedef void (*logFunction)(const char*);

@interface TestController : NSObject

@property logFunction log;

- (id)initWithLogger:(logFunction)logger;

- (void)runTests;
- (void)runBenchmarks;

@end

#endif /* TestController_h */
