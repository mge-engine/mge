// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "texture.hpp"

namespace mge {

    texture::texture(render_context& ctx, texture_type type)
        : context_object(ctx)
        , m_type(type)
    {}

    texture_type texture::type() const
    {
        return m_type;
    }

    void texture::set_data(const image& img)
    {
        set_data(img.format(), img.extent(), img.data(), img.binary_size());
    }

} // namespace mge