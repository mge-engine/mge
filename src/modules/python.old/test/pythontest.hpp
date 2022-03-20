// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"
#include "mge/script/script_engine.hpp"
#include "test/applicationtest.hpp"
#include "test/googletest.hpp"

namespace mge::python {

    class pythontest_application : public mge::test_application
    {};

    class pythontest : public ::testing::Test
    {
    public:
        pythontest() = default;
        virtual ~pythontest() = default;

        void SetUp() override;
        void TearDown() override;

        mge::script_engine_ref engine;
    };

} // namespace mge::python