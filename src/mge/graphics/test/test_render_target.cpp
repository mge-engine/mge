#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/render_target.hpp"
#include "mge/core/test/mock_async_executor.hpp"

using namespace testing;

namespace mge {
    class test_render_target : public render_target
    {
    public:
        test_render_target(async_executor *ex)
            :render_target(ex)
        {}
    };
}

TEST(render_target, dispatches_await)
{
    mge::mock_async_executor executor;
    mge::test_render_target target(&executor);
    EXPECT_CALL(executor, await(_));
    target.await([] {});
}
