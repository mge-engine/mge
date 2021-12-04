#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    template <typename T> class variable
    {
    public:
        variable(T& value)
            : m_value(value)
        {}

    private:
        T& m_value;
    };
} // namespace mge::script