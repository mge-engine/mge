#include "pipeline.hpp"
namespace dx11 {
    pipeline::pipeline(mge::render_context& context)
        : mge::pipeline(context)
    {}

    pipeline::~pipeline()
    {}

    void
    pipeline::on_link()
    {}

    void
    pipeline::on_set_shader(const mge::shader_ref& shader)
    {}
}