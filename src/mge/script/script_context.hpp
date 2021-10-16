#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
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
    {};
} // namespace mge