// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/program_options.hpp"
#include "mge/core/stdexceptions.hpp"

#include <iomanip>

namespace mge {

    MGE_DEFINE_EXCEPTION_CLASS(unknown_option);

    void program_options::option(
        const char*                                       name,
        const char*                                       description,
        std::vector<program_options::option_description>& options)
    {
        const char* commapos = strchr(name, ',');

        if (commapos == nullptr) {
            auto namelen = strlen(name);
            if (namelen == 0) {
                MGE_THROW(illegal_argument) << "Empty option name";
            } else if (namelen == 1) {
                options.emplace_back(name, "", description);
            } else {
                options.emplace_back("", name, description);
            }
        } else {
            std::string name1(name, commapos);
            std::string name2(commapos + 1);
            if (name1.empty() || name2.empty()) {
                MGE_THROW(illegal_argument) << "Invalid option name: " << name;
            }
            if (name1.length() == 1 && name2.length() > 1) {
                options.emplace_back(name1, name2, description);
            } else if (name2.length() == 1 && name1.length() > 1) {
                options.emplace_back(name2, name1, description);
            } else {
                MGE_THROW(illegal_argument) << "Invalid option name: " << name;
            }
        }

        return;
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
            if (opt.on_option_found) {
                thislen += 6; // "<arg> "
            }
            len = std::max(len, thislen);
        }
        for (const auto& opt : m_options) {
            if (opt.short_name.length() == 0) {
                os << "  --" << opt.long_name
                   << (opt.on_option_found ? " <arg>" : "") << std::left
                   << std::setw(len -
                                (opt.long_name.length() + 4 +
                                 (opt.on_option_found ? 6 : 0)) +
                                2)
                   << "" << opt.description << std::endl;
            } else if (opt.long_name.length() == 0) {
                os << "  -" << opt.short_name
                   << (opt.on_option_found ? " <arg>" : "") << std::left
                   << std::setw(len - (opt.short_name.length() + 3) + 2 +
                                (opt.on_option_found ? 6 : 0))
                   << "" << opt.description << std::endl;

            } else {
                os << "  -" << opt.short_name << " [ --" << opt.long_name
                   << " ]" << (opt.on_option_found ? " <arg>" : "") << std::left
                   << std::setw(len -
                                (3 + opt.short_name.length() + 5 +
                                 opt.long_name.length() + 2 +
                                 (opt.on_option_found ? 6 : 0)) +
                                2)
                   << "" << opt.description << std::endl;
            }
        }
    }

    void
    program_options::parse(int argc, char* argv[], program_options::options& o)
    {
        auto current_positional = m_positional_options.begin();

        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg.starts_with("--")) {
                arg = arg.substr(2);
            } else if (arg.starts_with("-")) {
                arg = arg.substr(1);
            } else {
                if (current_positional == m_positional_options.end()) {
                    MGE_THROW(unknown_option)
                        << "Unexpected positional option: " << arg;
                }
                auto& opt = *current_positional;
                auto& value = o.positional(opt.name());
                if (opt.on_option_found) {
                    opt.on_option_found(value, arg);
                }
                if (!opt.composing) {
                    ++current_positional;
                }
                continue;
            }
        }
    }
} // namespace mge