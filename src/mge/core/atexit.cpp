// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/atexit.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/trace.hpp"
#include <atomic>
#include <cstdio>
#include <vector>

namespace mge {

    MGE_USE_TRACE(CORE);

    class atexit_callback_runner
    {
    public:
        atexit_callback_runner() = default;
        ~atexit_callback_runner();

        void add(atexit::callback&& cb)
        {
            m_callbacks.push_back(std::move(cb));
        }

        std::vector<atexit::callback> m_callbacks;
    };

    atexit_callback_runner g_atexit_callback_runner;
    std::atomic<bool>      g_atexit_processing_stopped = false;

    void atexit::run(callback&& cb)
    {
        g_atexit_callback_runner.add(std::move(cb));
    }

    void atexit::stop_processing()
    {
        g_atexit_processing_stopped = true;
    }

    atexit_callback_runner::~atexit_callback_runner()
    {
        if (g_atexit_processing_stopped) {
            MGE_DEBUG_TRACE(CORE, "Atexit processing stopped");
        } else {
            MGE_TRACE_OBJECT(CORE, DEBUG) << "Running atexit callbacks";
            for (auto rit = m_callbacks.begin(); rit != m_callbacks.end();
                 ++rit) {
                auto& cb = *rit;
                try {
                    cb();
                    fflush(stdout);
                    fflush(stderr);
                    if (g_atexit_processing_stopped) {
                        MGE_DEBUG_TRACE(CORE, "Atexit processing stopped");
                        break;
                    }
                } catch (const mge::exception& e) {
                    MGE_ERROR_TRACE(CORE,
                                    "Exception in atexit callback: {}",
                                    e.what());
                } catch (const std::exception& e) {
                    MGE_ERROR_TRACE(CORE,
                                    "Exception in atexit callback: {}",
                                    e.what());
                } catch (...) {
                    MGE_ERROR_TRACE(CORE,
                                    "Unknown exception in atexit callback");
                }
            }
        }
    }

} // namespace mge