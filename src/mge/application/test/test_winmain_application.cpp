// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(WINMAINAPP);

    class winmainapp : public application
    {
    public:
        winmainapp() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(WINMAINAPP, "Setup winmainapp");
        }

        void teardown() override
        {
            MGE_DEBUG_TRACE(WINMAINAPP, "Teardown winmainapp");
        }

        void run() override
        {
            MGE_DEBUG_TRACE(WINMAINAPP, "Run winmainapp");
            set_quit();
            application::run();
        }
    };

    MGE_REGISTER_IMPLEMENTATION(winmainapp,
                                mge::application,
                                test_winmain_application);
} // namespace mge

MGE_WINMAINFUNCTION
