#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    /**
     * Context of a call.
     *
     * When a script calls a function or method, is used to request the
     * parameter values.
     */
    class MGESCRIPT_EXPORT call_context
    {
    public:
        call_context();
        ~call_context();
    };

} // namespace mge::script