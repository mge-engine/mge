#include "mge/script/proxy.hpp"
#include "test/googletest.hpp"

namespace mge::script {

    class proxytest_interface
    {
    public:
        virtual ~proxytest_interface() = default;

        virtual int foo(int x) = 0;
    };

    class proxytest : public proxy<proxytest_interface>
    {
    public:
        int foo(int x) override { return 0; }
    };

} // namespace mge::script