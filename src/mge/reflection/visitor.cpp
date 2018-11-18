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
        visitor::start(const type &t)
        {}

        void
        visitor::finish(const type& t)
        {}

        void
        visitor::enum_value(const std::string &name, int64_t value)
        {}
    }
}
