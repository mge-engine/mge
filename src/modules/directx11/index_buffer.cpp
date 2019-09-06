#include "index_buffer.hpp"
#include "render_context.hpp"
#include "error.hpp"
#include "common.hpp"
#include "mge/core/numeric_cast.hpp"
#include "mge/core/memory.hpp"

namespace dx11 {

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
        if(initial_data) {
            create_buffer(initial_data);
        }
        if(cpu_access == mge::buffer_access::READ
           || cpu_access == mge::buffer_access::READ_WRITE) {
            MGE_THROW(dx11::error) << "Only write to index buffer supported";
        }
    }

    bool
    index_buffer::for_read_mapping() const
    {
        return change_policy() == mge::buffer_change_policy::STAGING
               && (cpu_access() == mge::buffer_access::READ
                   || cpu_access() == mge::buffer_access::READ_WRITE);
    }

    void
    index_buffer::create_buffer(void *data)
    {
        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage     = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        buffer_desc.ByteWidth = mge::numeric_cast<UINT>(size());
        buffer_desc.CPUAccessFlags = 0; //dx11_access(cpu_access());
        buffer_desc.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA init_data = {};
        init_data.pSysMem = data;
        ID3D11Buffer *buffer = nullptr;
        auto hr = dx11_device(context())->CreateBuffer(&buffer_desc,
                                                       &init_data,
                                                       &buffer);
        CHECK_HRESULT(hr, ID3D11Device, CreateBuffer);
        m_buffer.reset(buffer);
    }

    index_buffer::~index_buffer()
    {
        if(m_mapped_memory) {
            mge::free(m_mapped_memory);
        }
    }

    size_t
    index_buffer::size() const noexcept
    {
        return m_size;
    }

    void *
    index_buffer::on_map()
    {
        m_buffer.reset();
        void *mapped_memory = mge::malloc(size());
        return mapped_memory;
    }

    void
    index_buffer::on_unmap()
    {
        create_buffer(m_mapped_memory);
        mge::free(m_mapped_memory);
    }
}
