// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx11 {
    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(render_context&      context,
                      const vertex_layout& layout,
                      size_t               data_size);
        ~vertex_buffer();

        ID3D11Buffer* buffer() const
        {
            return m_buffer.get();
        }

        void on_set_data(void* data, size_t data_size) override;

    private:
        void create_buffer(void* data);

        mge::com_unique_ptr<ID3D11Buffer> m_buffer;
        void*                             m_mapped_memory;
    };

} // namespace mge::dx11
