// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_module.hpp"

namespace mge::python {
    python_module::python_module(python_context&                       context,
                                 const reflection::module_details_ref& details)
        : m_context(context)
        , m_details(details)
    {}

    python_module::~python_module() {}

} // namespace mge::python