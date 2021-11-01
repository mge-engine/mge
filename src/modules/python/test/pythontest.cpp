// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "pythontest.hpp"

namespace mge::python {

    void pythontest::SetUp() { engine = mge::script_engine::create("python"); }

    void pythontest::TearDown() { engine.reset(); }

    MGE_REGISTER_IMPLEMENTATION(pythontest_application,
                                mge::application,
                                pythontest);

} // namespace mge::python
APPLICATIONTEST_MAIN(pythontest);