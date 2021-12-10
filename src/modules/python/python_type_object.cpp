#include "python_type_object.hpp"

namespace mge::python {

    python_type_object::python_type_object(const std::string& name, size_t size)
        : m_name(name)
    {}
} // namespace mge::python