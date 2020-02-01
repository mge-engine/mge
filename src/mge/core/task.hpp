// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/types.hpp"
#include "mge/core/dllexport.hpp"

#include <functional>
#include <future>

namespace mge {

    MGE_DECLARE_REF(task);

    class MGE_CORE_EXPORT task
            : public noncopyable
    {
    public:
        explicit task(const std::function<void()>& f);
        explicit task(std::function<void()>&& f);

        ~task() = default;

        void run();
        void wait();
    private:
        std::function<void()> m_function;
        std::promise<void>    m_result;
    };
}
