// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.

#include "mge/core/configuration_database.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/path.hpp"
#include "mge/core/stdexceptions.hpp"

#include <sqlite3.h>

#include <cstring>
#include <fcntl.h>

namespace mge {
    configuration_database::configuration_database() : m_db(nullptr)
    {
        open_database();
    }

    configuration_database::~configuration_database() { close_database(); }

    configuration_database &configuration_database::instance()
    {
        static singleton<configuration_database> instance;
        return *instance;
    }

    void configuration_database::open_database()
    {
        path p(get_configuration_directory());

        std::string databasename;
        databasename.append("mge_config_");
        databasename.append(executable_name());
        databasename.append(".db");

        p /= databasename;
        std::string dbstring = p.string();

        int open_rc = sqlite3_open(dbstring.c_str(), (sqlite3 **)&m_db);

        if (open_rc) {
            std::string open_error = sqlite3_errmsg((sqlite3 *)m_db);
            MGE_THROW(runtime_exception)
                << "Cannot open configuration database: " << open_error;
        }

        check_schema();
    }

    void configuration_database::close_database()
    {
        sqlite3_close((sqlite3 *)m_db);
        m_db = nullptr;
    }

    void configuration_database::check_schema()
    {
        if (!schema_exists()) {
            create_schema();
        }
    }

    static int schema_exists_callback(void *context, int cols, char **values,
                                      char **)
    {
        int *exists = (int *)context;
        if (cols != 1) {
            *exists = 0;
        } else {
            if (strcmp(values[0], "1") == 0) {
                *exists = 1;
            } else {
                *exists = 0;
            }
        }
        return SQLITE_OK;
    }

    bool configuration_database::schema_exists()
    {
        int   exists       = 0;
        char *errormessage = 0;

        int rc = sqlite3_exec((sqlite3 *)m_db,
                              "SELECT 1 FROM SQLITE_MASTER WHERE TYPE = "
                              "'table' AND NAME='CONFIGURATION'",
                              &schema_exists_callback, &exists, &errormessage);

        if (rc != SQLITE_OK) {
            std::string error(errormessage);
            sqlite3_free(errormessage);
            MGE_THROW(runtime_exception)
                << "Cannot query configuration database: " << error;
        }
        return exists == 1;
    }

    void configuration_database::create_schema()
    {
        const char *statements[] = {
            "CREATE TABLE CONFIGURATION(NAME NVARCHAR(256) PRIMARY KEY, "
            "                           VALUE NVARCHAR(256)) ",
            0};

        int i = 0;
        while (statements[i]) {
            char *errormessage;
            int   rc = sqlite3_exec((sqlite3 *)m_db, statements[i], NULL, NULL,
                                  &errormessage);
            if (rc != SQLITE_OK) {
                std::string error(errormessage);
                sqlite3_free(errormessage);
                MGE_THROW(runtime_exception)
                    << "Cannot create configuration database: " << error;
            }
            ++i;
        }
    }

    std::string configuration_database::get_configuration_directory()
    {
        char *configdir = getenv("MOGE_CONFIG_DIR");
        if (!configdir) {
            return std::string(".");
        } else {
            return std::string(configdir);
        }
    }

    static int store_values_callback(void *context, int, char **values, char **)
    {
        std::map<std::string, std::string> *valuemap =
            (std::map<std::string, std::string> *)context;
        (*valuemap)[values[0]] = values[1];
        return SQLITE_OK;
    }

    void configuration_database::read_configuration_values(
        std::map<std::string, std::string> &values)
    {
        char *errormessage = 0;
        int   rc =
            sqlite3_exec((sqlite3 *)m_db,
                         "SELECT NAME, VALUE FROM CONFIGURATION ORDER BY NAME",
                         store_values_callback, &values, &errormessage);
        if (rc != SQLITE_OK) {
            values.clear();
            std::string error(errormessage);
            sqlite3_free(errormessage);
            MGE_THROW(runtime_exception)
                << "Cannot read configuration values: " << error;
        }
    }

    void configuration_database::read_configuration_values(
        const char *prefix, std::map<std::string, std::string> &values)
    {
        std::stringstream ss;
        char *            errormessage = 0;

        ss << "SELECT SUBSTR(NAME, LENGTH('" << prefix
           << "') + 2, LENGTH(NAME)), VALUE FROM CONFIGURATION "
           << "WHERE SUBSTR(NAME, 1, LENGTH('" << prefix << "')) = '" << prefix
           << "' ORDER BY NAME";
        int rc = sqlite3_exec((sqlite3 *)m_db, ss.str().c_str(),
                              store_values_callback, &values, &errormessage);
        if (rc != SQLITE_OK) {
            values.clear();
            std::string error(errormessage);
            sqlite3_free(errormessage);
            MGE_THROW(runtime_exception)
                << "Cannot read configuration values: " << error;
        }

        return;
    }

    void configuration_database::store_configuration_values(
        std::map<std::string, std::string> &values)
    {
        std::map<std::string, std::string>::const_iterator i = values.begin();
        while (i != values.end()) {
            store_value(i->first, i->second);
            ++i;
        }
    }

    void configuration_database::store_configuration_values(
        const char *prefix, std::map<std::string, std::string> &values)
    {
        std::map<std::string, std::string>::const_iterator i = values.begin();
        while (i != values.end()) {
            std::string key(prefix);
            key.append(".");
            key.append(i->first);
            store_value(key, i->second);
            ++i;
        }
    }

    void configuration_database::store_value(const std::string &key,
                                             const std::string &value)
    {
        std::stringstream ss;
        char *            errormessage = 0;
        ss << "INSERT OR REPLACE INTO CONFIGURATION (NAME, VALUE) VALUES('"
           << key << "', '" << value << "')";
        int rc = sqlite3_exec((sqlite3 *)m_db, ss.str().c_str(), NULL, NULL,
                              &errormessage);
        if (rc != SQLITE_OK) {
            std::string error(errormessage);
            sqlite3_free(errormessage);
            MGE_THROW(runtime_exception)
                << "Cannot store configuration value: " << error;
        }
    }

    void configuration_database::delete_key(const char *key)
    {
        std::stringstream ss;
        char *            errormessage = 0;
        ss << "DELETE FROM CONFIGURATION WHERE NAME='" << key << "'";
        int rc = sqlite3_exec((sqlite3 *)m_db, ss.str().c_str(), NULL, NULL,
                              &errormessage);
        if (rc != SQLITE_OK) {
            std::string error(errormessage);
            sqlite3_free(errormessage);
            MGE_THROW(runtime_exception)
                << "Cannot delete configuration key: " << error;
        }
    }

    void configuration_database::clear_configuration(const char *prefix)
    {
        int   rc           = 0;
        char *errormessage = 0;

        if (strlen(prefix)) {
            std::stringstream ss;
            ss << "DELETE FROM CONFIGURATION WHERE NAME LIKE '" << prefix
               << ".%'";
            rc = sqlite3_exec((sqlite3 *)m_db, ss.str().c_str(), NULL, NULL,
                              &errormessage);
        } else {
            rc = sqlite3_exec((sqlite3 *)m_db, "DELETE FROM CONFIGURATION",
                              NULL, NULL, &errormessage);
        }

        if (rc != SQLITE_OK) {
            std::string error(errormessage);
            sqlite3_free(errormessage);
            MGE_THROW(runtime_exception)
                << "Cannot clear configuration: " << error;
        }
    }
} // namespace mge
