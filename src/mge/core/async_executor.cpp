// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/async_executor.hpp"

namespace mge {

    class synchronous_executor : public async_executor
    {
        void await(const std::function<void()> &f) { f(); }
    };

    async_executor &async_executor::synchronous_executor()
    {
        static class synchronous_executor ex;
        return ex;
    }
} // namespace mge
