/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

#include "mge/core/dllexport.hpp"

#include <vector>
#include <string>
#include <iosfwd>

namespace mge {
    class MGE_CORE_EXPORT path
    {
    public:
        path();
        explicit path(const char *p);
        explicit path(const std::string& p);
        path(const path& p);
        path(const path& p, const char *f);
        path(const path& p, const std::string& f);
        path(path&& p);
        ~path();

        path& operator=(const path& p);
        path& operator /=(const char *path);
        path& operator /=(const std::string& path);
        path& operator /=(const path& path);

        bool operator ==(const std::string& p) const;
        bool operator ==(const char *p) const;
        bool operator ==(const path& p) const;
        bool operator !=(const std::string& p) const;
        bool operator !=(const char *p) const;
        bool operator !=(const path& p) const;

        std::string native_string() const;
        std::string generic_string() const;
        std::string extension() const;
        std::string file_name() const;

        bool absolute() const;
        bool cwd() const;
        bool prefix(const path& other) const;
     private:
        void construct(const std::string& p);
        void normalize();

        std::vector<std::string> m_elements;
        bool m_absolute;
        bool m_cwd;
    };

    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os,
                                              const path& p);
}


