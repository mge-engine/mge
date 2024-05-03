#pragma once
#include "dx11.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx11 {
    class render_context;

    class texture : public mge::texture
    {
    public:
        texture(render_context& context, mge::texture_type type);
        ~texture() override;

        void set_data(const mge::image_format& format,
                      const mge::extent&       extent,
                      const void*              data,
                      size_t                   size) override;

    private:
        DXGI_FORMAT texture_format(const mge::image_format& format) const;
        mge::com_unique_ptr<ID3D11Texture2D> m_texture;
    };
} // namespace mge::dx11
