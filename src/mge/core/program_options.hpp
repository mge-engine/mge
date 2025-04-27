// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

#include <any>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "boost/boost_lexical_cast.hpp"

namespace mge {

    /**
     * @brief Program options.
     *
     * Description of command line options.
     */
    class MGECORE_EXPORT program_options
    {
    public:
        using option_found_callback =
            std::function<void(std::any&, const std::string&)>;

        template <typename T> struct value
        {
            using type = T;

            value() {}

            value<T>& composing()
            {
                m_composing = true;
                return *this;
            }

            option_found_callback option_found() const
            {
                static_assert(!std::is_same_v<T, void>,
                              "option_found() is not available for void type.");
                static_assert(
                    !std::is_same_v<T, std::any>,
                    "option_found() is not available for std::any type.");
                static_assert(!std::is_same_v<T, std::string_view>,
                              "option_found() is not available for "
                              "std::string_view type.");

                if constexpr (std::is_same_v<T, bool>) {
                    return nullptr;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    return [](std::any& v, const std::string& s) { v = s; };
                }

                return [](std::any& v, const std::string& s) {
                    v = boost::lexical_cast<T>(s);
                };
            }

            bool m_composing = false;
        };

        program_options() = default;
        ~program_options() = default;

        program_options& option(const char* name, const char* description);

        template <typename T>
        program_options& option(const char*                      name,
                                const char*                      description,
                                const program_options::value<T>& value)
        {
            option(name, description);
            auto& opt = m_options.back();
            opt.on_option_found = value.option_found();
            return *this;
        }

        size_t size() const noexcept
        {
            return m_options.size();
        }

        void print(std::ostream& os) const;

    private:
        struct option_description
        {
            std::string           short_name;
            std::string           long_name;
            std::string           description;
            option_found_callback on_option_found;
        };

        std::vector<option_description> m_options;
    };

    inline std::ostream& operator<<(std::ostream& os, const program_options& po)
    {
        po.print(os);
        return os;
    }

} // namespace mge