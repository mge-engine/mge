#include "mge/script/signature.hpp"
#include "test/googletest.hpp"
#include <string>
#include <vector>

namespace mge::script {
    TEST(signature, from_template_args)
    {
        signature s = signature::create<int>();
    }
} // namespace mge::script