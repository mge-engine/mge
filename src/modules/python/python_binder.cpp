// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_binder.hpp"

#include "mge/core/trace.hpp"

#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"
#include "mge/reflection/type_identifier.hpp"

#include "python_context.hpp"
#include "python_module.hpp"

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {
    python_binder::~python_binder() {}

    void python_binder::before(const mge::reflection::module_details& details)
    {}
    void python_binder::on(const mge::reflection::module_details& details)
    {
        if (details.is_root()) {
            return;
        }
        auto mod = std::make_shared<python_module>(
            m_context,
            std::const_pointer_cast<mge::reflection::module_details>(
                details.shared_from_this()));
        m_context.add_module(mod);
    }
    void python_binder::after(const mge::reflection::module_details& details) {}

    void python_binder::before(const mge::reflection::type_details& details) {}
    void python_binder::on(const mge::reflection::type_details& details)
    {
        MGE_DEBUG_TRACE(PYTHON,
                        "Binding type: {}",
                        std::string(details.name).c_str());
        if (details.is_enum) {
            bind_enum(details);
        } // else if (details.is_class) {
          // bind_class(details);
        //}
    }

    void python_binder::after(const mge::reflection::type_details& details) {}

    void python_binder::before(const mge::reflection::function_details& details)
    {}
    void python_binder::on(const mge::reflection::function_details& details) {}
    void python_binder::after(const mge::reflection::function_details& details)
    {}

    void python_binder::bind_enum(const mge::reflection::type_details& details)
    {
        MGE_DEBUG_TRACE(PYTHON, "Binding enum: {}", details.name);
    }

} // namespace mge::python