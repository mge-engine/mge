// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_binder.hpp"

#include "mge/reflection/function_details.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"
#include "mge/reflection/type_identifier.hpp"

namespace mge::python {
    python_binder::~python_binder() {}

    void python_binder::before(const mge::reflection::module_details& details)
    {}
    void python_binder::on(const mge::reflection::module_details& details) {}
    void python_binder::after(const mge::reflection::module_details& details) {}

    void python_binder::before(const mge::reflection::type_details& details) {}
    void python_binder::on(const mge::reflection::type_details& details) {}
    void python_binder::after(const mge::reflection::type_details& details) {}

    void python_binder::before(const mge::reflection::function_details& details)
    {}
    void python_binder::on(const mge::reflection::function_details& details) {}
    void python_binder::after(const mge::reflection::function_details& details)
    {}

} // namespace mge::python