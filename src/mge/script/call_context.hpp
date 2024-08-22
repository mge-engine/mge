#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <functional>
namespace mge::script {

    /**
     * Context of a call.
     *
     * When a script calls a function or method, is used to request the
     * parameter values.
     */
    class MGESCRIPT_EXPORT call_context
    {
    protected:
        call_context() = default;

    public:
        virtual ~call_context() = default;
    };

    using invoke_function = std::function<void(call_context&)>;

} // namespace mge::script