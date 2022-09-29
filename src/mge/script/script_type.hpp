#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {
    class MGESCRIPT_EXPORT script_type
    {
    protected:
        script_type() = default;

    public:
        virtual ~script_type() = default;
    };
} // namespace mge::script