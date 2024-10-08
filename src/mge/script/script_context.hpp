// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge {

    /**
     * Context of script invocation.
     */
    class MGESCRIPT_EXPORT script_context
    {
    protected:
        script_context();

    public:
        virtual ~script_context() = default;

        /**
         * Evaluate a script.
         * @param script script to evaluate
         */
        virtual void eval(const std::string& script) = 0;

        /**
         * Invoke interpreter as if called from command line.
         * @param argc argument count
         * @param argv argument values
         * @return return code
         */
        virtual int main(int argc, const char** argv) = 0;

        /**
         * Bind all exposed modules, types, and functions. Without calling this,
         * the script will just be plain and doesn't know anything aside
         * builtins.
         */
        virtual void bind() = 0;

        /**
         * Resolve bindings without creating a script context.
         */
        static void resolve_bindings();
    };

    MGE_DECLARE_REF(script_context);

} // namespace mge