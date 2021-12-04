// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python_bind_helper.hpp"
#include "mge/core/trace.hpp"
#include "python_context.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    python_bind_helper::python_bind_helper(python_context& context)
        : m_context(context)
    {}

    python_bind_helper::~python_bind_helper() {}

    void python_bind_helper::begin(const mge::script::module_details& m)
    {
        auto m = std::make_shared<python_module>(mge::script::module(m));
        m_context.add_module(m);
    }
    void python_bind_helper::end(const mge::script::module_details& m) {}

    void python_bind_helper::begin(const mge::script::type_details& t) {}
    void python_bind_helper::end(const mge::script::type_details& t) {}

    void python_bind_helper::begin(const mge::script::function_details& t) {}
    void python_bind_helper::end(const mge::script::function_details& t) {}

} // namespace mge::python