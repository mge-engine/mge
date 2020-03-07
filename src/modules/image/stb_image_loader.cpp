#include "mge/asset/asset_type.hpp"
#include "mge/asset/asset_type_mapper.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/component.hpp"
#include "mge/core/log.hpp"
#include "mge/core/buffer.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/memory_image.hpp"

#define STB_IMAGE_IMPLEMENTATION

// STBI_MALLOC
// STBI_REALLOC
// STBI_FREE
#include <stb_image.h>

MGE_DEFINE_LOG(IMGLOAD)

static mge::asset_type_mapper gif_mapper(mge::asset_type("image", "gif"),
                                         "gif");
static mge::asset_type_mapper bmp_mapper(mge::asset_type("image", "bmp"),
                                         "bmp");
static mge::asset_type_mapper png_mapper(mge::asset_type("image", "png"),
                                         "png");
static mge::asset_type_mapper tga_mapper(mge::asset_type("image", "tga"),
                                         "tga");

namespace imgload {
    class loader : public mge::asset_loader
    {
    private:
        stbi_io_callbacks m_callbacks;

        static int read_data(void *context, char *data, int size)
        {
            mge::input_stream_ref *stream =
                reinterpret_cast<mge::input_stream_ref *>(context);
            return static_cast<int>((*stream)->read(data, size));
        }

        static void skip_data(void *context, int n)
        {
            if (n < 0) {
                return;
            }
            char skipbuffer[256];
            mge::input_stream_ref *stream =
                reinterpret_cast<mge::input_stream_ref *>(context);
            while(n > 256) {
                auto rc = (*stream)->read(skipbuffer, 256);
                if (rc != 256) {
                    return;
                }
                n -= 256;
            }
            (*stream)->read(skipbuffer, n);
        }

        static int eof_data(void *context)
        {
            mge::input_stream_ref *stream =
                reinterpret_cast<mge::input_stream_ref *>(context);
            return (*stream)->eof()?1:0;
        }
    public:
        loader()
        {
            m_callbacks.read = &read_data;
            m_callbacks.skip = &skip_data;
            m_callbacks.eof = &eof_data;
        }
        ~loader() = default;



        std::any load(const mge::asset& a) const override
        {
            std::any result;
            auto asset_data = a.data();
            int x = 0;
            int y = 0;
            int components = STBI_rgb_alpha;
            auto loaded = stbi_load_from_callbacks(&m_callbacks,
                                                   &asset_data,
                                                   &x,
                                                   &y,
                                                   &components,
                                                   STBI_rgb_alpha);
            result = std::make_shared<mge::memory_image>
                    (mge::extent(x, y),
                     mge::image_format::RGBA8,
                     loaded,
                     true /* take ownership */);

            return result;
        }

        void handled_types(std::function<void (const mge::asset_type&)>&& f) const override
        {
            f(mge::asset_type("image","gif"));
            f(mge::asset_type("image","bmp"));
            f(mge::asset_type("image","png"));
            f(mge::asset_type("image","tga"));
        }

    };

    MGE_REGISTER_IMPLEMENTATION(loader, ::mge::asset_loader, tga, bmp, gif, png);
}
