// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "win32/com_unique_ptr.hpp"

namespace mge::dx11 {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context& context, mge::data_type type, size_t data_size, void* data);
        virtual ~index_buffer();

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        void create_buffer(void* data);

        mge::com_unique_ptr<ID3D11Buffer> m_buffer;
        DXGI_FORMAT                       m_format;
    };

} // namespace mge::dx11
