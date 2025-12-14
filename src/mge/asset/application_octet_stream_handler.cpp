// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_handler.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    class application_octet_stream_handler : public asset_handler
    {
    public:
        application_octet_stream_handler() = default;
        ~application_octet_stream_handler() = default;

        std::any load(const mge::asset& a) override
        {
            auto buffer = std::make_shared<mge::buffer>();
            a.data()->read(*buffer);
            return buffer;
        }

        void store(const mge::asset&      a,
                   const mge::asset_type& type,
                   const std::any&        data) override
        {
            auto buffer = std::any_cast<std::shared_ptr<mge::buffer>>(data);
            auto stream = a.output_stream();
            stream->write(buffer->data(), buffer->size());
        }

        std::span<mge::asset_type>
        handled_types(asset_handler::operation_type t) const override
        {
            using namespace mge::literals;
            static asset_type supported[] = {"application/octet-stream"_at};
            return supported;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(application_octet_stream_handler,
                                mge::asset_handler,
                                octet_stream);
} // namespace mge