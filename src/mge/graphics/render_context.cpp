// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/properties.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/frame_buffer.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/vertex_buffer.hpp"

namespace mge {
    extern parameter<bool> p_graphics_record_frames;

    MGE_DEFINE_PARAMETER_WITH_DEFAULT(uint64_t,
                                      graphics,
                                      screenshot_at_frame,
                                      "Frame number to capture screenshot",
                                      0);

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

    render_context::capabilities::capabilities() = default;

    render_context::capabilities::~capabilities() = default;

    uint32_t render_context::capabilities::max_texture_size() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_texture_3d_size() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_texture_cube_size() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_texture_array_layers() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_vertex_attributes() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_uniform_buffer_bindings() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_texture_bindings() const
    {
        return 1;
    }

    uint32_t render_context::capabilities::max_color_attachments() const
    {
        return 1;
    }

    const render_context::capabilities&
    render_context::context_capabilities() const
    {
        return *m_capabilities;
    }

    render_context::render_context(mge::render_system& rs,
                                   const mge::extent&  ext)
        : m_render_system(rs)
        , m_extent(ext)
        , m_window_extent(ext)
        , m_prepare_frame_resource(m_prepare_frame_memory.data(),
                                   m_prepare_frame_memory.size())
        , m_prepare_frame_actions(&m_prepare_frame_resource)
    {
        m_index = render_context_registry::instance->register_context(this);

        m_record_frames = MGE_PARAMETER(graphics, record_frames).get();
        m_screenshot_at_frame =
            MGE_PARAMETER(graphics, screenshot_at_frame).get();

        if (m_record_frames) {
            MGE_INFO_TRACE(GRAPHICS, "Frame recording is enabled");
        } else {
            MGE_DEBUG_TRACE(GRAPHICS, "Frame recording is disabled");
        }
    }

    render_context::~render_context()
    {
        render_context_registry::instance->unregister_context(m_index, this);
    }

    void render_context::reset_prepare_frame_actions()
    {
        // Swap with empty vector to detach from resource memory before
        // releasing. The scoped block ensures tmp (holding the old
        // elements) is destroyed while the resource memory is still valid.
        {
            std::pmr::vector<prepare_frame_action> tmp(
                &m_prepare_frame_resource);
            m_prepare_frame_actions.swap(tmp);
        }
        m_prepare_frame_resource.release();
    }

    void render_context::frame()
    {
        if (m_first_frame) {
            m_first_frame = false;
            if (m_record_frames) {
                auto fd = m_render_system.frame_debugger();
                if (fd) {
                    MGE_INFO_TRACE(GRAPHICS, "Starting frame recording");
                    fd->begin_capture();
                }
            }
        }

        if (!m_prepare_frame_actions.empty()) {
            try {
                for (const auto& action : m_prepare_frame_actions) {
                    action();
                }
            } catch (...) {
                reset_prepare_frame_actions();
                throw;
            }
            reset_prepare_frame_actions();
        }
        bool rendered = false;
        if (m_passes.size() > 0) {
            for (const auto& p : m_passes)
                if (p.active()) {
                    render(p);
                    rendered = true;
                }
            for (auto& p : m_passes) {
                p.reset();
            }
        }
        if (rendered) {
            if (m_screenshot_at_frame != 0 &&
                m_frame_counter == m_screenshot_at_frame) {
                auto img = screenshot();
                if (img) {
                    save_screenshot(img, m_frame_counter);
                }
            }
            on_frame_present();
        }
        ++m_frame_counter;
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
                prepare_frame([ib, data]() {
                    ib->on_set_data(data->data(), data->size());
                });
            }
            m_index_buffers.emplace_back(ptr.release());
            return handle;
        } else {
            return index_buffer_handle();
        }
    }

    index_buffer_handle render_context::create_index_buffer(data_type dt,
                                                            size_t    data_size)
    {
        return create_index_buffer(dt, data_size, buffer_ref());
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
                prepare_frame([vb, data]() {
                    vb->on_set_data(data->data(), data->size());
                });
            }
            m_vertex_buffers.emplace_back(ptr.release());
            return handle;
        } else {
            return vertex_buffer_handle();
        }
    }

    vertex_buffer_handle
    render_context::create_vertex_buffer(const vertex_layout& layout,
                                         size_t               data_size)
    {
        return create_vertex_buffer(layout, data_size, buffer_ref());
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

    mge::command_buffer& render_context::command_buffer(bool clear)
    {
        if (!m_command_buffer) {
            m_command_buffer = std::make_unique<mge::command_buffer>();
        } else if (clear) {
            m_command_buffer->clear();
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

    mge::rectangle render_context::default_scissor() const
    {
        return mge::rectangle{0, 0, m_extent.width, m_extent.height};
    }

    void render_context::set_screenshot_at_frame(uint64_t frame)
    {
        m_screenshot_at_frame = frame;
    }

    void render_context::save_screenshot(const image_ref& img, uint64_t frame)
    {
        try {
            mge::properties p;
            p.set("directory", ".");
            mge::asset::mount("/screenshot",
                              "file",
                              mge::asset_source::access_mode::READ_WRITE,
                              p);
            auto        aliases = m_render_system.alias_names();
            auto        comma_pos = aliases.find(',');
            std::string render_system_name(comma_pos != std::string_view::npos
                                               ? aliases.substr(0, comma_pos)
                                               : aliases);
            std::string filename = std::string(mge::executable_name()) + "-" +
                                   render_system_name + "-" +
                                   std::to_string(frame) + ".png";
            mge::asset  img_asset(std::string("/screenshot/") + filename);
            img_asset.store(mge::asset_type("image", "png"), img);
            MGE_INFO_TRACE(GRAPHICS, "Screenshot saved to {}", filename);
            mge::asset::umount("/screenshot");
        } catch (const std::exception& e) {
            MGE_INFO_TRACE(GRAPHICS, "Failed to save screenshot: {}", e.what());
        }
    }

} // namespace mge