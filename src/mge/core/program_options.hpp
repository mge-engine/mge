// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

#include <any>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace mge {

    /**
     * @brief Program options.
     *
     * Description of command line options.
     */
    class MGECORE_EXPORT program_options
    {
    public:
        template <typename T> struct value
        {
            using type = T;
            using option_found_callback =
                std::function<void(std::any&, const std::string&)>;

            value() {}

            value<T>& composing()
            {
                m_composing = true;
                return *this;
            }

            option_found_callback option_found()
            {
                return nullptr;
            }

            bool m_composing = false;
        };

        program_options() = default;
        ~program_options() = default;

        program_options& option(const char* name, const char* description);

        template <typename T>
        program_options& option(const char*                      name,
                                const char*                      description,
                                const program_options::value<T>& value);

        size_t size() const noexcept
        {
            return m_options.size();
        }

        void print(std::ostream& os) const;

    private:
        struct option_description
        {
            std::string short_name;
            std::string long_name;
            std::string description;
        };

        std::vector<option_description> m_options;
    };

    inline std::ostream& operator<<(std::ostream& os, const program_options& po)
    {
        po.print(os);
        return os;
    }

} // namespace mge