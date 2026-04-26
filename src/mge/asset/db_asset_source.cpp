// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/db_asset_source.hpp"
#include "mge/asset/db_asset_access.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

#include <sqlite3.h>

namespace mge {

    MGE_DEFINE_TRACE(DB_ASSET);

    MGE_REGISTER_IMPLEMENTATION(db_asset_source, mge::asset_source, db);

    db_asset_source::db_asset_source()
        : m_configured(false)
    {}

    void db_asset_source::configure(const mge::properties& p)
    {
        if (p.exists("path")) {
            m_db_path = p.get<std::string>("path");
            MGE_DEBUG_TRACE(DB_ASSET,
                            "Configured db asset source with path {}",
                            m_db_path.string());
        } else {
            MGE_THROW(bad_configuration)
                << "DB asset source requires 'path' property";
        }
        m_configured = true;
    }

    bool db_asset_source::is_configured() const
    {
        return m_configured;
    }

    asset_access_ref db_asset_source::access(const mge::path& p)
    {
        if (!m_configured) {
            MGE_THROW(illegal_state) << "DB asset source is not configured";
        }
        return std::make_shared<db_asset_access>(p, m_db_path);
    }

    bool db_asset_source::asset_exists(const mge::path& p)
    {
        if (!m_configured) {
            MGE_THROW(illegal_state) << "DB asset source is not configured";
        }
        sqlite3* db = nullptr;
        int      rc = sqlite3_open_v2(m_db_path.string().c_str(),
                                      &db,
                                      SQLITE_OPEN_READONLY,
                                      nullptr);
        if (rc != SQLITE_OK) {
            if (db) {
                sqlite3_close(db);
            }
            return false;
        }
        const char*   sql = "SELECT 1 FROM assets WHERE path=? LIMIT 1";
        sqlite3_stmt* stmt = nullptr;
        bool          exists = false;
        std::string   path_str = p.string();
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, path_str.c_str(), -1, SQLITE_STATIC);
            exists = sqlite3_step(stmt) == SQLITE_ROW;
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
        return exists;
    }

    void db_asset_source::gist(std::format_context& context) const
    {
        std::format_to(context.out(),
                       "{{type: db, mount_point: {}, path: {}}}",
                       mount_point().string(),
                       m_db_path.string());
    }

} // namespace mge
