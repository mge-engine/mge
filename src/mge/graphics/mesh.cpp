// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/mesh.hpp"

namespace mge {

    mesh::mesh(const vertex_layout& layout, data_type index_element_type)
        : m_vertex_layout(layout)
        , m_index_element_type(index_element_type)
    {}

} // namespace mge