// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/draw_command.hpp"

namespace mge {

    draw_command::draw_command()
    {}

    draw_command::draw_command(const pipeline_ref &p,
                     const vertex_buffer_ref &v,
                     const index_buffer_ref &i)
        : m_pipeline(p)
        , m_vertices(v)
        , m_indices(i)
    {}

    draw_command::draw_command(const draw_command &c)
        : m_pipeline(c.m_pipeline)
        , m_vertices(c.m_vertices)
        , m_indices(c.m_indices)
    {}

    draw_command::draw_command(draw_command &&c)
        : m_pipeline(std::move(c.m_pipeline))
        , m_vertices(std::move(c.m_vertices))
        , m_indices(std::move(c.m_indices))
    {}

    draw_command&
    draw_command::operator =(const draw_command& c)
    {
        m_pipeline = c.m_pipeline;
        m_vertices = c.m_vertices;
        m_indices = c.m_indices;
        return *this;
    }

    draw_command&
    draw_command::operator =(draw_command&& c)
    {
        m_pipeline = std::move(c.m_pipeline);
        m_vertices = std::move(c.m_vertices);
        m_indices = std::move(c.m_indices);
        return *this;
    }


    void
    draw_command::set_pipeline(const pipeline_ref& pipeline)
    {
        m_pipeline = pipeline;
    }

    void
    draw_command::set_vertices(const vertex_buffer_ref &buffer)
    {
        m_vertices = buffer;
    }

    void
    draw_command::set_indices(const index_buffer_ref& buffer)
    {
        m_indices = buffer;
    }

    

}
