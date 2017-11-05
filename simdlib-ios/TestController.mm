//
//  TestController.m
//  VectorialHost
//
//  Created by Daniel Doubleday on 16/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//
#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_COLOUR_NONE
//#define CATCH_CONFIG_NOSTDOUT
#define CATCH_CONFIG_CONSOLE_WIDTH 50

#include "catch.hpp"
#import <Foundation/Foundation.h>
#include "TestController.h"
#include "benchpress/benchpress.hpp"

benchpress::registration* benchpress::registration::d_this;

class LogStreamBuffer : public std::streambuf
{
public:
    LogStreamBuffer(logFunction logger) :
        _logger(logger)
    {}
    
    int overflow(int c) override
    {
        if (c != std::char_traits<char>::eof()) {
            _buffer.push_back(c);
        }
        return c != '\n';
    }
    int sync() override
    {
        _logger(_buffer.c_str());
        _buffer.clear();
        return 0;
    }
    
private:
    std::string _buffer;
    logFunction _logger;
};

@interface TestController ()
{
    Catch::Session  *session;
    LogStreamBuffer *streambuf;
    std::streambuf  *stdCoutStreambuf;
    std::streambuf  *stdCerrStreambuf;
}

- (void)redirectStdStreams:(logFunction)logger;

@end

@implementation TestController

- (id)initWithLogger:(logFunction)logger
{
    if (self = [super init]) {
        self.log = logger;
        [self redirectStdStreams:logger];
        session = new Catch::Session();
    }
    return self;
}

- (void)redirectStdStreams:(logFunction)logger
{
    streambuf = new LogStreamBuffer(logger);
    stdCoutStreambuf = std::cout.rdbuf(streambuf);
    stdCerrStreambuf = std::cerr.rdbuf(streambuf);
}

- (void)dealloc
{
    std::cout.rdbuf(stdCoutStreambuf);
    std::cerr.rdbuf(stdCerrStreambuf);

    delete session;
    delete streambuf;
}

- (void)runTests
{
    self.log("Start running tests\n");
    
    auto result = session->run();
    
    self.log("Done.\n");
}

- (void)runBenchmarks
{
    self.log("Start running benchmarks\n");
    
    std::chrono::high_resolution_clock::time_point bp_start = std::chrono::high_resolution_clock::now();
    
    benchpress::options bench_opts;
    
    // benchmarks to run ...
//    bench_opts.bench("");
    auto benchmarks = benchpress::registration::get_ptr()->get_benchmarks();
    for (auto& info : benchmarks)
    {
        benchpress::context c(info, bench_opts);
        auto r = c.run();
        std::cout << std::setw(35) << std::left << info.get_name() << r.to_string() << std::endl;
    }

    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - bp_start
    ).count() / 1000.f;
    std::cout <<  duration << "s" << std::endl;
}

@end
