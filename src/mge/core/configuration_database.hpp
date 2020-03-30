// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/singleton.hpp"

#include <map>
#include <string>

namespace mge {
    class configuration_database
    {
    public:
        using value_map = std::map<std::string, std::string>;

        configuration_database();
        ~configuration_database();
        static configuration_database &instance();
        void read_configuration_values(value_map &values);
        void read_configuration_values(const char *prefix, value_map &values);
        void store_configuration_values(value_map &values);
        void store_configuration_values(const char *prefix, value_map &values);
        void clear_configuration(const char *prefix);
        void delete_key(const char *key);

    private:
        static std::string get_configuration_directory();
        void               open_database();
        void               close_database();
        void               check_schema();
        bool               schema_exists();
        void               create_schema();
        void store_value(const std::string &key, const std::string &value);

        void *m_db;
    };
} // namespace mge
