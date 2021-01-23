// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <variant>

namespace mge {

    class benchmark_stages : public benchmark
    {
    public:
        class warmup_stage : public benchmark::stage
        {
        public:
            warmup_stage() : loops(10), iterations(10) {}

            uint64_t loops;
            uint64_t iterations;

            uint64_t update(benchmark &b, uint64_t performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                if (--loops == 0) {
                    iterations = 0;
                }
                return iterations;
            }
        };

        class calibrate_stage : public benchmark::stage
        {
        public:
            calibrate_stage() : loops(10), iterations(10), current_loop(0) {}

            uint64_t loops;
            uint64_t iterations;
            uint64_t current_loop;

            uint64_t initial_iterations() const override { return iterations; }

            uint64_t update(benchmark &b, uint64_t performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                ++current_loop;
                if (current_loop == loops) {
                    return 0;
                }

                auto duration = end - start;
                auto casted =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(
                        duration);
                std::cout << "Measured: " << casted.count() << std::endl;
                return 0;
            }
        };
        class measure_stage : public benchmark::stage
        {
            uint64_t update(benchmark &b, uint64_t iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                return 0;
            }
        };
        class done_stage : public benchmark::stage
        {
            uint64_t update(benchmark &b, uint64_t iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                return 0;
            }
        };
    }; // namespace mge

    uint64_t benchmark::stage::initial_iterations() const { return 0; }

    benchmark::benchmark() { compute_clock_resolution(); }

    void benchmark::compute_clock_resolution()
    {
        auto resolution_duration = clock::duration::max();

        clock::time_point start, finish;
        for (size_t i = 0; i < 10; ++i) {
            start = clock::now();
            do {
                finish = clock::now();
            } while (finish == start);
            resolution_duration = std::min(resolution_duration, finish - start);
        }
        m_clock_resolution = resolution_duration;
    }

    void benchmark::reset()
    {
        m_next_iterations = 10;
        m_current_stage   = WARMUP;
        m_stages.emplace_back(
            std::make_unique<benchmark_stages::warmup_stage>());
        m_stages.emplace_back(
            std::make_unique<benchmark_stages::calibrate_stage>());
        m_stages.emplace_back(
            std::make_unique<benchmark_stages::measure_stage>());
        m_stages.emplace_back(std::make_unique<benchmark_stages::done_stage>());
    }

    uint64_t benchmark::next_iterations() const { return m_next_iterations; }

    void benchmark::start_measuring() { m_measure_start = clock::now(); }

    void benchmark::stop_measuring()
    {
        clock::time_point measure_end = clock::now();
        m_next_iterations =
            update(m_next_iterations, m_measure_start, measure_end);
    }

    uint64_t benchmark::update(uint64_t iterations, clock::time_point start,
                               clock::time_point end)
    {
        std::cout << "update: iterations" << iterations << std::endl;

        uint64_t result =
            m_stages.at(m_current_stage)->update(*this, iterations, start, end);
        if (result == 0) {
            ++m_current_stage;
            if (m_current_stage >= DONE) {
                m_current_stage = DONE;
            }
            result = m_stages.at(m_current_stage)->initial_iterations();
        }
        return result;
    }
} // namespace mge