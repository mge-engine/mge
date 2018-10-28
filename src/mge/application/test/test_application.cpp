// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/application/application.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/component.hpp"

using mge::application;

class my_application : public application
{
public:
    my_application()
    {
        s_instance = this;
    }

    ~my_application()
    {}

    void start() override
    {
        int argc = this->argc();
        char **argv = (char **) this->argv();
        ::testing::InitGoogleTest(&argc, argv);
        try {
            set_return_code(RUN_ALL_TESTS());
        } catch(const std::exception& ex) {
            std::cerr << "Exception caught: " << ex.what() << std::endl;
            set_return_code(1);
        }

        if(!is_quit()) {
            set_quit();
        }

    }

    static my_application* s_instance;
};

TEST(application, instance)
{
    EXPECT_EQ(my_application::s_instance, &application::instance());
}

TEST(application, set_quit)
{
    EXPECT_FALSE(application::instance().is_quit());
    application::instance().set_quit();
    EXPECT_TRUE(application::instance().is_quit());
}

MGE_REGISTER_IMPLEMENTATION(my_application,
                            application, my_app);

my_application* my_application::s_instance = 0;

MGE_MAINFUNCTION


