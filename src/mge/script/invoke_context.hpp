#pragma once
#include "mge/script/dllexport.hpp"
namespace mge::script {

    class MGESCRIPT_EXPORT invoke_context
    {
    public:
        invoke_context() = default;
        virtual ~invoke_context() = default;
    };

} // namespace mge::script