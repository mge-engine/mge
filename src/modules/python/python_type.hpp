#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/type_data.hpp"

#include "python_fwd.hpp"

namespace mge::python {

    class python_type
    {
    public:
        python_type() = default;
        virtual ~python_type() = default;
    };
} // namespace mge::python
