// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/component.hpp"
#include "mge/core/stdexceptions.hpp"
#include "test/googletest.hpp"

using mge::application;

class dx12test_application : public application
{
public:
    dx12test_application() {}

    ~dx12test_application() {}

    void start() override
    {
        int    argc = this->argc();
        char **argv = (char **)this->argv();
        ::testing::InitGoogleTest(&argc, argv);
        try {
            set_return_code(RUN_ALL_TESTS());
        } catch (const std::exception &ex) {
            std::cerr << "Exception caught: " << ex.what() << std::endl;
            set_return_code(1);
        }

        if (!is_quit()) {
            set_quit();
        }
    }
};

MGE_REGISTER_IMPLEMENTATION(dx12test_application, application,
                            dx12test_application);

MGE_MAINFUNCTION
