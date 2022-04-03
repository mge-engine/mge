#include "python_binder.hpp"

namespace mge::python {
    python_binder::python_binder(python_context& context)
        : m_context(context)
    {}

    python_binder::~python_binder() {}

    void python_binder::bind(const mge::script::module& m) { m.apply(*this); }
} // namespace mge::python