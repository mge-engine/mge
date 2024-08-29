// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mock_script_context.hpp"
#include "test/googletest.hpp"

class script_context_test : public testing::Test
{
protected:
    void SetUp() override
    {
        if (!mge::configuration::loaded()) {
            mge::configuration::load();
        }
        mge::module::load_all();
    }
};

TEST_F(script_context_test, create) { MOCK_script_context ctx; }