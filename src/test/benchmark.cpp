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

            uint64_t              loops;
            uint64_t              iterations;
            uint64_t              current_loop;
            static constexpr auto wanted_duration =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::milliseconds(2));

            uint64_t initial_iterations() const override { return iterations; }

            uint64_t update(benchmark &b, uint64_t performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                std::cout << "calibrate::update" << std::endl;
                ++current_loop;
                if (current_loop == loops) {
                    return 0;
                }

                auto duration =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(end -
                                                                         start);

                std::cout << "Measured runtime: " << duration.count() << "ns"
                          << std::endl;
                std::cout << "Wanted runtime  : " << wanted_duration.count()
                          << "ns" << std::endl;

                if (duration >= wanted_duration) {
                    b.set_measure_iterations(performed_iterations);
                    return 0;
                } else {
                    double factor =
                        static_cast<double>(wanted_duration.count()) /
                        static_cast<double>(duration.count());
                    iterations = static_cast<uint64_t>(
                        static_cast<double>(iterations) * factor);
                }

                return iterations;
            }
        };
        class measure_stage : public benchmark::stage
        {
        public:
            measure_stage() : loops(11), iterations(10), current_loops(0) {}
            uint64_t loops;
            uint64_t iterations;
            uint64_t current_loops;
            uint64_t initial_iterations() const override { return iterations; }
            void     set_iterations(uint64_t iterations_)
            {
                iterations = iterations_;
            }
            uint64_t update(benchmark &b, uint64_t performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                ++current_loops;
                if (current_loops <= loops) {
                    return iterations;
                } else {
                    return 0;
                }
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

    void benchmark::stage::set_iterations(uint64_t iterations) {}

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
        auto resolution_ns =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                resolution_duration);
        std::cout << "Clock resolution: " << resolution_ns.count() << "ns"
                  << std::endl;
        m_clock_resolution = resolution_ns;
    }

    void benchmark::reset()
    {
        std::cout << "benchmark::reset" << std::endl;
        m_next_iterations = 10;
        m_current_stage   = WARMUP;
        m_stages.emplace_back(
            std::make_unique<benchmark_stages::warmup_stage>());
        m_stages.emplace_back(
            std::make_unique<benchmark_stages::calibrate_stage>());
        m_stages.emplace_back(
            std::make_unique<benchmark_stages::measure_stage>());
        m_stages.emplace_back(std::make_unique<benchmark_stages::done_stage>());
        std::cout << "benchmark::reset done" << std::endl;
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

    benchmark::clock::duration benchmark::clock_resolution() const
    {
        return m_clock_resolution;
    }

    void benchmark::set_measure_iterations(uint64_t iterations)
    {
        m_stages.at(MEASURE)->set_iterations(iterations);
    }

} // namespace mge