#include "mge/graphics/memory_image.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    memory_image::memory_image(const mge::extent& e, const image_format& f)
        :image(e, f)
    {}

    memory_image::memory_image(const mge::extent& e, const image_format& f, void *data)
        :image(e, f)
    {
        if (!data) {
            MGE_THROW(illegal_argument) << "Data for memory image must not be null";
        }

        memcpy(map(), data, buffer_size());
        unmap();
    }

    memory_image::~memory_image()
    {}

    void *
    memory_image::on_map()
    {
        if(m_data.size() != buffer_size()) {
            m_data.resize(buffer_size());
        }
        return m_data.data();
    }

    void
    memory_image::on_unmap()
    {}

    size_t
    memory_image::buffer_size() const
    {
        return extent().area() * pixel_size(format());
    }
}
