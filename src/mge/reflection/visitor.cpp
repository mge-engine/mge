#include "mge/reflection/visitor.hpp"

namespace mge {
    namespace reflection {
        void
        visitor::start(module &m)
        {}

        void
        visitor::finish(module &m)
        {}

        void
        visitor::start(type &t)
        {}

        void
        visitor::finish(type& t)
        {}

        void
        visitor::enum_value(const std::string &name, int64_t value)
        {}
    }
}
