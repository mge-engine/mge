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
                              "Cannot link empty pipeline");
}

TEST_F(test_pipeline, successful_simple_link)
{
    const char *vertex_shader_hlsl =
        "float4 main(float4 vertexPosition : vertexPosition) : SV_POSITION\n"
        "{\n"
        "  return vertexPosition;\n"
        "}";

    const char *fragment_shader_hlsl =
        "float4 main() : SV_TARGET\n"
        "{\n"
        "    return float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}";

    auto vs = window->render_context().create_shader(mge::shader_type::VERTEX);
    vs->compile(vertex_shader_hlsl);
    auto fs = window->render_context().create_shader(mge::shader_type::FRAGMENT);
    fs->compile(fragment_shader_hlsl);
    auto p = window->render_context().create_pipeline();
    p->set_shader(vs);
    p->set_shader(fs);
    p->link();
    EXPECT_EQ(1u, p->attributes().size());
    EXPECT_EQ(std::string_view("vertexPosition"), p->attributes()[0].name);
    EXPECT_EQ(mge::data_type::FLOAT_VEC4, p->attributes()[0].type);
    EXPECT_EQ(1, p->attributes()[0].size);
}
}
