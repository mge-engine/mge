// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_handler.hpp"
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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_MALLOC(sz) ::mge::malloc(sz)
#define STBIW_REALLOC(p, sz) ::mge::realloc(p, sz)
#define STBIW_FREE(p) ::mge::free(p)
#include <stb_image_write.h>

namespace mge {
    class stb_image_handler : public asset_handler
    {
    public:
        stb_image_handler() = default;
        ~stb_image_handler() = default;

        std::any load(const mge::asset& a) override;

        void store(const mge::asset&      a,
                   const mge::asset_type& type,
                   const std::any&        data) override
        {
            using namespace mge::literals;
            if (type != "image/png"_at && type != "image/jpeg"_at) {
                MGE_THROW(mge::illegal_argument)
                    << "Only PNG and JPEG formats are supported for storing, "
                       "got: "
                    << type;
            }

            auto img = std::any_cast<mge::image_ref>(data);
            auto fmt = img->format();

            if (fmt.type() != mge::data_type::UINT8) {
                MGE_THROW(mge::illegal_argument)
                    << "Only UINT8 images can be stored";
            }

            // Validate format based on image type
            if (type == "image/png"_at) {
                if (fmt.format() != mge::image_format::data_format::RGBA) {
                    MGE_THROW(mge::illegal_argument)
                        << "Only RGBA images can be stored as PNG";
                }
            } else if (type == "image/jpeg"_at) {
                if (fmt.format() != mge::image_format::data_format::RGB) {
                    MGE_THROW(mge::illegal_argument)
                        << "Only RGB images can be stored as JPEG";
                }
            }

            auto ext = img->extent();
            int  width = static_cast<int>(ext.width);
            int  height = static_cast<int>(ext.height);
            int  components =
                (fmt.format() == mge::image_format::data_format::RGBA) ? 4 : 3;
            int stride = width * components;

            // Create buffer to hold encoded data
            std::vector<unsigned char> image_buffer;

            auto write_func = [](void* context, void* data, int size) {
                auto* buf = static_cast<std::vector<unsigned char>*>(context);
                auto* bytes = static_cast<unsigned char*>(data);
                buf->insert(buf->end(), bytes, bytes + size);
            };

            int result = 0;
            if (type == "image/png"_at) {
                result = stbi_write_png_to_func(write_func,
                                                &image_buffer,
                                                width,
                                                height,
                                                components,
                                                img->data(),
                                                stride);
            } else if (type == "image/jpeg"_at) {
                result = stbi_write_jpg_to_func(write_func,
                                                &image_buffer,
                                                width,
                                                height,
                                                components,
                                                img->data(),
                                                90); // quality
            }

            if (result == 0) {
                MGE_THROW(mge::runtime_exception)
                    << "Failed to encode image as " << type;
            }

            auto stream = a.output_stream();
            stream->write(image_buffer.data(), image_buffer.size());
            stream->flush();
        }

        std::span<mge::asset_type>
            handled_types(asset_handler::operation_type) const override;

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

    std::any stb_image_handler::load(const mge::asset& a)
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

    std::span<mge::asset_type>
    stb_image_handler::handled_types(asset_handler::operation_type t) const
    {
        using namespace mge::literals;
        static asset_type supported_load[] = {"image/jpeg"_at,
                                              "image/bmp"_at,
                                              "image/tga"_at,
                                              "image/png"_at,
                                              "image/gif"_at};
        static asset_type supported_store[] = {"image/png"_at, "image/jpeg"_at};
        if (t == asset_handler::operation_type::LOAD) {
            return supported_load;
        } else {
            return supported_store;
        }
    }

    MGE_REGISTER_IMPLEMENTATION(
        stb_image_handler, mge::asset_handler, jpeg, bmp, tga, png, gif);
} // namespace mge