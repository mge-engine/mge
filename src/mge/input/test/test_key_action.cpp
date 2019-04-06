#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"
#include "mge/input/key_action.hpp"

TEST(key_action, print)
{
    mge::test_stream_output(mge::key_action::PRESS, "PRESS");
    mge::test_stream_output(mge::key_action::RELEASE, "RELEASE");
    mge::test_stream_output(mge::key_action::REPEAT, "REPEAT");
}
