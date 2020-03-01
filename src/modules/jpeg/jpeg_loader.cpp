#include "mge/asset/asset_type.hpp"
#include "mge/asset/asset_type_mapper.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/core/component.hpp"

static mge::asset_type_mapper jpeg_mapper(mge::asset_type("image", "jpeg"),
                                          "jpg,jpeg");

namespace jpeg {

    class loader : public mge::asset_loader
    {
    public:
        loader() = default;
        ~loader() = default;
        std::shared_ptr<std::any> load(const mge::asset& a) const override
        {
            std::shared_ptr<std::any> result;
            return result;
        }

        void handled_types(std::function<void (const mge::asset_type&)>&& f) const override
        {
            f(mge::asset_type("image","jpeg"));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(loader, ::mge::asset_loader, jpg, jpeg);

}
