// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/topology.hpp"
namespace mge {

    class pass;

    class MGEGRAPHICS_EXPORT encoder
    {
    public:
        friend class pass;

        /**
         * @brief Default destructor.
         */
        ~encoder();

        void set_vertices(mge::vertex_buffer* vb);
        void set_indices(mge::index_buffer* ib);
        void set_topology(mge::topology t);
        void submit(mge::program* program);

    private:
        mge::pass* m_pass;
    };

} // namespace mge