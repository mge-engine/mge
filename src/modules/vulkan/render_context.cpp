#include "render_context.hpp"
#include "render_system.hpp"
#include "window.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(VULKAN);

namespace vulkan {

    render_context::render_context(window *win, 
                                   render_system& system,
                                   const system_config &config)
        : mge::render_context(win)
    {
        MGE_DEBUG_LOG(VULKAN) << "Create render context";
    }

    render_context::~render_context()
    {}

    void
    render_context::flush()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::vertex_buffer_ref
    render_context::create_vertex_buffer(const mge::vertex_layout& layout,
                                         mge::usage usage,
                                         size_t element_count,
                                         void *initial_data)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type,
                                        mge::usage usage,
                                        size_t element_count,
                                        void *initial_data)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d(const mge::image_ref& image)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::shader_ref
    render_context::create_shader(mge::shader_type type)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::pipeline_ref
    render_context::create_pipeline()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::command_list_ref
    render_context::create_command_list()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    render_context::execute(const mge::command_list_ref& commands)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    render_context::shader_languages(std::vector<mge::shader_language>& languages) const
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

}
