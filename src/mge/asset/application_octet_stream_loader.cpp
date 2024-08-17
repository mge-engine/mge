#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/buffer.hpp"

namespace mge {
    class application_octet_stream_loader : public asset_loader
    {
    public:
        application_octet_stream_loader() = default;
        ~application_octet_stream_loader() = default;

        std::any load(const mge::asset& a) override
        {
            auto buffer = std::make_shared<mge::buffer>();
            a.data()->read(*buffer);
            return buffer;
        }

        std::span<mge::asset_type> handled_types() const override
        {
            using namespace mge::literals;
            static asset_type supported[] = {"application/octet-stream"_at};
            return supported;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(application_octet_stream_loader,
                                mge::asset_loader,
                                octet_stream);
} // namespace mge