// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#endif
#include <functional>
#include <future>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5204)
#endif

namespace mge {

    MGE_DECLARE_REF(task);

    class MGE_CORE_EXPORT task : public noncopyable
    {
    public:
        explicit task(const std::function<void()> &f);
        explicit task(std::function<void()> &&f);

        ~task() = default;

        void run();
        void wait();

    private:
        std::function<void()> m_function;
        std::promise<void>    m_result;
    };
} // namespace mge
