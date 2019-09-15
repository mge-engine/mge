#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/pipeline.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/graphics/index_buffer.hpp"


namespace mge {

    /**
     * Command to execute a pipeline using specified settings.
     */
    class MGE_GRAPHICS_EXPORT command
    {
    public:
        /**
         * Create empty command.
         */
        command();
        ~command() = default;

        void set_pipeline(const pipeline_ref& pipeline);
        void set_vertices(uint32_t index, const vertex_buffer_ref& buffer);
        void set_indices(const index_buffer_ref& buffer);
    private:
        pipeline_ref m_pipeline;
    };

}
