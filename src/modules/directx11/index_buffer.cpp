#include "index_buffer.hpp"
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/numeric_cast.hpp"

namespace dx11 {
    static DXGI_FORMAT dx11_format(mge::data_type type)
    {
        switch(type) {
        case mge::data_type::INT16:
        case mge::data_type::UINT16:
            return DXGI_FORMAT_R16_UINT;
        case mge::data_type::INT32:
        case mge::data_type::UINT32:
            return DXGI_FORMAT_R32_UINT;
        default:
            MGE_THROW(dx11::error) << "Unsupported data type " << type;
        }
    }

    static D3D11_USAGE dx11_usage(mge::buffer_change_policy policy)
    {
        switch(policy) {
        case mge::buffer_change_policy::DEFAULT:
            return D3D11_USAGE_DEFAULT;
        case mge::buffer_change_policy::DYNAMIC:
            return D3D11_USAGE_DYNAMIC;
        case mge::buffer_change_policy::STAGING:
            return D3D11_USAGE_STAGING;
        case mge::buffer_change_policy::IMMUTABLE:
            return D3D11_USAGE_IMMUTABLE;
        }
        return D3D11_USAGE_DEFAULT;
    }

    static UINT dx11_cpu_access(mge::buffer_access cpu_access)
    {
        switch(cpu_access) {
        case mge::buffer_access::NONE:
            return 0;
        case mge::buffer_access::READ:
            return D3D11_CPU_ACCESS_READ;
        case mge::buffer_access::WRITE:
            return D3D11_CPU_ACCESS_WRITE;
        case mge::buffer_access::READ_WRITE:
            return D3D11_CPU_ACCESS_READ|D3D11_CPU_ACCESS_WRITE;
        }
        return 0;
    }

    index_buffer::index_buffer(mge::render_context& context,
                               mge::data_type type,
                               mge::buffer_change_policy change_policy,
                               mge::buffer_access cpu_access,
                               mge::buffer_access gpu_access,
                               size_t element_count,
                               void  *initial_data)
        :mge::index_buffer(context,
                           type,
                           change_policy,
                           cpu_access,
                           gpu_access,
                           element_count,
                           initial_data)
        ,m_format(dx11_format(type))
        ,m_size(element_count * mge::data_type_size(type))
    {
        m_format = dx11_format(type);
        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage     = dx11_usage(change_policy);
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.ByteWidth = mge::numeric_cast<UINT>(size());
        buffer_desc.CPUAccessFlags = dx11_cpu_access(cpu_access);
        D3D11_SUBRESOURCE_DATA init_data = {};
        init_data.pSysMem = initial_data;
        ID3D11Buffer *buffer = nullptr;
        auto hr = dx11_device(context)->CreateBuffer(&buffer_desc,
                                                     &init_data,
                                                     &buffer);
        CHECK_HRESULT(hr, ID3D11Device, CreateBuffer);
        m_buffer.reset(buffer);
    }

    index_buffer::~index_buffer()
    {}

    size_t
    index_buffer::size() const noexcept
    {
        return m_size;
    }
}
