#pragma once
#include "mge/script/script_engine.hpp"
#include "test/applicationtest.hpp"
#include "test/googletest.hpp"

namespace mge::lua {

    class luatest_application : public mge::test_application
    {};

    class luatest : public ::testing::Test
    {
    public:
        luatest() = default;
        virtual ~luatest() = default;

        void SetUp() override;
        void TearDown() override;

        mge::script_engine_ref engine;
    };
} // namespace mge::lua