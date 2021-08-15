// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/loop.hpp"
#include <chrono>

namespace mge {
    class default_loop : public loop
    {
    public:
        using clock = std::chrono::high_resolution_clock;
        using double_secs = std::chrono::duration<double>;

        default_loop() { m_epoch = clock::now().time_since_epoch().count(); }
        ~default_loop() = default;

        uint64_t now() const
        {
            return clock::now().time_since_epoch().count() - m_epoch;
        }

        void run(loop_target& t)
        {
            // auto     next_game_tick = now();
            // uint32_t update_rate    = 25;
            // uint32_t skip_ticks     = 1000000000u / update_rate;
            // uint32_t max_frame_skip = 3;
            while (!t.is_quit()) {
            }
        }

    private:
        uint64_t m_epoch;
    };
} // namespace mge