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
    };

    MGE_DECLARE_REF(script_context);

} // namespace mge