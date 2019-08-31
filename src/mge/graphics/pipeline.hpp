// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/core/property_object.hpp"

#include <string>

namespace mge {

    /**
     * A pipeline represents the stages in the drawing process. 
     * 
     * When applied to a render command list, a pipeline executes several stages:
     * - input assembly, where bound buffers are processed and vertices are
     *   generated depending on the drawing instructions
     * - vertex processing, where vertex, tessellation and geometry shaders 
     *   are executed, resulting in transformed output vertices
     * - clipping, where a subset of vertices is selected for further processing
     * - rasterization, where frame buffer addresses (fragments) are computed
     * - fragment processing, where fragment (pixel) shaders are executed for
     *   each fragment 
     * - frame buffer operations, where the computed fragment is applied to the
     *   frame buffer, with possible depth buffering, blending, masking, stenciling
     *    or other similar operations
     * 
     * Pipelines are either implemented with corresponding objects in the render
     * system, or handled in software by executing a number of calls that implement
     * the mentioned stages.
     */
    class MGE_GRAPHICS_EXPORT pipeline
            : public context_object
    {
    protected:
        pipeline(render_context& context);
    public:
        virtual ~pipeline();

        /**
         * Set a shader object. The shader object must be a shader designated 
         * for the graphics pipeline (not a compute shader).
         * 
         * @param shader shader object that is set
         */
        void set_shader(const shader_ref& shader);

        /**
         * Links the pipeline. A pipeline must be linked after setting all shader
         * programs.
         */
        void link();

        /**
         * Return whether this pipeline needs the link step.
         * 
         * @return @c true if pipeline needs to be linked
         */
        bool needs_link() const noexcept { return m_needs_link; }
    protected:
        bool m_needs_link;

        virtual void on_link() = 0;
        virtual void on_set_shader(const shader_ref& shader) = 0;
    };
}
