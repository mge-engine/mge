// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/singleton.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/swap_chain.hpp"

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

    render_context::render_context(const mge::extent& ext)
        : m_extent(ext)
    {
        m_index = render_context_registry::instance->register_context(this);
    }

    render_context::~render_context()
    {
        render_context_registry::instance->unregister_context(m_index, this);
    }

    frame_command_list* render_context::create_current_frame_command_list()
    {
        class delegating_frame_command_list : public frame_command_list
        {
        public:
            delegating_frame_command_list(command_list* l)
                : frame_command_list(l->context(),
                                     frame_command_list::NO_BACKBUFFER,
                                     l->native())
                , m_command_list(l)
            {}

            ~delegating_frame_command_list()
            {
                if (m_command_list) {
                    m_command_list->destroy();
                }
            }

            void viewport(const mge::viewport& vp) override
            {
                m_command_list->viewport(vp);
            }

            void scissor(const rectangle& rect) override
            {
                m_command_list->scissor(rect);
            }

            void default_scissor() override
            {
                m_command_list->default_scissor();
            }

            void clear(const rgba_color& c) override
            {
                m_command_list->clear(c);
            }

            void clear_depth(float depth) override
            {
                m_command_list->clear_depth(depth);
            }

            void clear_stencil(int32_t stencil) override
            {
                m_command_list->clear_stencil(stencil);
            }

            void draw(const draw_command& command) override
            {
                m_command_list->draw(command);
            }

            void execute() override
            {
                m_command_list->execute();
            }
            void finish() override
            {
                m_command_list->finish();
            }

        private:
            command_list* m_command_list;
        };

        auto ptr = std::make_unique<delegating_frame_command_list>(
            create_command_list());
        auto* result = ptr.release();
        return result;
    }

    void render_context::frame()
    {
        if (!m_prepare_frame_actions.empty()) {
            try {
                for (const auto& [action, on_success] :
                     m_prepare_frame_actions) {
                    action();
                    on_success();
                }
            } catch (...) {
                m_prepare_frame_actions.clear();
                throw;
            }
            m_prepare_frame_actions.clear();
        } else {
            if (m_swap_chain) {
                m_swap_chain->present();
            }
        }
    }

    image_ref render_context::screenshot()
    {
        if (m_swap_chain) {
            return m_swap_chain->screenshot();
        }
        return image_ref();
    }

    render_context* render_context::get(uint16_t index)
    {
        return render_context_registry::instance->get(index);
    }

    shader_handle render_context::create_shader(shader_type t)
    {
        std::unique_ptr<shader> ptr{on_create_shader(t)};
        if (ptr) {
            shader_handle handle{index(),
                                 0,
                                 static_cast<uint32_t>(m_shaders.size())};
            m_shaders.emplace_back(ptr.get());
            ptr.release();
            return handle;
        }
        return shader_handle();
    }

} // namespace mge