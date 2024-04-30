#pragma once
#include "mge/graphics/texture.hpp"
#include "test/googlemock.hpp"

class MOCK_texture : public mge::texture
{
public:
    MOCK_texture(mge::render_context& context, mge::texture_type type)
        : mge::texture(context, type)
    {}

    ~MOCK_texture() {}

    MOCK_METHOD(void,
                set_data,
                (mge::image_format, const mge::extent&, const void*, size_t),
                ());

    using mge::texture::set_data;
};