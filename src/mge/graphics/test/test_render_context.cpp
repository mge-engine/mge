#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/core/test/mock_async_executor.hpp"

using namespace testing;

namespace mge {
    class test_render_context : public render_context
    {
    public:
        test_render_context(async_executor *ex)
            :render_context(ex)
        {}
    };
}

TEST(render_context, dispatches_await)
{
    mge::mock_async_executor executor;
    mge::test_render_context context(&executor);
    EXPECT_CALL(executor, await(_));
    context.await([] {});
}
