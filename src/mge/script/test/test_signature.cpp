#include "mge/core/gist.hpp"
#include "mge/script/signature.hpp"
#include "test/googletest.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace mge::script {
    TEST(signature, from_template_args)
    {
        signature s = signature::create<int>();
    }
} // namespace mge::script

namespace mge::foo {
    TEST(signature, gist)
    {
        mge::script::signature s = mge::script::signature::create<int, float>();
        std::stringstream      ss;
        ss << gist(s);
        EXPECT_EQ(ss.str(), "(int, float)");
    }
} // namespace mge::foo
