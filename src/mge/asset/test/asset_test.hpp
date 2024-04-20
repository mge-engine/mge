// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/applicationtest.hpp"
#include "test/googletest.hpp"
namespace mge {
    class asset_test_application : public mge::test_application
    {};

    class asset_test : public ::testing::Test
    {
    public:
        asset_test() = default;
        ~asset_test() = default;

        void SetUp() override;
        void TearDown() override;
    };

} // namespace mge
