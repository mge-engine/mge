// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/path.hpp"
#include "mge/config.hpp"
#include <boost/algorithm/string.hpp>

namespace mge {
    path::path()
    :m_absolute(false)
    ,m_cwd(false)
    {}

    path::path(const char *p)
    :m_absolute(false),
     m_cwd(false)
    {
        construct(std::string(p));
        normalize();
    }

    path::path(const std::initializer_list<const char *>& elements)
        :m_absolute(false)
        ,m_cwd(false)
    {
        auto it = elements.begin();
        construct(std::string(*it));
        ++it;
        while(it != elements.end()) {
            operator /=(*it);
            normalize();
            ++it;
        }
    }

    void path::construct(const std::string& p)
    {
        boost::split(m_elements, p, boost::is_any_of("/\\"));
#ifdef MGE_OS_WINDOWS
        if (*m_elements[0].rbegin()==':') {
            m_absolute=true;
        }
#else
        m_absolute = p[0] == '/';
#endif
    }

    void path::normalize()
    {
        std::vector<std::string> new_elements;
        for (auto s : m_elements) {
            if (!s.empty()) {
                new_elements.push_back(s);
            }
         }
         m_elements = new_elements;
         m_cwd = m_elements.size() == 1
                 && m_elements[0].length() == 1
                 && m_elements[0][0] == '.';
    }

    path::path(const std::string& p)
    :m_absolute(false)
    ,m_cwd(false)
    {
        construct(p);
        normalize();
    }

    path::path(path&& p)
    :m_elements(std::move(p.m_elements))
     ,m_absolute(std::move(p.m_absolute))
     ,m_cwd(std::move(p.m_cwd))
    {}

    path::path(const path& p)
    :m_elements(p.m_elements)
    ,m_absolute(p.m_absolute)
    ,m_cwd(p.m_cwd)
    {}

    path::path(const path& p, const char *f)
    :m_elements(p.m_elements)
    ,m_absolute(p.m_absolute)
    ,m_cwd(false)
    {
        operator /=(f);
    }

    path::path(const path& p, const std::string& f)
        :m_elements(p.m_elements)
        ,m_absolute(p.m_absolute)
        ,m_cwd(false)
    {
        operator /=(f);
    }

    path::~path()
    {}

    path&
    path::operator /=(const path& p)
    {
        if(&p == this) {
            path pcopy(p);
            operator /= (pcopy);
        } else {
            for(const auto& s : p.m_elements) {
                m_elements.push_back(s);
            }
        }
        m_cwd = false;
        return *this;
    }


    path&
    path::operator /=(const std::string& p)
    {
        return operator /=(path(p));
    }

    path&
    path::operator /=(const char * p)
    {
        return operator /=(path(p));
    }

    std::string
    path::native_string() const
    {
        std::string str;
#ifdef MGE_OS_WINDOWS
        str = boost::join(m_elements, "\\");
#else
        if (m_absolute) {
            str += "/";
        }
        str = boost::join(m_elements, "/");
#endif
        return str;
    }


    std::string
    path::generic_string() const
    {
        std::string str;
#ifndef MGE_OS_WINDOWS
        if (m_absolute) {
            str += "/";
        }
#endif
        str =  boost::join(m_elements, "/");
        return str;
    }

    bool
    path::absolute() const
    {
        return m_absolute;
    }

    bool
    path::cwd() const
    {
        return m_cwd;
    }

    std::string
    path::extension() const
    {
        if (!m_elements.empty()) {
            const char *dot = strrchr(m_elements.rbegin()->c_str(),
                                      '.');
            if(dot != nullptr) {
                return std::string(dot+1);
            }
        }
        return "";
    }

    std::string
    path::file_name() const
    {
        if (m_elements.empty()) {
            return "";
        } else {
            return *m_elements.rbegin();
        }
    }

    path&
    path::operator =(const path& p)
    {
        m_elements = p.m_elements;
        m_absolute = p.m_absolute;
        m_cwd = p.m_cwd;
        return *this;
    }

    std::ostream&
    operator <<(std::ostream& os, const path& p)
    {
        return os << p.generic_string();
    }

    bool
    path::operator ==(const std::string& p) const
    {
        path cmp_p(p);
        return operator ==(cmp_p);
    }

    bool
    path::operator ==(const char * p) const
    {
        path cmp_p(p);
        return operator ==(cmp_p);
    }

    bool
    path::operator ==(const path& p) const
    {
        return m_elements == p.m_elements;
    }

    bool
    path::operator !=(const std::string& p) const
    {
        path cmp_p(p);
        return !operator ==(cmp_p);
    }

    bool
    path::operator !=(const char * p) const
    {
        path cmp_p(p);
        return !operator ==(cmp_p);
    }

    bool
    path::operator !=(const path& p) const
    {
        return m_elements != p.m_elements;
    }

    bool
    path::prefix(const path& other) const
    {
        if(m_elements.size() <= other.m_elements.size()) {
            for(size_t i=0; i<m_elements.size(); ++i) {
                if(m_elements[i] != other.m_elements[i]) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }

    }

}
