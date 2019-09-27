#include "dx11_test.hpp"
#include "test/test_exception.hpp"
#include "mge/graphics/window.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/pipeline.hpp"
#include "mge/graphics/render_context.hpp"

namespace dx11 {

class test_pipeline : public dx11test_with_window
{};

TEST_F(test_pipeline, create)
{
    auto pipeline = window->render_context().create_pipeline();
}

TEST_F(test_pipeline, link_empty)
{
    auto pipeline = window->render_context().create_pipeline();
    EXPECT_THROW_WITH_MESSAGE(pipeline->link(),
                              mge::exception,
                              "pipeline link failed");
}

}
