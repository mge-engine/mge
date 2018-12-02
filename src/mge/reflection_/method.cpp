#include "mge/reflection/method.hpp"

namespace mge {
    namespace reflection {
        method::method(const char *name,
                       const method::function& f)
            :m_name(name)
            ,m_function(f)
        {}
    }
}
