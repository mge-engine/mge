#include "type_matches.hpp"

namespace mge::python {

    match_type type_matches(PyObject*                           object,
                            const mge::script::type_identifier& id)
    {
        return match_type::NO_MATCH;
    }
} // namespace mge::python