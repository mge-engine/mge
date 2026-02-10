// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

namespace mge {

    /**
     * @brief A uniform block represents data of uniforms that is grouped
     * together.
     *
     * Note all uniforms in a program need to be part of a uniform block,
     * those who are not explicitly assigned to a block are implicitly part of
     * a default block.
     */
    class MGEGRAPHICS_EXPORT uniform_block
    {
    public:
        virtual ~uniform_block();

    protected:
        uniform_block(size_t data_size);

    private:
        void* m_data; //!< CPU-side data for the uniform block
    };

} // namespace mge