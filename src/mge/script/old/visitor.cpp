// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/visitor.hpp"
#include "mge/script/module.hpp"

namespace mge::script {

    void visitor::visit(const module& m) { m.apply(*this); }

    void visitor::begin(const module_details&) {}
    void visitor::end(const module_details&) {}

    void visitor::begin(const type_details&) {}
    void visitor::end(const type_details&) {}

    void visitor::visit(const function_details&) {}
    void visitor::visit(const variable_details&) {}

    void visitor::enum_value(const std::string& name, const any_integer& value)
    {}

    void visitor::field(const field_details& f) {}
    void visitor::constructor(const signature&        signature,
                              const context_function& cf)
    {}
} // namespace mge::script