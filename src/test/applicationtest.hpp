// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"
#include "test/googletest.hpp"

namespace mge {

    MGE_USE_TRACE(APPLICATIONTEST);

    class test_application : public application
    {
    public:
        test_application()
            : m_tests_did_run(false)
        {
            add_update_listener([&](uint64_t, double, double) {
                if (!m_tests_did_run) {
                    run_all_tests();
                }
                set_quit();
            });
        }

        void run_all_tests()
        {
            std::vector<const char*> argv;
            for (const auto& s : arguments()) {
                argv.push_back(s.c_str());
            }
            int tmp_argc = argc();
            ::testing::InitGoogleTest(&tmp_argc, const_cast<char**>(&argv[0]));
            set_return_code(RUN_ALL_TESTS());
        }

        bool m_tests_did_run;
    };
} // namespace mge

#define APPLICATIONTEST_MAIN(NAME)                                             \
    MGE_DEFINE_TRACE(APPLICATIONTEST);                                         \
    int main(int argc, const char** argv)                                      \
    {                                                                          \
        return mge::application::main(#NAME, argc, argv);                      \
    }
