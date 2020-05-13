// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/draw_command.hpp"

namespace mge {

    draw_command::draw_command() : m_topology(mge::topology::TRIANGLES) {}

    draw_command::draw_command(const pipeline_ref &     p,
                               const vertex_buffer_ref &v,
                               const index_buffer_ref &i, mge::topology mode)
        : m_pipeline(p), m_vertices(v), m_indices(i), m_topology(mode)
    {}

    draw_command::draw_command(const draw_command &c)
        : m_pipeline(c.m_pipeline), m_vertices(c.m_vertices),
          m_indices(c.m_indices), m_topology(c.m_topology)
    {}

    draw_command::draw_command(draw_command &&c)
        : m_pipeline(std::move(c.m_pipeline)),
          m_vertices(std::move(c.m_vertices)),
          m_indices(std::move(c.m_indices)), m_topology(std::move(c.m_topology))
    {}

    draw_command &draw_command::operator=(const draw_command &c)
    {
        m_pipeline = c.m_pipeline;
        m_vertices = c.m_vertices;
        m_indices  = c.m_indices;
        m_topology = c.m_topology;
        return *this;
    }

    draw_command &draw_command::operator=(draw_command &&c)
    {
        m_pipeline = std::move(c.m_pipeline);
        m_vertices = std::move(c.m_vertices);
        m_indices  = std::move(c.m_indices);
        m_topology = std::move(c.m_topology);
        return *this;
    }

    void draw_command::set_pipeline(const pipeline_ref &pipeline)
    {
        m_pipeline = pipeline;
    }

    void draw_command::set_vertices(const vertex_buffer_ref &buffer)
    {
        m_vertices = buffer;
    }

    void draw_command::set_indices(const index_buffer_ref &buffer)
    {
        m_indices = buffer;
    }

    void draw_command::set_topology(mge::topology mode) { m_topology = mode; }

    void draw_command::bind_uniform_buffer(uint32_t buffer, void *data)
    {
        if (m_uniform_buffer_data.size() <= buffer) {
            m_uniform_buffer_data.resize(buffer + 1);
        }
        m_uniform_buffer_data[buffer] = data;
    }

    void draw_command::bind_uniform(uint32_t buffer, uint32_t uniform,
                                    void *data)
    {
        if (data) {
            auto it = m_uniform_data.begin();
            while (it != m_uniform_data.end()) {
                if (it->uniform_buffer == buffer && it->uniform == uniform) {
                    it->data = data;
                    return;
                }
            }
            uniform_data d{buffer, uniform, data};
            m_uniform_data.push_back(d);
        } else {
            auto it = m_uniform_data.begin();
            while (it != m_uniform_data.end()) {
                if (it->uniform_buffer == buffer && it->uniform == uniform) {
                    m_uniform_data.erase(it);
                    return;
                }
                ++it;
            }
        }
    }

    size_t draw_command::uniform_buffer_count() const
    {
        return m_uniform_buffer_data.size();
    }

    void *draw_command::uniform_buffer(size_t i) const
    {
        return m_uniform_buffer_data.at(i);
    }

} // namespace mge
