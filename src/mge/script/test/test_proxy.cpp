#include "mge/script/proxy.hpp"
#include "test/googletest.hpp"

namespace mge::script {

    class ptest1
    {
        virtual ~ptest1() = default;
        virtual int foo() = 0;
    };

} // namespace mge::script