// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "opengl_test.hpp"

namespace opengl {
    class test_vertex_buffer : public opengltest_with_window
    {};

    TEST_F(test_vertex_buffer, create)
    {
        mge::vertex_layout vl;
        vl.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));

        auto buffer = window->render_context().create_vertex_buffer(
            vl, mge::usage::DEFAULT, 3, nullptr);
    }

    TEST_F(test_vertex_buffer, map_unmap)
    {
        mge::vertex_layout vl;
        vl.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));

        auto buffer = window->render_context().create_vertex_buffer(
            vl, mge::usage::DEFAULT, 1024, nullptr);
        void *bufferdata = buffer->map();
        memset(bufferdata, 'A', vl.binary_size() * 1024);
        buffer->unmap();
    }

} // namespace opengl
