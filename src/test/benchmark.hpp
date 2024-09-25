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
    template <typename T> void do_not_optimize_away(T&& data) { data = data; }
#    pragma optimize("", on)
#else
#    error Missing port
#endif

    class benchmark
    {
    protected:
        using clock =
            std::conditional<std::chrono::high_resolution_clock::is_steady,
                             std::chrono::high_resolution_clock,
                             std::chrono::steady_clock>::type;

        class stage
        {
        public:
            virtual ~stage() = default;

            virtual uint64_t initial_iterations() const;
            virtual void     set_iterations(uint64_t iterations);
            virtual uint64_t update(benchmark&                   b,
                                    uint64_t                     iterations,
                                    benchmark::clock::time_point start,
                                    benchmark::clock::time_point end) = 0;
        };

    public:
        struct result
        {
            std::string                   benchmark;
            std::map<std::string, double> kpi;
        };

        benchmark();

        ~benchmark();

        template <typename O>
        MGE_NO_INLINE benchmark& run(const char* name, O&& op)
        {
            reset(name);
            while (auto loops = next_iterations()) {
                start_measuring();
                while (loops-- > 0) {
                    op();
                }
                stop_measuring();
            }
            return *this;
        }

        benchmark& show_results();

        /**
         * @brief Get clock resolution of benchmark clock.
         *
         * @return duration of smallest measurement possible
         */
        clock::duration clock_resolution() const;

        void set_measure_iterations(uint64_t iterations);

        const std::string& current() const;

        const auto& results() const;

        void submit(const benchmark::result& r);

    private:
        const uint8_t WARMUP = 0;
        const uint8_t CALIBRATE = 1;
        const uint8_t MEASURE = 2;
        const uint8_t DONE = 3;

        uint64_t next_iterations() const;
        void     reset(const char* name);
        void     compute_clock_resolution();
        void     start_measuring();
        void     stop_measuring();
        uint64_t update(uint64_t          iterations,
                        clock::time_point start,
                        clock::time_point end);
        void     print_results();

        using stage_vector = std::vector<std::unique_ptr<stage>>;

        clock::time_point   m_measure_start;
        uint64_t            m_next_iterations;
        clock::duration     m_clock_resolution;
        uint8_t             m_current_stage;
        bool                m_print_results;
        stage_vector        m_stages;
        std::string         m_current;
        std::vector<result> m_results;
    };

    std::ostream& operator<<(std::ostream& os, const benchmark::result& r);
} // namespace mge