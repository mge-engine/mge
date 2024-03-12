
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "dx12_fwd.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx12 {
    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context& context,
                     mge::data_type  type,
                     size_t          data_size,
                     void*           data);
        virtual ~index_buffer();

        const D3D12_INDEX_BUFFER_VIEW& view() const { return m_buffer_view; }

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        void create_buffer(void* data);

        mge::com_unique_ptr<ID3D12Resource> m_buffer;
        void*                               m_mapped_memory;
        D3D12_INDEX_BUFFER_VIEW             m_buffer_view;
    };

    inline index_buffer& dx12_index_buffer(mge::index_buffer& ib)
    {
        return static_cast<index_buffer&>(ib);
    }

    inline const index_buffer& dx12_index_buffer(const mge::index_buffer& ib)
    {
        return static_cast<const index_buffer&>(ib);
    }
} // namespace mge::dx12
