#include "test/googlemock.hpp"
#include "test/test_exception.hpp"

#include "mge/core/test/mock_async_executor.hpp"
#include "mge/graphics/test/mock_render_context.hpp"
#include "mge/graphics/test/mock_pipeline.hpp"
#include "mge/graphics/test/mock_shader.hpp"

using namespace testing;
namespace mge {

TEST(pipeline, needs_link_after_create)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());

    mock_pipeline p(*context);
    EXPECT_TRUE(p.needs_link());
}

TEST(pipeline, set_null_shader_ref_throws)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());
    
    mock_pipeline p(*context);
    shader_ref sh;
    EXPECT_THROW_WITH_MESSAGE(p.set_shader(sh), mge::illegal_argument, "Argument 'shader' must not be null");
}

TEST(pipeline, set_compute_shader_throws)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());
    auto shader = std::make_shared<mock_shader>(*context, shader_type::COMPUTE);

    mock_pipeline p(*context);
    EXPECT_THROW_WITH_MESSAGE(p.set_shader(shader), mge::illegal_argument, "Shader type must not be shader_type::COMPUTE");
}

TEST(pipeline, set_undefined_shader_throws)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());
    auto shader = std::make_shared<mock_shader>(*context, shader_type::VERTEX);

    mock_pipeline p(*context);
    EXPECT_THROW_WITH_MESSAGE(p.set_shader(shader), mge::illegal_argument, "Shader must be defined before attaching to pipeline");
}

TEST(pipeline, set_shader_succeeds)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());
    auto shader = std::make_shared<mock_shader>(*context, shader_type::VERTEX);
    EXPECT_CALL(*shader, on_compile(_)).Times(1);
    shader->compile("foobar");
    mock_pipeline p(*context);
    EXPECT_CALL(p, on_set_shader(_)).Times(1);
    p.set_shader(shader);
}

TEST(pipeline, link_succeeds)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());
    mock_pipeline p(*context);
    EXPECT_CALL(p, on_link()).Times(1);
    p.link();
    EXPECT_FALSE(p.needs_link());
}

TEST(pipeline, link_fails)
{
    auto executor = std::make_shared<mock_async_executor>();
    auto context = std::make_shared<mock_render_context>(executor.get());
    mock_pipeline p(*context);
    EXPECT_CALL(p, on_link()).Times(1).WillOnce(Throw(mge::illegal_argument()));
    try { p.link(); } catch(const mge::illegal_argument&) {}
    EXPECT_TRUE(p.needs_link());
}

}