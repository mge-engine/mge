// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "dx12_fwd.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx12 {
    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(render_context&      context,
                      const vertex_layout& layout,
                      size_t               data_size,
                      void*                initial_data);
        ~vertex_buffer();

        ID3D12Resource* buffer() const { return m_buffer.get(); }

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        void create_buffer(void* data);

        mge::com_unique_ptr<ID3D12Resource> m_buffer;
        void*                               m_mapped_memory;
    };

} // namespace mge::dx12
