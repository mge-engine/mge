// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/loop_target.hpp"
#include "test/googlemock.hpp"

class MOCK_loop_target : public mge::loop_target
{
public:
    MOCK_loop_target() = default;

    MOCK_METHOD(bool, is_quit, (), (const));
    MOCK_METHOD(void, input, (uint64_t));
    MOCK_METHOD(void, update, (uint64_t, double));
    MOCK_METHOD(void, present, (uint64_t, double));
};
