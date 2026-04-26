// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_source.hpp"
#include "mge/asset/db_asset_access.hpp"

namespace mge {

    class db_asset_source : public asset_source
    {
    public:
        db_asset_source();
        virtual ~db_asset_source() = default;
        void             configure(const mge::properties& p) override;
        bool             is_configured() const override;
        asset_access_ref access(const mge::path& p) override;
        bool             asset_exists(const mge::path& p) override;
        void             gist(std::format_context& context) const override;

    private:
        mge::path m_db_path;
        bool      m_configured;
    };

} // namespace mge
