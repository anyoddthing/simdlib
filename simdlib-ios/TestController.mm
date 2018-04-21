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

struct LogStreamBuffer : std::streambuf
{
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

struct BenchmarkRunner
{
    std::thread run()
    {
        return std::thread(std::bind(&BenchmarkRunner::runBenchmarks, this));
    }
    
private:

    void runBenchmarks()
    {
        std::cout << "Start running benchmarks" << std::endl;;
        
        std::chrono::high_resolution_clock::time_point bp_start = std::chrono::high_resolution_clock::now();
        
        benchpress::options bench_opts;
        
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
    std::cout << "Start running tests" << std::endl;;
    
    auto result = session->run();
    
    std::cout << "Done." << std::endl;;
}

- (void)runBenchmarks
{
    // run the benchmark in own thread to get log view updated 
    BenchmarkRunner runner;
    auto thread = runner.run();
    thread.detach();
}

@end
