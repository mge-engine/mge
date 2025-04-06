// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "pythontest.hpp"

namespace mge::python {

    void pythontest::SetUp()
    {
        std::call_once(s_once_flag, []() {
            engine = mge::script_engine::create("python");
            context = engine->create_context();
            context->bind();
        });

        engine = mge::script_engine::create("python");
    }

    std::once_flag          pythontest::s_once_flag;
    mge::script_engine_ref  pythontest::engine;
    mge::script_context_ref pythontest::context;

    void pythontest_application::teardown()
    {
        pythontest::context.reset();
    }

    MGE_REGISTER_IMPLEMENTATION(pythontest_application,
                                mge::application,
                                pythontest);

} // namespace mge::python
APPLICATIONTEST_MAIN(pythontest);