// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/update_thread.hpp"
#include "mge/core/crash.hpp"
#include "mge/application/application.hpp"
#include "mge/core/log.hpp"
#include "mge/core/clock.hpp"

MGE_USE_LOG(APPLICATION);

namespace mge {

    volatile update_thread *
    update_thread::s_update_thread;

    update_thread::update_thread()
    {
        if(s_update_thread != nullptr) {
            MGE_THROW(mge::illegal_state)
                      << "Update thread is supposed to be a singleton";
        }
    }

    update_thread::~update_thread()
    {
        if(s_update_thread != nullptr) {
            crash("Update thread still active on destruction");
        }
    }

    void
    update_thread::execute_work_item()
    {
        auto task = m_tasks.pop_front();
        if (task) {
            task->run();
        }
    }

    void
    update_thread::run()
    {
        try {
            s_update_thread = this;
            s_update_thread = nullptr;

            auto& app = application::instance();
            MGE_DEBUG_LOG(APPLICATION) << "Main update loop started";
            auto next_game_tick = clock::now();
            uint32_t update_rate =app.update_rate();
            uint32_t skip_ticks = 1000000000u / update_rate;
            uint32_t max_frame_skip = app.max_frame_skip();
            MGE_DEBUG_LOG(APPLICATION) << "Update rate: " << update_rate << " Hz";
            MGE_DEBUG_LOG(APPLICATION) << "Skip ticks: " << skip_ticks << " ns";
            MGE_DEBUG_LOG(APPLICATION) << "Maximum skipped frames: " << max_frame_skip;
            while (!app.is_quit()) {
                uint32_t loops = 0;
                update_rate = app.update_rate();
                skip_ticks = 1000000000u / update_rate;
                max_frame_skip = app.max_frame_skip();
                loops = 0;
                for (;;) {
                    auto update_ts = clock::now();
                    if (!(update_ts > next_game_tick && loops < max_frame_skip)) {
                        break;
                    }
                    app.update(update_ts);
                    update_ts = clock::now();
                    if (!(update_ts > next_game_tick && loops < max_frame_skip)) {
                        break;
                    }
                    while (!m_tasks.empty()) {
                        execute_work_item();
                        update_ts = clock::now();
                        if (!(update_ts > next_game_tick && loops < max_frame_skip)) {
                            break;
                        }
                    }
                    if (!(update_ts > next_game_tick && loops < max_frame_skip)) {
                        break;
                    }
                    next_game_tick += skip_ticks;
                    ++loops;
                }
            }
            s_update_thread = nullptr;
            MGE_DEBUG_LOG(APPLICATION) << "Main loop finished";

        } catch(...) {
            s_update_thread = nullptr;
            mge::rethrow();
        }
    }
}
