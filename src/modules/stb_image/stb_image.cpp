// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/memory.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/image.hpp"
#include "mge/graphics/memory_image.hpp"

#define STB_IMAGE_IMPLEMENTATION

#ifndef STBI_MALLOC
#    define STBI_MALLOC(sz) ::mge::malloc(sz)
#    define STBI_REALLOC(p, sz) ::mge::realloc(p, sz)
#    define STBI_FREE(p) ::mge::free(p)
#endif

#include <stb_image.h>

namespace mge {
    class stb_image_loader : public asset_loader
    {
    public:
        stb_image_loader() = default;
        ~stb_image_loader() = default;

        std::any load(const mge::asset& a) override;

        std::span<mge::asset_type> handled_types() const override;

        static int read_callback(void* context, char* data, int size)
        {
            mge::input_stream* stream =
                reinterpret_cast<mge::input_stream*>(context);
            return mge::checked_cast<int>(stream->read(data, size));
        }

        static void skip_callback(void* context, int n)
        {
            if (n < 0) {
                return;
            }
            char               skipbuffer[256];
            mge::input_stream* stream =
                reinterpret_cast<mge::input_stream*>(context);
            while (n > 256) {
                auto rc = stream->read(skipbuffer, 256);
                if (rc != 256) {
                    return;
                }
                n -= 256;
            }
            stream->read(skipbuffer, n);
        }

        static int eof_callback(void* context)
        {
            mge::input_stream* stream =
                reinterpret_cast<mge::input_stream*>(context);
            return stream->eof() ? 1 : 0;
        }

        static const inline stbi_io_callbacks s_callbacks = {
            .read = read_callback, .skip = skip_callback, .eof = eof_callback};
    };

    std::any stb_image_loader::load(const mge::asset& a)
    {
        int  x = 0;
        int  y = 0;
        int  components = STBI_rgb_alpha;
        auto loaded = stbi_load_from_callbacks(&s_callbacks,
                                               a.data().get(),
                                               &x,
                                               &y,
                                               &components,
                                               STBI_rgb_alpha);
        try {
            mge::image_ref result = std::make_shared<mge::memory_image>(
                mge::image_format(mge::image_format::data_format::RGBA,
                                  mge::data_type::UINT8),
                mge::extent(x, y),
                loaded,
                false,
                x * y * 4);
            return result;
        } catch (...) {
            stbi_image_free(loaded);
            throw;
        }
    }

    std::span<mge::asset_type> stb_image_loader::handled_types() const
    {
        using namespace mge::literals;
        static asset_type supported[] = {"image/jpeg"_at,
                                         "image/bmp"_at,
                                         "image/tga"_at,
                                         "image/png"_at,
                                         "image/gif"_at};
        return supported;
    }

    MGE_REGISTER_IMPLEMENTATION(
        stb_image_loader, mge::asset_loader, jpeg, bmp, tga, png, gif);
} // namespace mge