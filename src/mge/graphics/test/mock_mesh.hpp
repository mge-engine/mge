#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/mesh.hpp"

namespace mge {
    class mock_mesh 
    {
    public:
        mock_mesh()
        {}

        ~mock_mesh()
        {}

        MOCK_CONST_METHOD0(layout, const mge::vertex_layout& ());
        MOCK_CONST_METHOD0(num_vertices, size_t ());
        MOCK_CONST_METHOD0(num_indices, size_t ());
        MOCK_CONST_METHOD1(vertices, vertex_buffer_ref (render_context&));
        MOCK_CONST_METHOD1(indices, index_buffer_ref (render_context&));
    };
}