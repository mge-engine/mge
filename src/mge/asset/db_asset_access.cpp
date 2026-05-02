// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/db_asset_access.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/io_exception.hpp"
#include "mge/core/output_stream.hpp"
#include "mge/core/stdexceptions.hpp"

#include <magic.h>
#include <sqlite3.h>

#include <cstring>
#include <string>
#include <vector>

namespace mge {

    namespace {

        static std::string detect_mime_type(const void* data, size_t size)
        {
            if (!data || size == 0) {
                return {};
            }
            magic_t mc = magic_open(MAGIC_MIME_TYPE);
            if (!mc) {
                return {};
            }
            auto        exe_path = mge::executable_path();
            std::string magic_file(exe_path.begin(), exe_path.end());
#ifdef MGE_OS_WINDOWS
            magic_file += "\\mge_magic.mgc";
#else
            magic_file += "/mge_magic.mgc";
#endif
            if (magic_load(mc, magic_file.c_str()) != 0) {
                magic_close(mc);
                return {};
            }
            const char* t = magic_buffer(mc, data, size);
            std::string result = t ? t : "";
            magic_close(mc);
            return result;
        }

        class db_input_stream : public input_stream
        {
        public:
            db_input_stream(std::vector<uint8_t> data)
                : m_data(std::move(data))
                , m_pos(0)
            {}

            ~db_input_stream() override = default;

            offset_type position() override
            {
                return m_pos;
            }

            offset_type seek(offset_type offset, direction_type dir) override
            {
                offset_type newpos = 0;
                switch (dir) {
                case POS_BEG:
                    newpos = offset;
                    break;
                case POS_CUR:
                    newpos = static_cast<offset_type>(m_pos) + offset;
                    break;
                case POS_END:
                    newpos = static_cast<offset_type>(m_data.size()) + offset;
                    break;
                default:
                    MGE_THROW(illegal_argument) << "Invalid seek direction";
                }
                if (newpos < 0 ||
                    newpos > static_cast<offset_type>(m_data.size())) {
                    MGE_THROW(out_of_range) << "Seek position out of range";
                }
                m_pos = static_cast<size_t>(newpos);
                return m_pos;
            }

        private:
            streamsize_type on_read(void*           destination,
                                    streamsize_type size) override
            {
                size_t remaining = m_data.size() - m_pos;
                size_t to_read = std::min(static_cast<size_t>(size), remaining);
                if (to_read > 0) {
                    std::memcpy(destination, m_data.data() + m_pos, to_read);
                    m_pos += to_read;
                }
                return static_cast<streamsize_type>(to_read);
            }

            std::vector<uint8_t> m_data;
            size_t               m_pos;
        };

        class db_output_stream : public output_stream
        {
        public:
            db_output_stream(sqlite3* db, const std::string& asset_path)
                : m_db(db)
                , m_asset_path(asset_path)
            {}

            ~db_output_stream() override
            {
                if (!m_buffer.empty()) {
                    flush_to_db();
                }
            }

            void on_write(const void* data, streamsize_type size) override
            {
                const auto* bytes = static_cast<const uint8_t*>(data);
                m_buffer.insert(m_buffer.end(), bytes, bytes + size);
            }

            void on_flush() override
            {
                flush_to_db();
            }

        private:
            void flush_to_db()
            {
                std::string mime =
                    detect_mime_type(m_buffer.data(), m_buffer.size());
                const char* sql =
                    "INSERT OR REPLACE INTO assets(path, data, type) "
                    "VALUES(?, ?, ?)";
                sqlite3_stmt* stmt = nullptr;
                if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) !=
                    SQLITE_OK) {
                    MGE_THROW(io_exception)
                        << "Failed to prepare asset insert statement: "
                        << sqlite3_errmsg(m_db);
                }
                sqlite3_bind_text(stmt,
                                  1,
                                  m_asset_path.c_str(),
                                  -1,
                                  SQLITE_STATIC);
                sqlite3_bind_blob(stmt,
                                  2,
                                  m_buffer.data(),
                                  static_cast<int>(m_buffer.size()),
                                  SQLITE_STATIC);
                if (!mime.empty()) {
                    sqlite3_bind_text(stmt,
                                      3,
                                      mime.c_str(),
                                      -1,
                                      SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, 3);
                }
                int rc = sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                if (rc != SQLITE_DONE) {
                    MGE_THROW(io_exception) << "Failed to store asset data: "
                                            << sqlite3_errmsg(m_db);
                }
                m_buffer.clear();
            }

            sqlite3*             m_db;
            std::string          m_asset_path;
            std::vector<uint8_t> m_buffer;
        };

    } // namespace

    db_asset_access::db_asset_access(const mge::path& asset_path,
                                     const mge::path& db_path)
        : m_asset_path(asset_path)
        , m_asset_path_str(asset_path.string())
        , m_db_path(db_path)
    {
        int rc = sqlite3_open(m_db_path.string().c_str(), &m_db);
        if (rc != SQLITE_OK) {
            std::string msg = m_db ? sqlite3_errmsg(m_db) : "unknown error";
            sqlite3_close(m_db);
            m_db = nullptr;
            MGE_THROW(io_exception) << "Cannot open asset database '"
                                    << m_db_path.string() << "': " << msg;
        }
        ensure_schema();
    }

    db_asset_access::~db_asset_access()
    {
        if (m_db) {
            sqlite3_close(m_db);
            m_db = nullptr;
        }
    }

    void db_asset_access::ensure_schema()
    {
        const char* sql = "CREATE TABLE IF NOT EXISTS assets ("
                          "  path TEXT NOT NULL PRIMARY KEY,"
                          "  data BLOB,"
                          "  type TEXT"
                          ");"
                          "CREATE TABLE IF NOT EXISTS asset_properties ("
                          "  path  TEXT NOT NULL,"
                          "  key   TEXT NOT NULL,"
                          "  value TEXT NOT NULL,"
                          "  PRIMARY KEY (path, key),"
                          "  FOREIGN KEY (path) REFERENCES assets(path)"
                          ");";
        char*       errmsg = nullptr;
        int         rc = sqlite3_exec(m_db, sql, nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK) {
            std::string msg = errmsg ? errmsg : "unknown error";
            sqlite3_free(errmsg);
            MGE_THROW(io_exception)
                << "Failed to create asset database schema: " << msg;
        }
    }

    size_t db_asset_access::size() const
    {
        const char*   sql = "SELECT length(data) FROM assets WHERE path=?";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            MGE_THROW(io_exception)
                << "Failed to prepare size query: " << sqlite3_errmsg(m_db);
        }
        sqlite3_bind_text(stmt, 1, m_asset_path_str.c_str(), -1, SQLITE_STATIC);
        size_t result = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = static_cast<size_t>(sqlite3_column_int64(stmt, 0));
        }
        sqlite3_finalize(stmt);
        return result;
    }

    input_stream_ref db_asset_access::data() const
    {
        const char*   sql = "SELECT data FROM assets WHERE path=?";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            MGE_THROW(io_exception)
                << "Failed to prepare data query: " << sqlite3_errmsg(m_db);
        }
        sqlite3_bind_text(stmt, 1, m_asset_path_str.c_str(), -1, SQLITE_STATIC);
        std::vector<uint8_t> buffer;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const void* blob = sqlite3_column_blob(stmt, 0);
            int         bytes = sqlite3_column_bytes(stmt, 0);
            if (blob && bytes > 0) {
                const auto* src = static_cast<const uint8_t*>(blob);
                buffer.assign(src, src + bytes);
            }
        }
        sqlite3_finalize(stmt);
        return std::make_shared<db_input_stream>(std::move(buffer));
    }

    asset_type db_asset_access::type() const
    {
        const char*   sql = "SELECT type FROM assets WHERE path=?";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            return asset_type::UNKNOWN;
        }
        sqlite3_bind_text(stmt, 1, m_asset_path_str.c_str(), -1, SQLITE_STATIC);
        asset_type result = asset_type::UNKNOWN;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* t =
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (t && *t) {
                result = asset_type::parse(t);
            }
        }
        sqlite3_finalize(stmt);
        return result;
    }

    bool db_asset_access::has_properties() const
    {
        if (m_properties) {
            return true;
        }
        const char* sql = "SELECT COUNT(*) FROM asset_properties WHERE path=?";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }
        sqlite3_bind_text(stmt, 1, m_asset_path_str.c_str(), -1, SQLITE_STATIC);
        bool has = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            has = sqlite3_column_int(stmt, 0) > 0;
        }
        sqlite3_finalize(stmt);
        return has;
    }

    properties_ref db_asset_access::properties() const
    {
        if (!m_properties) {
            const char* sql =
                "SELECT key, value FROM asset_properties WHERE path=?";
            sqlite3_stmt* stmt = nullptr;
            if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) !=
                SQLITE_OK) {
                MGE_THROW(io_exception)
                    << "Failed to prepare properties query: "
                    << sqlite3_errmsg(m_db);
            }
            sqlite3_bind_text(stmt,
                              1,
                              m_asset_path_str.c_str(),
                              -1,
                              SQLITE_STATIC);
            auto props = std::make_shared<mge::properties>();
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                const char* key =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                const char* value =
                    reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                if (key && value) {
                    props->put(std::string(key), std::string(value));
                }
            }
            sqlite3_finalize(stmt);
            m_properties = std::move(props);
        }
        return m_properties;
    }

    output_stream_ref db_asset_access::output_stream()
    {
        return std::make_shared<db_output_stream>(m_db, m_asset_path_str);
    }

    void db_asset_access::store_properties(const mge::properties& props)
    {
        const char*   del_sql = "DELETE FROM asset_properties WHERE path=?";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(m_db, del_sql, -1, &stmt, nullptr) !=
            SQLITE_OK) {
            MGE_THROW(io_exception) << "Failed to prepare properties delete: "
                                    << sqlite3_errmsg(m_db);
        }
        sqlite3_bind_text(stmt, 1, m_asset_path_str.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        const char* ins_sql =
            "INSERT INTO asset_properties(path, key, value) VALUES(?,?,?)";
        for (const auto& [key, value] : props) {
            if (sqlite3_prepare_v2(m_db, ins_sql, -1, &stmt, nullptr) !=
                SQLITE_OK) {
                MGE_THROW(io_exception) << "Failed to prepare property insert: "
                                        << sqlite3_errmsg(m_db);
            }
            sqlite3_bind_text(stmt,
                              1,
                              m_asset_path_str.c_str(),
                              -1,
                              SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, value.c_str(), -1, SQLITE_STATIC);
            int rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            if (rc != SQLITE_DONE) {
                MGE_THROW(io_exception) << "Failed to insert property '" << key
                                        << "': " << sqlite3_errmsg(m_db);
            }
        }
        m_properties = std::make_shared<mge::properties>(props);
    }

} // namespace mge
