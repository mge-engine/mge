// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "win32/com_unique_ptr.hpp"

namespace dx11 {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(mge::render_context &context, mge::data_type type,
                     mge::usage usage, size_t element_count,
                     void *initial_data);
        virtual ~index_buffer();
        size_t size() const noexcept override;

    protected:
        void *on_map() override;
        void  on_unmap() override;

    private:
        void create_buffer(void *data);
        bool for_read_mapping() const;

        mge::com_unique_ptr<ID3D11Buffer> m_buffer;
        DXGI_FORMAT                       m_format;
        size_t                            m_size;
    };

} // namespace dx11
