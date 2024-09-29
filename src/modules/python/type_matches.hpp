#pragma once
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_identifier.hpp"
#include "python.hpp"

namespace mge::python {

    enum class match_type
    {
        NO_MATCH,
        CAST_MATCH,
        EXACT_MATCH
    };

    match_type type_matches(PyObject*                           object,
                            const mge::script::type_identifier& id);

} // namespace mge::python