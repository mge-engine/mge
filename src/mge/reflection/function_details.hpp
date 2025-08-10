// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    class MGEREFLECTION_EXPORT function_details
    {
    public:
        function_details();
        ~function_details();

        template <typename Ret, typename... Args>
        function_details(const char* name, Ret (*func)(Args...))
        {
            construct(name, reinterpret_cast<void*>(func));
        }

        function_details(const function_details&) = delete;
        function_details& operator=(const function_details&) = delete;
        function_details(function_details&&) = delete;
        function_details& operator=(function_details&&) = delete;

    private:
        void construct(const char* name, void* func);
    };

} // namespace mge::reflection
