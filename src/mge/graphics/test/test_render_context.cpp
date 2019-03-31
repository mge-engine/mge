#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/core/test/mock_async_executor.hpp"
#include "mock_render_context.hpp"
#include <memory>
using namespace testing;

namespace mge {
    class test_render_context : public mock_render_context
    {
    public:
        test_render_context(async_executor *ex)
            :mock_render_context(ex)
        {}
    };
}

TEST(render_context, dispatches_await)
{
    mge::mock_async_executor executor;
    auto context = std::make_shared<mge::test_render_context>(&executor);
    EXPECT_CALL(executor, await(_));
    context->await([] {});
}
