#pragma once
#include "mge/graphics/render_context.hpp"
#include "system_config.hpp"
#include "vk/surface.hpp"

namespace vulkan {
    class window;
    class render_system;

    class render_context : public mge::render_context
    {
    public:
        render_context(window *win,
                       render_system& system,
                       const system_config& config);
        ~render_context();

        void flush() override;
        mge::vertex_buffer_ref create_vertex_buffer(const mge::vertex_layout& layout,
                                            mge::usage usage,
                                            size_t element_count,
                                            void *initial_data) override;
        mge::index_buffer_ref create_index_buffer(mge::data_type type,
                                                  mge::usage usage,
                                                  size_t element_count,
                                                  void *initial_data) override;
        mge::texture_2d_ref create_texture_2d(const mge::image_ref& image) override;
        mge::texture_2d_ref create_texture_2d() override;
        mge::shader_ref create_shader(mge::shader_type type) override;
        mge::pipeline_ref create_pipeline() override;
        mge::command_list_ref create_command_list() override;
        void execute(const mge::command_list_ref& commands) override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;

    private:
        vk::surface_ref m_surface;
        uint32_t        m_present_queue_family;
    };
}
