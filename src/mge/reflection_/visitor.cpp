// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/visitor.hpp"

namespace mge {
    namespace reflection {
        void
        visitor::start(const module &m)
        {}

        void
        visitor::finish(const module &m)
        {}

        void
        visitor::start(const type_definition &t)
        {}

        void
        visitor::finish(const type_definition& t)
        {}

        void
        visitor::enum_value(const std::string &name, int64_t value)
        {}
    }
}
