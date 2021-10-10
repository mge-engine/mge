// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/exception.hpp"
#include "test/googletest.hpp"

#include <string_view>
using namespace std::string_view_literals;

TEST(application, argc_argv)
{
    const char* app_argv[] = {"test_application", "foo", "bar"};

    mge::application app;
    app.initialize(3, app_argv);
    EXPECT_EQ(3, app.argc());
    EXPECT_EQ("foo"sv, app.argv(1));
}

TEST(application, argv_throws_for_invalid_index)
{
    const char* app_argv[] = {"test_application", "foo", "bar"};

    mge::application app;
    app.initialize(3, app_argv);
    EXPECT_THROW(app.argv(17), mge::exception);
}

class foobar_app : public mge::application
{
public:
    foobar_app()
    {
        this->add_update_listener(
            [&](uint64_t, double, double) { set_quit(); });
    }
};

MGE_REGISTER_IMPLEMENTATION(foobar_app, mge::application, foobar);

TEST(application, main)
{
    const char* app_argv[] = {"foobar", "foo", "bar"};

    mge::application::main("foobar"sv, 3, app_argv);
}
