// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/visitor.hpp"

namespace mge::script {
    void visitor::start(const module_details&) {}

    void visitor::finish(const module_details&) {}

    void visitor::start(const type_details&) {}

    void visitor::finish(const type_details&) {}

    void visitor::enum_value(const std::string& name, int64_t value) {}
} // namespace mge::script