// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/applicationtest.hpp"

class applicationtest_application : public mge::test_application
{};

MGE_REGISTER_IMPLEMENTATION(applicationtest_application,
                            mge::application,
                            applicationtest);

APPLICATIONTEST_MAIN(applicationtest)