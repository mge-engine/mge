// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string_view>

namespace mge {
    /**
     * A script context provides the frame around the execution of once
     * or more scripts.
     *
     * It provides access to objects defined in the script (or predefined
     * in the interpreter) (globally defined synbols, and means to execute
     * code.
     */
    class MGESCRIPT_EXPORT script_context : public noncopyable
    {
    public:
        /**
         * @brief Destructor.
         */
        virtual ~script_context() = default;

        /**
         * @brief Eval code.
         *
         * @param code code to evaluate
         */
        virtual void eval(const std::string& code) = 0;

        /**
         * @brief Binds a module, i.e. makes it visible inside the interpreter.
         *
         * @param m module
         */
        virtual void bind(const mge::script::module& m) = 0;
    };
} // namespace mge