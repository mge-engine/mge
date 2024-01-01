#include "shader.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    shader::shader(render_context& context, shader_type type)
        : mge::shader(context, type)
    {}

    shader::~shader() {}

    void shader::on_compile(std::string_view code) {}

    void shader::on_set_code(const mge::buffer& code) {}

    void
    shader::reflect(mge::program::attribute_list&      attributes,
                    mge::program::uniform_list&        uniforms,
                    mge::program::uniform_buffer_list& uniform_buffers) const
    {}

} // namespace mge::dx12