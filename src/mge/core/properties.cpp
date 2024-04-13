// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/properties.hpp"
#include "mge/config.hpp"

namespace mge {
    properties::properties() {}

    properties::properties(const properties& p)
        : m_data(p.m_data)
    {}

    properties::properties(properties&& p)
        : m_data(std::move(p.m_data))
    {}

    properties::properties(const input_stream_ref& input)
    {
        load(input->istream());
    }

    properties::properties(std::istream& s) { load(s); }

    properties& properties::operator=(const properties& p)
    {
        m_data = p.m_data;
        return *this;
    }

    properties& properties::operator=(properties&& p)
    {
        m_data = std::move(p.m_data);
        return *this;
    }

    void properties::clear() { m_data.clear(); }

    void properties::erase(std::string_view key)
    {
        m_data.erase(std::string(key.begin(), key.end()));
    }

    void properties::erase(const std::string& key) { m_data.erase(key); }

    void properties::load(std::istream& s)
    {
        map_type tmp_data;

        std::string line;

        while (!s.eof()) {
            std::getline(s, line);
            // skip completely empty lines
            if (line.empty()) {
                continue;
            }
            auto eqpos = line.find_first_of('=', 0);
            if (eqpos == std::string::npos) {
                MGE_THROW(runtime_exception)
                    << "Invalid property file line '" << line << "' found";
            }
            std::string_view key(&line[0], eqpos);
            std::string_view value(&line[0] + eqpos + 1,
                                   line.size() - eqpos - 1);
            auto             value_end = value.end();
// strip spurious \r
#ifdef MGE_OS_WINDOWS
            if (!value.empty()) {
                if (*value.rbegin() == '\r') {
                    --value_end;
                }
            }
#endif
            tmp_data[std::string(key.begin(), key.end())] =
                std::string(value.begin(), value_end);
        }
        std::swap(m_data, tmp_data);
        return;
    }

} // namespace mge
