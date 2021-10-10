// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/loop.hpp"
#include <chrono>

namespace mge {
    class simple_loop : public loop
    {
    public:
        using clock = std::chrono::high_resolution_clock;
        using double_secs = std::chrono::duration<double>;

        simple_loop() = default;
        ~simple_loop() = default;

        void run(loop_target& t) override
        {
            auto     before = clock::now();
            uint64_t cycle = 0;
            double   elapsed = 0.0;

            while (!t.is_quit()) {
                ++cycle;
                auto current = clock::now();

                t.input(cycle);

                double_secs delta = current - before;
                before = current;

                t.update(cycle, elapsed, delta.count());
                elapsed += delta.count();

                t.present(cycle, 0.0);
            }
        }
    };

    MGE_REGISTER_IMPLEMENTATION(simple_loop, loop, simple);
} // namespace mge
