#pragma once
#include "mge/asset/asset_access_factory.hpp"
#include "mge/asset/file_asset_access.hpp"
namespace mge {

    class file_asset_access_factory : public asset_access_factory
    {
    public:
        file_asset_access_factory();
        virtual ~file_asset_access_factory() = default;
        void             configure(const mge::properties& p) override;
        bool             is_configured() const override;
        asset_access_ref access(const mge::path& p) override;
        bool             asset_exists(const mge::path& p) override;
        void             gist(std::ostream& os) const override;

    public:
        mge::path m_directory;
        bool      m_configured;
    };

} // namespace mge