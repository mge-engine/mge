#include "mge/application/loop.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"
#include <chrono>

namespace mge {

    MGE_USE_TRACE(APPLICATION);

    class fixed_step_loop : public loop
    {
    public:
        using clock = std::chrono::high_resolution_clock;
        using double_secs = std::chrono::duration<double>;

        fixed_step_loop() {}
        ~fixed_step_loop() = default;

        void run(loop_target& t) override
        {
            uint64_t frame = 0;
            double   total_time = 0.0;
            double   time_step = 0.01;
            auto     current_time = clock::now();
            double   accumulator = 0.0;

            while (!t.is_quit()) {
                ++frame;
                t.input(frame);
                auto        new_time = clock::now();
                double_secs frame_time = new_time - current_time;
                double      frame_time_val = std::min(frame_time.count(), 0.25);

                accumulator += frame_time_val;
                while (accumulator >= time_step) {
                    t.update(frame, total_time, time_step);
                    total_time += time_step;
                    accumulator -= time_step;
                }

                double alpha = accumulator / time_step;
                t.present(frame, alpha);
            }
        }
    };

    MGE_REGISTER_IMPLEMENTATION(fixed_step_loop, loop, fixed, fixed_step);
} // namespace mge