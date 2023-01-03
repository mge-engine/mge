// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
    protected:
        script_context();

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
         * @brief Invoke interpreter as if called from command line.
         * @param argc argument count
         * @param argv argument values
         */
        virtual int main(int argc, const char** argv) = 0;

        /**
         * @brief Binds a module, i.e. makes it visible inside the interpreter.
         *
         * @param m module
         */
        virtual void bind(const mge::script::module& m) = 0;

        /**
         * @brief Binds a list of modules in a container.
         *
         * @tparam ModuleContainer  container type of @c mge::script::module
         * @param c container to bind
         */
        template <typename ModuleContainer> void bind(const ModuleContainer& c)
        {
            for (const auto& el : c) {
                bind(el);
            }
        }

        /**
         * @brief Get type of expression.
         *
         * @param expression  expression in scripting language
         * @return script type
         */
        virtual script_type_ref type_of(const std::string& expression) = 0;
    };
} // namespace mge