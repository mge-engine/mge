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

#include <memory>

#include <jpeglib.h>

MGE_DEFINE_LOG(JPEG);

static mge::asset_type_mapper jpeg_mapper(mge::asset_type("image", "jpeg"),
                                          "jpg,jpeg");

namespace jpeg {

    class loader : public mge::asset_loader
    {
    public:
        loader() = default;
        ~loader() = default;


        static void output_message(j_common_ptr cinfo)
        {
            char buffer[JMSG_LENGTH_MAX];
            (*cinfo->err->format_message)(cinfo, buffer);
            MGE_DEBUG_LOG(JPEG) << buffer;
        }

        static void emit_message(j_common_ptr cinfo, int msg_level)
        {
            struct jpeg_error_mgr * err = cinfo->err;
            char buffer[JMSG_LENGTH_MAX];

            if (msg_level < 0) {
                (*cinfo->err->format_message)(cinfo, buffer);
                MGE_WARNING_LOG(JPEG) << buffer;
                err->num_warnings++;
            } else {
                (*cinfo->err->format_message)(cinfo, buffer);
                MGE_DEBUG_LOG(JPEG) << buffer;
            }
        }

        static void error_exit(j_common_ptr cinfo)
        {
            char buffer[JMSG_LENGTH_MAX];
            (*cinfo->err->format_message)(cinfo, buffer);

            /* Always display the message */
            (*cinfo->err->output_message)(cinfo);
            /* Let the memory manager delete any temp files before we die */
            jpeg_destroy(cinfo);
//            throw AGGE_EXCEPTION(agge::asset::asset_corrupted)<< "JPEG image corrupted: "
//            << buffer
//            << ".";
            MGE_THROW(mge::runtime_exception) << "foobar";
        }


        static mge::buffer read_fully(const mge::input_stream_ref& is)
        {
            is->seek(0, mge::input_stream::POS_END);
            auto position = is->position();
            is->seek(0, mge::input_stream::POS_BEG);
            if (position < 0) {
                MGE_THROW(mge::illegal_state) << "Stream length < 0 read.";
            }
            size_t insize = (size_t) position;
            mge::buffer result;
            result.resize(insize);
            is->read(result.data(),
                     (mge::input_stream::streamsize_type) insize);
            return result;
        }


        std::any load(const mge::asset& a) const override
        {
            std::shared_ptr<std::any> result;

            auto is = a.data();
            struct jpeg_decompress_struct cinfo;
            struct jpeg_error_mgr jerr;
            cinfo.err = jpeg_std_error(&jerr);
            jerr.error_exit = error_exit;
            jerr.output_message = output_message;
            jerr.emit_message = emit_message;
            jpeg_create_decompress(&cinfo);
            try {
                mge::buffer inbuffer(read_fully(is));
                jpeg_mem_src(&cinfo, inbuffer.data(), (unsigned long)inbuffer.size());
                jpeg_read_header(&cinfo, TRUE);
                mge::image_format fmt;
                if (cinfo.num_components == 4) {
                    MGE_THROW_NOT_IMPLEMENTED << "Only RGB8 format supported";
                } else {
                    fmt = mge::image_format::RGB8;
                    cinfo.out_color_space = JCS_RGB;
                    cinfo.quantize_colors = FALSE;
                }
                jpeg_start_decompress(&cinfo);
                auto img = std::make_shared<mge::memory_image>
                    (mge::extent(cinfo.output_width, cinfo.output_height),
                     fmt);
                while (cinfo.output_scanline < cinfo.output_height) {
                    JSAMPROW row[1];
                    row[0] = (unsigned char *) img->scanline(cinfo.output_scanline);
                    jpeg_read_scanlines(&cinfo, row, 1);
                }
                jpeg_finish_decompress(&cinfo);
                jpeg_destroy_decompress(&cinfo);
                mge::image_ref imgref = img;
                return imgref;
            } catch(...) {
                jpeg_destroy_decompress(&cinfo);
                throw;
            }

            return result;
        }

        void handled_types(std::function<void (const mge::asset_type&)>&& f) const override
        {
            f(mge::asset_type("image","jpeg"));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(loader, ::mge::asset_loader, jpg, jpeg);

}
