#include "mge/script/bind.hpp"
#include "mge/script/module.hpp"
#include "mge/script/variable.hpp"
#include "test/googletest.hpp"

namespace mge {
    class test_variable : public ::testing::Test
    {
    public:
        void SetUp() override { mge::script::bind_all(); }
    };

    static int ivar;

    TEST_F(test_variable, integer_var)
    {
        using namespace mge::script;
        module("mge")(variable("ivar", ivar));
    }
} // namespace mge