// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/call_debugger.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/signature.hpp"

#include <tuple>

namespace mge::script {

    /**
     * @brief Invocation context.
     */
    class MGESCRIPT_EXPORT invoke_context
    {
    public:
        invoke_context() = default;
        virtual ~invoke_context() = default;

        template <typename R> R invoke(const char* name)
        {
            call();
            return retrieve_result<R>();
        }

        template <typename R, typename... Args>
        R invoke(const char*                   name,
                 const mge::script::signature& s,
                 const std::tuple<Args...>&    args)
        {
            return R();
        }

        template <typename T> T retrieve_result()
        {
            call_debugger();
            return T();
        }

        virtual void call() = 0;
    };

} // namespace mge::script