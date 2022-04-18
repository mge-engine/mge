// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_type.hpp"

namespace mge::python {

    python_type::python_type(python_context&                      context,
                             const mge::script::type_details_ref& type)
        : m_context(context)
        , m_type(type)
    {}

} // namespace mge::python