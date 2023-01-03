// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <vector>

namespace mge {

    class benchmark_stages : public benchmark
    {
    public:
        struct measure_point
        {
            measure_point() = default;

            measure_point(const measure_point&) = default;

            measure_point& operator=(const measure_point& m) = default;

            measure_point(uint64_t                 iterations_,
                          std::chrono::nanoseconds duration_)
                : iterations(iterations_)
                , duration(duration_)
            {}

            uint64_t                 iterations;
            std::chrono::nanoseconds duration;
        };

        using measure_series = std::vector<measure_point>;

        static measure_point duration_median(measure_series& s)
        {
            if (s.empty()) {
                return measure_point();
            }
            std::sort(
                s.begin(),
                s.end(),
                [](const measure_point& a, const measure_point& b) -> bool {
                    return a.duration < b.duration;
                });
            auto midpoint = s.size() >> 1;
            if (midpoint & 1u) {
                return s[midpoint];
            } else {
                measure_point result;
                result.iterations =
                    (s[midpoint - 1u].iterations + s[midpoint].iterations) / 2;
                result.duration =
                    (s[midpoint - 1u].duration + s[midpoint].duration) / 2;
                return result;
            }
        }

        static measure_point time_per_op_median(measure_series& s)
        {
            if (s.empty()) {
                return measure_point();
            }
            std::sort(
                s.begin(),
                s.end(),
                [](const measure_point& a, const measure_point& b) -> bool {
                    double aval = static_cast<double>(a.duration.count()) /
                                  static_cast<double>(a.iterations);
                    double bval = static_cast<double>(b.duration.count()) /
                                  static_cast<double>(b.iterations);

                    return aval < bval;
                });
            auto midpoint = s.size() >> 1;
            if (midpoint & 1u) {
                return s[midpoint];
            } else {
                measure_point result;
                result.iterations =
                    (s[midpoint - 1u].iterations + s[midpoint].iterations) / 2;
                result.duration =
                    (s[midpoint - 1u].duration + s[midpoint].duration) / 2;
                return result;
            }
        }

        static double deviation(const std::chrono::nanoseconds& a,
                                const std::chrono::nanoseconds& b)
        {
            double aval = static_cast<double>(a.count());
            double bval = static_cast<double>(b.count());
            if (aval == 0.0) {
                return bval == 0.0 ? 0.0 : std::numeric_limits<double>::max();
            }

            double dist = abs(aval - bval);
            return dist / aval;
        }

        class warmup_stage : public benchmark::stage
        {
        public:
            warmup_stage()
                : loops(20)
                , iterations(10)
            {
                series.reserve(loops);
            }

            uint64_t       loops;
            uint64_t       iterations;
            measure_series series;

            uint64_t update(benchmark&                   b,
                            uint64_t                     performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                if (--loops == 0) {
                    return 0;
                }
                auto duration =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(end -
                                                                         start);
                measure_point p(performed_iterations, duration);
                series.push_back(p);
                if (series.size() >= 5) {
                    measure_point m = time_per_op_median(series);
                    double        d = deviation(m.duration, p.duration);
                    if (d > 0.5) {
                        iterations += 10;
                    }
                    if (d < 0.2) {
                        return 0;
                    }
                }
                return iterations;
            }
        };

        class calibrate_stage : public benchmark::stage
        {
        public:
            calibrate_stage()
                : loops(10)
                , max_unstable_loops_add(5)
                , iterations(10)
                , current_loop(0)
                , stable(false)
            {}

            uint64_t              loops;
            uint64_t              max_unstable_loops_add;
            uint64_t              iterations;
            uint64_t              current_loop;
            measure_series        series;
            static constexpr auto wanted_duration =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::milliseconds(2));
            bool stable;

            uint64_t initial_iterations() const override { return iterations; }

            uint64_t update(benchmark&                   b,
                            uint64_t                     performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                ++current_loop;
                if (current_loop == loops) {
                    if (!stable) {
                        if (max_unstable_loops_add == 0) {
                            std::stringstream msg;
                            msg << "Benchmark '" << b.current()
                                << "' unstable after " << current_loop
                                << " iterations";
                            throw std::runtime_error(msg.str());
                        }
                        loops += max_unstable_loops_add;
                        max_unstable_loops_add = 0;
                    } else {
                        return 0;
                    }
                }

                measure_point p(performed_iterations, end - start);
                series.push_back(p);

                if (series.size() >= 5) {
                    measure_point m = time_per_op_median(series);
                    double        d = deviation(m.duration, p.duration);
                    if (d < 0.2) {
                        stable = true;
                    }
                }

                if (p.duration >= wanted_duration) {
                    if (stable) {
                        b.set_measure_iterations(performed_iterations);
                        return 0;
                    }
                } else {
                    double factor =
                        static_cast<double>(wanted_duration.count()) /
                        static_cast<double>(p.duration.count());
                    iterations = static_cast<uint64_t>(
                        static_cast<double>(iterations) * factor);
                }
                return iterations;
            }
        };

        class measure_stage : public benchmark::stage
        {
        public:
            measure_stage()
                : loops(11)
                , iterations(10)
                , current_loops(0)
            {}
            uint64_t                               loops;
            uint64_t                               iterations;
            uint64_t                               current_loops;
            std::mt19937                           rng;
            measure_series                         series;
            std::uniform_int_distribution<int64_t> distributor;

            uint64_t initial_iterations() const override { return iterations; }
            void     set_iterations(uint64_t iterations_)
            {
                iterations = iterations_;
                int64_t max_deviate = iterations / 10;
                if (max_deviate == 0) {
                    max_deviate = 1;
                }
                distributor =
                    std::uniform_int_distribution<int64_t>(-max_deviate,
                                                           max_deviate);
            }

            uint64_t next_iterations()
            {
                int64_t result = iterations + distributor(rng);
                if (result <= 0) {
                    result = 1;
                }
                return result;
            }

            void submit_result(benchmark& b)
            {
                benchmark::result r;

                r.benchmark = b.current();
                auto m = time_per_op_median(series);

                double ns_per_op = static_cast<double>(m.duration.count()) /
                                   static_cast<double>(m.iterations);
                if (ns_per_op < 9e-17) {
                    std::stringstream msg;
                    msg << "Benchmark '" << b.current()
                        << "' did run too little time";
                    throw std::runtime_error(msg.str());
                }
                r.kpi["median ns/op"] = ns_per_op;
                if (ns_per_op != 0) {
                    double ops_sec = 1000000000.0 / ns_per_op;
                    r.kpi["median op/s"] = ops_sec;
                    double ops_frame = 1000000000.0 / (60.0 * ns_per_op);
                    r.kpi["median ops/frame"] = ops_frame;
                } else {
                    r.kpi["median op/s"] =
                        std::numeric_limits<double>::infinity();
                    r.kpi["median op/frame"] =
                        std::numeric_limits<double>::infinity();
                }
                b.submit(r);
            }

            uint64_t update(benchmark&                   b,
                            uint64_t                     performed_iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {
                measure_point p(performed_iterations, end - start);
                series.push_back(p);

                ++current_loops;
                if (current_loops <= loops) {
                    return next_iterations();
                } else {
                    submit_result(b);
                    return 0;
                }
            }
        };
        class done_stage : public benchmark::stage
        {
            uint64_t update(benchmark&                   b,
                            uint64_t                     iterations,
                            benchmark::clock::time_point start,
                            benchmark::clock::time_point end) override
            {

                return 0;
            }
        };
    }; // namespace mge

    uint64_t benchmark::stage::initial_iterations() const { return 0; }

    void benchmark::stage::set_iterations(uint64_t iterations) {}

    benchmark::benchmark()
        : m_print_results(false)
    {
        compute_clock_resolution();
    }

    benchmark::~benchmark()
    {
        if (m_print_results) {
            print_results();
        }
    }

    void benchmark::print_results()
    {
        for (const auto& r : m_results) {
            std::cout << r;
        }
    }

    std::ostream& operator<<(std::ostream& os, const benchmark::result& r)
    {
        os << r.benchmark << std::endl;
        for (const auto& [kpi, measure] : r.kpi) {
            os << "\t" << kpi << "\t" << measure << std::endl;
        }
        return os;
    }

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
        m_clock_resolution = resolution_ns;
    }

    void benchmark::reset(const char* name)
    {
        if (name) {
            m_current = name;
        } else {
            m_current.clear();
        }
        m_next_iterations = 10;
        m_current_stage = WARMUP;
        m_stages.clear();

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

    uint64_t benchmark::update(uint64_t          iterations,
                               clock::time_point start,
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

    const std::string& benchmark::current() const { return m_current; }

    const auto& benchmark::results() const { return m_results; }

    void benchmark::submit(const benchmark::result& r)
    {
        m_results.push_back(r);
    }

    benchmark& benchmark::show_results()
    {
        m_print_results = true;
        return *this;
    }
} // namespace mge