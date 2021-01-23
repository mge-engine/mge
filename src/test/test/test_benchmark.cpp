// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include "test/googletest.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

TEST(benchmark, slow_cycle)
{
    mge::benchmark().run("sleep200", [&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    });
}

#if 0
TEST(benchmark, chrono)
{

    std::cout << "period: num"
              << std::chrono::high_resolution_clock::period::num << "den "
              << std::chrono::high_resolution_clock::period::den << std::endl;
}

TEST(benchmark, tryadjust)
{
    int loops = 10;
    for (int i = 0; i < 10; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int k = 0; k < loops; ++k) {
            std::string x("hello");
            x = x + x + x;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = (end - start);
        std::cout << "elapsed: " << dur.count() << std::endl;
        std::cout << "used loops: " << loops << std::endl;
        loops = (int)((1000.0 / (double)dur.count()) * loops);
    }
}

template <typename T> void printRatio()
{
    std::cout << "  precision: " << T::num << "/" << T::den << " second "
              << std::endl;
    typedef typename std::ratio_multiply<T, std::kilo>::type MillSec;
    typedef typename std::ratio_multiply<T, std::mega>::type MicroSec;
    std::cout << std::fixed;
    std::cout << "             "
              << static_cast<double>(MillSec::num) / MillSec::den
              << " milliseconds " << std::endl;
    std::cout << "             "
              << static_cast<double>(MicroSec::num) / MicroSec::den
              << " microseconds " << std::endl;

    std::cout << "1000 cycles   "
              << (static_cast<double>(MillSec::num) * 1000.0) / MillSec::den
              << " milliseconds " << std::endl;
}

TEST(benchmark, chronobono)
{
    std::cout << std::boolalpha << std::endl;

    std::cout << "std::chrono::system_clock: " << std::endl;
    std::cout << "  is steady: " << std::chrono::system_clock::is_steady
              << std::endl;
    printRatio<std::chrono::system_clock::period>();

    std::cout << std::endl;

    std::cout << "std::chrono::steady_clock: " << std::endl;
    std::cout << "  is steady: " << std::chrono::steady_clock::is_steady
              << std::endl;
    printRatio<std::chrono::steady_clock::period>();

    std::cout << std::endl;

    std::cout << "std::chrono::high_resolution_clock: " << std::endl;
    std::cout << "  is steady: "
              << std::chrono::high_resolution_clock::is_steady << std::endl;
    printRatio<std::chrono::high_resolution_clock::period>();

    std::cout << std::endl;
}
#endif