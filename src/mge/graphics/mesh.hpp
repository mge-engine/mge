// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/vertex_layout.hpp"


namespace mge {

    /**
     * A mesh represents a geometry model that can be rendered without a state
     * change, i.e. using the same surface properties.
     */
    class MGE_GRAPHICS_EXPORT mesh 
    {
    public:
        /**
         * Constructor.
         */
        mesh();
        /**
         * Destructor.
         */
        virtual ~mesh();

        /**
         * Get the layout of the mesh's data.
         * @return mesh layout
         */
        virtual const mge::vertex_layout& layout() const noexcept = 0;
        
        /**
         * Get number of vertices.
         * 
         * @return number of vertices in this mesh
         */
        virtual size_t num_vertices() const noexcept = 0;
        
        /**
         * Get the number of indices.
         * @return number of elements of the mesh's index buffer
         */
        virtual size_t num_indices() const noexcept = 0;

        /**
         * Get a vertex buffer containing the mesh vertices.
         * 
         * @param context render context
         * @return vertex_buffer_ref vertices of mesh
         */
        virtual vertex_buffer_ref vertices(render_context& context) const = 0;

        /**
         * Get indices of mesh.
         * 
         * @param context render context
         * @return index_buffer_ref reference to index buffer
         */
        virtual index_buffer_ref indices(render_context& context) const = 0;
    };
}