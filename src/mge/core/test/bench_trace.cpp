// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include "test/googletest.hpp"

#include "mge/core/configuration.hpp"
#include "mge/core/memory_trace_sink.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(TRACEBENCHMARK);

    class trace_benchmark : public ::testing::Test
    {
    protected:
        static void SetUpTestCase()
        {
            if (!mge::configuration::loaded()) {
                mge::configuration::load();
            }
        }
    };

    TEST_F(trace_benchmark, trace)
    {
        auto sink = std::make_shared<mge::memory_trace_sink>();
        sink->set_capacity(1000);
        MGE_TRACE_TOPIC(TRACEBENCHMARK).add_sink(sink);

        mge::benchmark().run("trace_disabled", [&]() {
            MGE_DEBUG_TRACE(TRACEBENCHMARK) << "Hello";
        });

        MGE_TRACE_TOPIC(TRACEBENCHMARK).set_level(trace_level::ALL);

        mge::benchmark().run("trace_enabled", [&]() {
            MGE_DEBUG_TRACE(TRACEBENCHMARK) << "Hello";
        });
        EXPECT_NE(0u, sink->size());
    }
} // namespace mge