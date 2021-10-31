// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/visitor.hpp"
#include "mge/script/module.hpp"

namespace mge::script {

    void visitor::module_begin(const module& m) {}

    void visitor::module_end(const module& m) {}

    void visitor::type_begin(const type& t) {}

    void visitor::type_end(const type& t) {}
}