// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/draw_command.hpp"

namespace mge {
    draw_command::draw_command()
        : m_topology(mge::topology::TRIANGLES)
    {}

    draw_command::draw_command(mge::program*  program,
                               vertex_buffer* vertices,
                               index_buffer*  indices,
                               mge::topology  t)
        : m_program(program)
        , m_vertices(vertices)
        , m_indices(indices)
        , m_topology(t)
    {}

    draw_command::~draw_command() {}
} // namespace mge