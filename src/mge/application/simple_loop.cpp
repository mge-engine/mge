// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/loop.hpp"
#include <chrono>

namespace mge {
    class simple_loop : public loop
    {
    public:
        using clock       = std::chrono::high_resolution_clock;
        using double_secs = std::chrono::duration<double>;

        simple_loop()  = default;
        ~simple_loop() = default;

        void run(loop_target &t) override
        {
            auto before = clock::now();

            while (!t.is_quit()) {
                auto current = clock::now();

                t.input();

                double_secs delta = current - before;
                before     = current;

                t.update(delta.count());

                t.present(0.0);
            }
        }
    };

    MGE_REGISTER_IMPLEMENTATION(simple_loop, loop, simple);
}
