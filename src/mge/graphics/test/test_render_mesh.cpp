// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/render_mesh.hpp"
#include "test/googletest.hpp"

TEST(render_mesh, default_is_invalid)
{
    mge::render_mesh mesh;

    EXPECT_FALSE(mesh);
}

TEST(render_mesh, draw_adds_command)
{
    mge::vertex_buffer_handle vertices{1, 0, 2};
    mge::index_buffer_handle  indices{1, 0, 3};
    mge::program_handle       program{1, 0, 4};
    mge::render_mesh          mesh(vertices, indices);
    mge::command_buffer       command_buffer;

    mesh.draw(command_buffer, program, 12, 6);

    size_t count = 0;
    command_buffer.for_each([&](const mge::program_handle& /*p*/,
                                const mge::vertex_buffer_handle& /*v*/,
                                const mge::index_buffer_handle& /*i*/,
                                const mge::pipeline_state& /*state*/,
                                mge::uniform_block* /*ub*/,
                                const mge::texture_binding_list& /*textures*/,
                                uint32_t index_count,
                                uint32_t index_offset,
                                const mge::rectangle& /*scissor*/) {
        EXPECT_EQ(index_count, 12u);
        EXPECT_EQ(index_offset, 6u);
        ++count;
    });
    EXPECT_EQ(count, 1u);
}
