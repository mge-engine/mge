#include "mge/graphics/attribute_semantic.hpp"
#include "test/googletest.hpp"
#include "test/test_format.hpp"

TEST(attribute_semantic, output)
{
    test_format(mge::attribute_semantic::ANY, "ANY");
}