// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googlemock.hpp"
#include "mge/core/async_executor.hpp"

namespace mge {
    class mock_async_executor : public mge::async_executor
    {
    public:
        MOCK_METHOD1(await, void(const std::function<void()>&));
    };
}
