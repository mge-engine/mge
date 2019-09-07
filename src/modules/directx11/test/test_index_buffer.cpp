// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/index_buffer.hpp"
#include <array>
namespace dx11
{
    class test_index_buffer : public dx11test_with_window
    {};

    TEST_F(test_index_buffer, create)
    {
        std::array<int, 3> a = { 1, 2, 3};
        auto buffer = window->render_context().create_index_buffer(a);
    }


    TEST_F(test_index_buffer, normal_buffer_map_unmap)
    {
        auto buffer = window->render_context()
                .create_index_buffer(mge::data_type::INT32,
                                     mge::usage::DEFAULT,
                                     mge::buffer_access::WRITE,
                                     mge::buffer_access::READ,
                                     1024,
                                     nullptr);
        void *bufferdata = buffer->map();
        memset(bufferdata, 'A', 1024);
        buffer->unmap();
    }

// "staging" buffer may be really mapped for reading, but currently
// not implemented
#if 0
    TEST_F(test_index_buffer, staging_read_buffer_map_unmap)
    {
        auto buffer = window->render_context()
                .create_index_buffer(mge::data_type::INT32,
                                     mge::usage::STAGING,
                                     mge::buffer_access::READ,
                                     mge::buffer_access::WRITE,
                                     1024,
                                     nullptr);
        void *bufferdata = buffer->map();
        EXPECT_TRUE(bufferdata != nullptr);
        buffer->unmap();
    }
#endif

}
