// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image.hpp"
#include "mge/graphics/memory_image.hpp"

namespace mge {

    image::image(const mge::extent& e,
                 const image_format& format)
        :m_extent(e)
        ,m_format(format)
    {}

    void *
    image::map()
    {
        return on_map();
    }

    void
    image::unmap()
    {
        on_unmap();
    }

    image_ref
    image::clone()
    {
        image_ref result = std::make_shared<memory_image>(extent(), format(), map());
        unmap();
        return result;
    }


}
