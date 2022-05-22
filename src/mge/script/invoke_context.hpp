// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
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

        template <typename R> R invoke(const char* name) { return R(); }

        template <typename R>
        R invoke(const char* name, const mge::script::signature& s)
        {
            return R();
        }

        template <typename R, typename... Args>
        R invoke(const char*                   name,
                 const mge::script::signature& s,
                 const std::tuple<Args...>&    args)
        {
            return R();
        }
    };

} // namespace mge::script