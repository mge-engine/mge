#pragma once
#include "mge/asset/asset_access_factory.hpp
#include "mge/asset/file_asset_access.hpp"
namespace mge {

    class file_access_factory : public asset_access_factory
    {
    public:
        file_access_factory() = default;
        virtual ~file_access_factory() = default;

        virtual asset_access_ref create(const mge::path& p) override
        {
            return std::make_shared<file_asset_access>(p);
        }
    };

} // namespace mge