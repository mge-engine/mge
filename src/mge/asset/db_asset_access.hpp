// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_access.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/path.hpp"
#include "mge/core/properties.hpp"

struct sqlite3;

namespace mge {

    class MGEASSET_EXPORT db_asset_access : public asset_access
    {
    public:
        db_asset_access(const mge::path& asset_path, const mge::path& db_path);
        virtual ~db_asset_access();

        size_t            size() const override;
        input_stream_ref  data() const override;
        asset_type        type() const override;
        bool              has_properties() const override;
        properties_ref    properties() const override;
        output_stream_ref output_stream() override;
        void store_properties(const mge::properties& props) override;

    private:
        void ensure_schema();

        path                   m_asset_path;
        std::string            m_asset_path_str;
        path                   m_db_path;
        sqlite3*               m_db{nullptr};
        mutable properties_ref m_properties;
    };

} // namespace mge
