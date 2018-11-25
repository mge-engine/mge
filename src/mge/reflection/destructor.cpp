#include "mge/reflection/destructor.hpp"

namespace mge {
    namespace reflection {
        void
        destructor::invoke(void *ptr)
        {
            if(m_function) {
                m_function(ptr);
            }
        }
    }
}
