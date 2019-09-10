// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/index_buffer.hpp"
#include <array>
namespace opengl
{
    class test_index_buffer : public opengltest_with_window
    {};

    TEST_F(test_index_buffer, create)
    {
        std::array<int, 3> a = { 1, 2, 3};
        auto buffer = window->render_context().create_index_buffer(a);
    }


    TEST_F(test_index_buffer, buffer_map_unmap)
    {
        auto buffer = window->render_context()
                .create_index_buffer(mge::data_type::INT32,
                                     mge::usage::DEFAULT,
                                     1024,
                                     nullptr);
        void *bufferdata = buffer->map();
        memset(bufferdata, 'A', 1024);
        buffer->unmap();
    }

}
