// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>

namespace mge {

#ifdef MGE_COMPILER_MSVC
#    pragma optimize("", off)
    template <typename T> void do_not_optimize_away(T&& data)
    {
        data = data;
    }
#    pragma optimize("", on)
#elif defined(MGE_COMPILER_GCC) || defined(MGE_COMPILER_APPLECLANG)
    template <typename T> void do_not_optimize_away(T&& data)
    {
        asm volatile("" : "+r"(data) : : "memory");
    }
#else
#    error Missing port
#endif

    class benchmark
    {
    public:
        benchmark();
        ~benchmark();

        struct result
        {
            std::string name;
            std::string unit;
            double      value{0.0};
        };

        using clock =
            std::conditional<std::chrono::high_resolution_clock::is_steady,
                             std::chrono::high_resolution_clock,
                             std::chrono::steady_clock>::type;

        template <typename O>
        MGE_NO_INLINE benchmark& run(const char* name, O&& op)
        {
            auto loops = calibrate(std::forward<O>(op));
            auto start = clock::now();
            for (uint64_t i = 0; i < loops; ++i) {
                op();
            }
            auto stop = clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
                stop - start);
            auto duration = elapsed.count() / static_cast<double>(loops);
            if (duration < 1e-6) {
                std::stringstream msg;
                msg << "Benchmark '" << name << "' did run too little time";
                throw std::runtime_error(msg.str());
            }
            auto ops_per_second = 1000000000.0 / duration;
            auto ops_per_frame = ops_per_second / 60.0;
            auto bogomips_relative =
                ops_per_second / static_cast<double>(bogomips);

            std::string unit = "ns";
            if (duration > 1000) {
                duration /= 1000;
                unit = "us";
            }
            if (duration > 1000) {
                duration /= 1000;
                unit = "ms";
            }
            m_runs[name].push_back({"elapsed time", unit, duration});
            m_runs[name].push_back({"ops per second", "", ops_per_second});
            m_runs[name].push_back({"ops per frame", "", ops_per_frame});
            m_runs[name].push_back(
                {"bogomips relative", "ratio", bogomips_relative});

            return *this;
        }

        template <typename O> MGE_NO_INLINE uint64_t calibrate(O&& op)
        {
            std::chrono::microseconds jiffies(10000);
            std::chrono::microseconds elapsed(0);
            uint64_t                  loops = 1;
        try_again:
            while (elapsed < jiffies) {
                auto start = clock::now();
                for (uint64_t i = 0; i < loops; ++i) {
                    op();
                }
                auto stop = clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                    stop - start);
                if (elapsed < jiffies && loops < 1000000000) {
                    loops *= 2;
                } else {
                    break;
                }
            }
            if (loops == 1 && jiffies < std::chrono::microseconds(2000000)) {
                jiffies *= 2;
                elapsed = std::chrono::microseconds(0);
                goto try_again;
            }

            return loops;
        }

        static uint64_t bogomips;

    private:
        std::map<std::string, std::vector<result>> m_runs;
    };

} // namespace mge