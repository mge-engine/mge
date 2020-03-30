// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/clock.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
#include "test/googletest.hpp"

#include <cmath>

using mge::application;

class my_application;

MGE_DEFINE_LOG(TEST);

class my_application : public application
{
public:
    using super = application;

    my_application()
    {
        s_instance        = this;
        update_called     = 0;
        bad_interpolation = 0.0f;
        elapsed_time      = 0.0;
        quit_called       = false;
    }

    ~my_application() {}

    void setup()
    {
        add_update_listener([&](std::int64_t) {
            update_called++;
            if (update_called == 75) {
                set_quit();
            }
            // test exception safety as fly-by
            if (update_called == 10) {
                MGE_THROW(mge::illegal_state);
            }
        });
        add_quit_listener([&]() { quit_called = true; });
        start_ts = mge::clock::now();
    }

    void teardown()
    {
        if (return_code()) {
            return;
        }
        std::int64_t end_ts = mge::clock::now();

        elapsed_time = (double)(end_ts - start_ts) / 1000000000.0;
        elapsed_time = std::round(elapsed_time);

        int    argc = this->argc();
        char **argv = (char **)this->argv();
        ::testing::InitGoogleTest(&argc, argv);
        set_return_code(RUN_ALL_TESTS());
    }

    static my_application *s_instance;

    int     update_called;
    float   bad_interpolation;
    double  elapsed_time;
    int64_t start_ts;
    bool    quit_called;
};

TEST(applicationupdateloop, correct_number_of_update_calls)
{
    EXPECT_EQ(75, my_application::s_instance->update_called);
}

TEST(applicationupdateloop, correct_elapsed_time)
{
    EXPECT_EQ(3.0, my_application::s_instance->elapsed_time);
}

TEST(applicationupdateloop, quit_listener_called)
{
    EXPECT_TRUE(my_application::s_instance->quit_called);
}

my_application *my_application::s_instance = 0;

MGE_REGISTER_IMPLEMENTATION(my_application, application, my_app);
MGE_MAINFUNCTION;
