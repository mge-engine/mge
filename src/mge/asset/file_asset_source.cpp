// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/file_asset_source.hpp"
#include "mge/asset/file_asset_access.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge {

    MGE_DEFINE_TRACE(FILE_ASSET);

    MGE_REGISTER_IMPLEMENTATION(file_asset_source, mge::asset_source, file, fs);

    file_asset_source::file_asset_source()
        : m_configured(false)
    {}

    void file_asset_source::configure(const mge::properties& p)
    {
        if (p.exists("directory")) {
            m_directory = p.get<std::string>("directory");
            MGE_DEBUG_TRACE(FILE_ASSET,
                            "Configured file asset source with directory {}",
                            m_directory.string());
        } else {
            MGE_THROW(bad_configuration)
                << "File asset access factory requires 'directory' property";
        }
        m_configured = true;
    }

    bool file_asset_source::is_configured() const
    {
        return m_configured;
    }

    asset_access_ref file_asset_source::access(const mge::path& p)
    {
        if (!m_configured) {
            MGE_THROW(illegal_state)
                << "File asset access factory is not configured";
        }
        auto      rel_path = p.lexically_relative(mount_point());
        mge::path file_path = m_directory / rel_path;
        return std::make_shared<file_asset_access>(p, file_path);
    }

    bool file_asset_source::asset_exists(const mge::path& p)
    {
        if (!m_configured) {
            MGE_THROW(illegal_state)
                << "File asset access factory is not configured";
        }
        auto      rel_path = p.lexically_relative(mount_point());
        mge::path file_path = m_directory / rel_path;
        return std::filesystem::exists(file_path);
    }

    void file_asset_source::gist(std::format_context& context) const
    {
        std::format_to(context.out(),
                       "{{type: file, mount_point: {}, directory: {}}}",
                       mount_point().string(),
                       m_directory.string());
    }

} // namespace mge