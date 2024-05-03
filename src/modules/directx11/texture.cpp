#include "texture.hpp"
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/zero_memory.hpp"
#include "render_context.hpp"


namespace mge::dx11 {

    texture::texture(render_context& context, mge::texture_type type)
        : mge::texture(context, type)
    {}

    texture::~texture() {}

    DXGI_FORMAT texture::texture_format(const mge::image_format& format) const
    {
        switch (format.format()) {
        case mge::image_format::data_format::RGB:
            switch (format.type()) {
            case mge::data_type::FLOAT:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        case mge::image_format::data_format::RGBA:
            switch (format.type()) {
            case mge::data_type::UINT8:
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            case mge::data_type::UINT16:
                return DXGI_FORMAT_R16G16B16A16_UNORM;
            case mge::data_type::FLOAT:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        default:
            MGE_THROW(illegal_argument)
                << "Unsupported image format (format): " << format;
        }
    }

    void texture::set_data(const mge::image_format& format,
                           const mge::extent&       extent,
                           const void*              data,
                           size_t                   size)
    {
        D3D11_TEXTURE2D_DESC desc;
        mge::zero_memory(desc);
        desc.Width = extent.width;
        desc.Height = extent.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = texture_format(format);
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresource_data;
        mge::zero_memory(subresource_data);
        subresource_data.pSysMem = data;
        subresource_data.SysMemPitch =
            static_cast<UINT>(extent.width * format.binary_size());
        subresource_data.SysMemSlicePitch = 0;

        ID3D11Texture2D* texture = nullptr;

        auto hr =
            dx11_context(context()).device()->CreateTexture2D(&desc,
                                                              &subresource_data,
                                                              &texture);
        CHECK_HRESULT(hr, ID3D11Device, CreateTexture2D);
        m_texture.reset(texture);
    }
} // namespace mge::dx11