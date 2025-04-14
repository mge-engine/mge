// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include <algorithm>
#include <chrono>
#include <mutex>

namespace mge {

    static std::once_flag s_calibrate_delay_loop_flag;

    void delay_loop(uint64_t count)
    {
        for (uint64_t i = 0; i < count; ++i) {
            _ReadWriteBarrier(); // Prevent compiler from optimizing
        }
    }

    static uint64_t compute_bogomips()
    {

        uint64_t                        loops = 2048;
        const std::chrono::microseconds jiffy(10000); // 10ms
        std::chrono::microseconds       elapsed(0);

        while (elapsed < jiffy) {
            auto start = std::chrono::high_resolution_clock::now();
            delay_loop(loops * 2);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                end - start);
            if (elapsed < jiffy) {
                loops *= 2;
            }
        }

        uint64_t step = loops / 2;
        while (step > 0) {
            auto start = std::chrono::high_resolution_clock::now();
            delay_loop(loops + step);
            auto end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                end - start);
            if (elapsed < jiffy) {
                loops += step;
            }
            step /= 2;
        }
        auto bogomips = loops / (500000 / 100);
        return bogomips;
    }

    static void calibrate_delay_loop()
    {
        benchmark::bogomips = compute_bogomips();
    }

    uint64_t benchmark::bogomips = 0;

    benchmark::benchmark()
    {
        std::call_once(s_calibrate_delay_loop_flag, calibrate_delay_loop);
    }

    benchmark::~benchmark()
    {
        if (!m_runs.empty()) {
            std::cout << "Benchmark results:" << std::endl;
            for (const auto& [run, results] : m_runs) {
                std::cout << "Run: " << run << ": " << std::endl;
                for (const auto& result : results) {
                    std::cout << "  " << result.name << ": \t" << result.value
                              << " " << result.unit << std::endl;
                }
            }
        }
    }

} // namespace mge