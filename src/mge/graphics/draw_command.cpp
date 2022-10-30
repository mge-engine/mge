// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/draw_command.hpp"

namespace mge {
    draw_command::draw_command()
        : m_topology(mge::topology::TRIANGLES)
    {}

    draw_command::draw_command(const pipeline_ref&      pipeline,
                               const vertex_buffer_ref& vertices,
                               const index_buffer_ref&  indices,
                               mge::topology            t)
        : m_pipeline(pipeline)
        , m_vertices(vertices)
        , m_indices(indices)
        , m_topology(t)
    {}

    draw_command::~draw_command() {}
} // namespace mge