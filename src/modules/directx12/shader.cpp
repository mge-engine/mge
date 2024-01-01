#include "shader.hpp"
#include "dx12.hpp"
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

    void shader::on_compile(std::string_view code)
    {
        ID3DBlob* compiled_code = nullptr;
        ID3DBlob* errors = nullptr;
        auto      file_name = get_property("file_name", "shader");
        auto      main_function = get_property("main_function", "main");
        auto      shader_profile = profile();

        auto rc = D3DCompile(code.data(),
                             code.size(),
                             file_name.c_str(),
                             nullptr,
                             nullptr,
                             main_function.c_str(),
                             shader_profile.c_str(),
                             0,
                             0,
                             &compiled_code,
                             &errors);
        if (FAILED(rc)) {
            std::string errormessage(
                static_cast<const char*>(errors->GetBufferPointer()),
                static_cast<const char*>(errors->GetBufferPointer()) +
                    errors->GetBufferSize());
            errors->Release();
            throw dx12::error().set_info_from_hresult(rc,
                                                      __FILE__,
                                                      __LINE__,
                                                      "",
                                                      "D3DCompile")
                << "(" << errormessage << ")";
        } else {
            m_code.reset(compiled_code);
        }
    }

    std::string shader::profile() const
    {
        switch (type()) {
        case mge::shader_type::FRAGMENT:
            return get_property("profile", "ps_5_0");
        case mge::shader_type::VERTEX:
            return get_property("profile", "vs_5_0");
        case mge::shader_type::COMPUTE:
            return get_property("profile", "cs_5_0");
        default:
            MGE_THROW(dx12::error) << "Unsupported shader type: " << type();
        }
    }

    void shader::on_set_code(const mge::buffer& code) {}

    void
    shader::reflect(mge::program::attribute_list&      attributes,
                    mge::program::uniform_list&        uniforms,
                    mge::program::uniform_buffer_list& uniform_buffers) const
    {}

} // namespace mge::dx12