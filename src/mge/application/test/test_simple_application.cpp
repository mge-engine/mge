// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SIMPLEAPP);

    class simpleapp : public application
    {
    public:
        simpleapp() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(SIMPLEAPP, "Setup simpleapp");
        }

        void teardown() override
        {
            MGE_DEBUG_TRACE(SIMPLEAPP, "Teardown simpleapp");
        }

        void run() override
        {
            MGE_DEBUG_TRACE(SIMPLEAPP, "Run simpleapp");
            set_quit();
            application::run();
        }
    };

    MGE_REGISTER_IMPLEMENTATION(simpleapp,
                                mge::application,
                                test_simple_application);
} // namespace mge

MGE_MAINFUNCTION