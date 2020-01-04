// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/properties.hpp"

namespace mge {
    properties::properties()
    {}

    properties::properties(const properties& p)
        :m_data(p.m_data)
    {}

    properties::properties(properties&& p)
        :m_data(std::move(p.m_data))
    {}

    properties::properties(const input_stream_ref& input)
    {
        load(input->istream());
    }

    properties&
    properties::operator =(const properties& p)
    {
        m_data = p.m_data;
        return *this;
    }

    properties&
    properties::operator =(properties&& p)
    {
        m_data = std::move(p.m_data);
        return *this;
    }

    void
    properties::clear()
    {
        m_data.clear();
    }

    void
    properties::erase(const char *key)
    {
        m_data.erase(key);
    }

    void
    properties::erase(const std::string& key)
    {
        m_data.erase(key);
    }

    void properties::load(std::istream& s)
    {
        std::unordered_map<std::string, std::string> tmp_data;
        std::string line;
        while (!s.eof()) {
            std::getline(s, line);
            auto eqpos = line.find_first_of('=', 0);
            if (eqpos == std::string::npos) {
                MGE_THROW(runtime_exception) << "Invalid property file line '" << line << "' found";
            }
            std::string_view key(&line[0], eqpos);
            std::string_view value(&line[0] + eqpos + 1, line.size() - eqpos - 1);
            tmp_data[std::string(key.begin(), key.end())] = std::string(value.begin(), value.end());
        }
        std::swap(m_data, tmp_data);
        return;
    }

}
