// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/program_options.hpp"
#include "mge/core/stdexceptions.hpp"

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
} // namespace mge
