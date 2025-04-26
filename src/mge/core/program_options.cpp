// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/program_options.hpp"
#include "mge/core/stdexceptions.hpp"

#include <iomanip>

namespace mge {

    program_options& program_options::option(const char* name,
                                             const char* description)
    {
        const char* commapos = strchr(name, ',');

        if (commapos == nullptr) {
            auto namelen = strlen(name);
            if (namelen == 0) {
                MGE_THROW(illegal_argument) << "Empty option name";
            } else if (namelen == 1) {
                m_options.emplace_back(name, "", description);
            } else {
                m_options.emplace_back("", name, description);
            }
        } else {
            std::string name1(name, commapos);
            std::string name2(commapos + 1);
            if (name1.empty() || name2.empty()) {
                MGE_THROW(illegal_argument) << "Invalid option name: " << name;
            }
            if (name1.length() == 1 && name2.length() > 1) {
                m_options.emplace_back(name1, name2, description);
            } else if (name2.length() == 1 && name1.length() > 1) {
                m_options.emplace_back(name2, name1, description);
            } else {
                MGE_THROW(illegal_argument) << "Invalid option name: " << name;
            }
        }

        return *this;
    }

    void program_options::print(std::ostream& os) const
    {
        size_t len = 0;
        for (const auto& opt : m_options) {
            size_t thislen = 0;
            if (opt.short_name.length() == 0) {
                thislen = 2 + opt.long_name.length() + 2;
            } else if (opt.long_name.length() == 0) {
                thislen = 2 + opt.short_name.length() + 1;
            } else {
                thislen = 2 + 1 + opt.short_name.length() + 5 +
                          opt.long_name.length() + 2;
            }
            len = std::max(len, thislen);
        }
        for (const auto& opt : m_options) {
            if (opt.short_name.length() == 0) {
                os << "  --" << opt.long_name << std::left
                   << std::setw(len - (opt.long_name.length() + 4) + 2) << ""
                   << opt.description << std::endl;
            } else if (opt.long_name.length() == 0) {
                os << "  -" << opt.short_name << std::left
                   << std::setw(len - (opt.short_name.length() + 3) + 2) << ""
                   << opt.description << std::endl;

            } else {
                os << "  -" << opt.short_name << " [ --" << opt.long_name
                   << " ]" << std::left
                   << std::setw(len -
                                (3 + opt.short_name.length() + 5 +
                                 opt.long_name.length() + 2) +
                                2)
                   << "" << opt.description << std::endl;
            }
        }
    }
} // namespace mge
