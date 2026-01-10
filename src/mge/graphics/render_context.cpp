// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/frame_buffer.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "mge/graphics/vertex_buffer.hpp"

namespace mge {

    class render_context_registry
    {
    public:
        render_context_registry()
            : m_mutex("render_context_registry")
        {
            m_contexts.reserve(8);
        }

        uint16_t register_context(render_context* rc)
        {
            std::lock_guard<mge::mutex> lock(m_mutex);
            if (m_contexts.size() >= std::numeric_limits<uint16_t>::max()) {
                gc();
            }
            if (m_contexts.size() >= std::numeric_limits<uint16_t>::max()) {
                mge::crash("Too many render contexts registered");
            }
            m_contexts.push_back(rc);
            return static_cast<uint16_t>(m_contexts.size() - 1);
        }

        void unregister_context(uint32_t index, render_context* rc) noexcept
        {
            std::lock_guard<mge::mutex> lock(m_mutex);
            auto size = static_cast<uint32_t>(m_contexts.size());
            if (index < size) {
                if (m_contexts[index] != rc) {
                    mge::crash("Inconsistent render context unregistering");
                }
                if (index == size - 1) {
                    m_contexts.resize(size - 1);
                    gc();
                } else {
                    m_contexts[index] = nullptr;
                }
            } else {
                mge::crash("Invalid render context index unregistering");
            }
        }

        render_context* get(uint16_t index) noexcept
        {
            if (index < m_contexts.size()) {
                return m_contexts[index];
            }
            return nullptr;
        }

        void gc() noexcept
        {
            size_t sz = m_contexts.size();
            while (sz > 0 && m_contexts[sz - 1] == nullptr) {
                --sz;
            }
            m_contexts.resize(sz);
        }

        static mge::singleton<render_context_registry> instance;

    private:
        std::vector<render_context*> m_contexts;
        mge::mutex                   m_mutex;
    };

    mge::singleton<render_context_registry> render_context_registry::instance;

    render_context::render_context(mge::render_system& rs,
                                   const mge::extent&  ext)
        : m_render_system(rs)
        , m_extent(ext)
        , m_prepare_frame_resource(m_prepare_frame_memory.data(),
                                   m_prepare_frame_memory.size())
        , m_prepare_frame_actions(&m_prepare_frame_resource)
    {
        m_index = render_context_registry::instance->register_context(this);
    }

    render_context::~render_context()
    {
        render_context_registry::instance->unregister_context(m_index, this);
    }

    void render_context::frame()
    {
        if (!m_prepare_frame_actions.empty()) {
            try {
                for (const auto& action : m_prepare_frame_actions) {
                    action();
                }
            } catch (...) {
                m_prepare_frame_actions.clear();
                m_prepare_frame_resource.release();
                throw;
            }
            m_prepare_frame_actions.clear();
            m_prepare_frame_resource.release();
        }
        bool rendered = false;
        if (m_passes.size() > 0) {
            for (const auto& p : m_passes)
                if (p.active()) {
                    render(p);
                    rendered = true;
                }
        }
        if (rendered) {
            on_frame_present();
        }
    }

    void render_context::render(const mge::pass& p)
    {
        // to be implemented by subclasses
    }

    render_context* render_context::get(uint16_t index)
    {
        return render_context_registry::instance->get(index);
    }

    void render_context::on_destroy_index_buffer(index_buffer* ib)
    {
        delete ib;
    }

    void render_context::on_destroy_vertex_buffer(vertex_buffer* vb)
    {
        delete vb;
    }

    void render_context::on_destroy_shader(shader* s)
    {
        delete s;
    }

    void render_context::on_destroy_program(program* p)
    {
        delete p;
    }

    frame_buffer* render_context::on_create_frame_buffer()
    {
        MGE_THROW_NOT_IMPLEMENTED << "frame buffer creation not implemented";
        return nullptr;
    }

    void render_context::on_destroy_frame_buffer(frame_buffer* fb)
    {
        delete fb;
    }

    shader_handle render_context::create_shader(shader_type t)
    {
        std::unique_ptr<shader> ptr{on_create_shader(t)};
        if (ptr) {
            shader_handle handle{index(),
                                 0,
                                 static_cast<uint32_t>(m_shaders.size())};
            m_shaders.emplace_back(ptr.release());
            return handle;
        } else {
            return shader_handle();
        }
    }

    program_handle render_context::create_program()
    {
        std::unique_ptr<program> ptr{on_create_program()};
        if (ptr) {
            program_handle handle{index(),
                                  0,
                                  static_cast<uint32_t>(m_programs.size())};
            m_programs.emplace_back(ptr.release());
            return handle;
        } else {
            return program_handle();
        }
    }

    index_buffer_handle render_context::create_index_buffer(
        data_type dt, size_t data_size, const buffer_ref& data)
    {
        std::unique_ptr<index_buffer> ptr{
            on_create_index_buffer(dt, data_size)};
        if (ptr) {
            index_buffer_handle handle{
                index(),
                0,
                static_cast<uint32_t>(m_index_buffers.size())};
            if (data) {
                index_buffer* ib = ptr.get();
                prepare_frame_action([ib, data]() {
                    ib->on_set_data(data->data(), data->size());
                });
            }
            m_index_buffers.emplace_back(ptr.release());
            return handle;
        } else {
            return index_buffer_handle();
        }
    }

    vertex_buffer_handle render_context::create_vertex_buffer(
        const vertex_layout& layout, size_t data_size, const buffer_ref& data)
    {
        std::unique_ptr<vertex_buffer> ptr{
            on_create_vertex_buffer(layout, data_size)};
        if (ptr) {
            vertex_buffer_handle handle{
                index(),
                0,
                static_cast<uint32_t>(m_vertex_buffers.size())};
            if (data) {
                vertex_buffer* vb = ptr.get();
                prepare_frame_action([vb, data]() {
                    vb->on_set_data(data->data(), data->size());
                });
            }
            m_vertex_buffers.emplace_back(ptr.release());
            return handle;
        } else {
            return vertex_buffer_handle();
        }
    }

    frame_buffer_handle render_context::create_frame_buffer()
    {
        std::unique_ptr<frame_buffer> ptr{on_create_frame_buffer()};
        if (ptr) {
            frame_buffer_handle handle{
                index(),
                0,
                static_cast<uint32_t>(m_frame_buffers.size())};
            m_frame_buffers.emplace_back(ptr.release());
            return handle;
        } else {
            return frame_buffer_handle();
        }
    }

    mge::pass& render_context::pass(uint32_t index)
    {
        while (index >= m_passes.size()) {
            m_passes.emplace_back(
                mge::pass(this, static_cast<uint32_t>(m_passes.size())));
        }
        return m_passes[index];
    }

    mge::command_buffer& render_context::command_buffer()
    {
        if (!m_command_buffer) {
            m_command_buffer = std::make_unique<mge::command_buffer>();
        }
        return *m_command_buffer;
    }

    mge::viewport render_context::default_viewport() const
    {
        return mge::viewport{0.0f,
                             0.0f,
                             static_cast<float>(m_extent.width),
                             static_cast<float>(m_extent.height),
                             0.0f,
                             1.0f};
    }

} // namespace mge