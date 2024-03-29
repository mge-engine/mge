// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx11 {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context& context,
                     mge::data_type  type,
                     size_t          data_size,
                     void*           data);
        virtual ~index_buffer();

        ID3D11Buffer* buffer() const { return m_buffer.get(); }

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        void create_buffer(void* data);

        mge::com_unique_ptr<ID3D11Buffer> m_buffer;
        DXGI_FORMAT                       m_format;
        void*                             m_mapped_memory;
    };

} // namespace mge::dx11
