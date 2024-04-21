// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_access.hpp"
#include "mge/core/file_input_stream.hpp"
#include "mge/core/path.hpp"
#include "mge/core/properties.hpp"

namespace mge {

    class file_asset_access : public asset_access
    {
    public:
        file_asset_access(const mge::path& asset_path,
                          const mge::path& file_path);
        virtual ~file_asset_access();

        size_t           size() const override;
        input_stream_ref data() const override;
        asset_type       type() const override;
        bool             has_properties() const override;
        properties_ref   properties() const override;

    private:
        path                   m_asset_path;
        path                   m_file_path;
        mutable properties_ref m_properties;
    };

} // namespace mge