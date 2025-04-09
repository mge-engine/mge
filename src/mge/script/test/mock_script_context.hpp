// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/script_context.hpp"
#include "test/googlemock.hpp"

class MOCK_script_context : public mge::script_context
{
public:
    MOCK_script_context() = default;
    ~MOCK_script_context() = default;

    MOCK_METHOD(void, eval, (const std::string&), (override));
    MOCK_METHOD(int, main, (int a, const char** b), (override));
    MOCK_METHOD(void, bind, (), (override));
};
