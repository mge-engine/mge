// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/stdexceptions.hpp"

#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "boost/boost_lexical_cast.hpp"

namespace mge {

    /**
     * @brief Exception thrown when an option is specified that is unknown.
     */
    MGECORE_DECLARE_EXCEPTION_CLASS(unknown_option);

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

        class options
        {
        public:
            options() = default;
            options(const options&) = default;
            options(options&&) = default;
            options& operator=(const options&) = default;
            options& operator=(options&&) = default;
            ~options() = default;

            std::any& positional(const std::string& name)
            {
                return m_positional_options[name];
            }

        private:
            std::map<std::string, std::any, std::less<>> m_options;
            std::map<std::string, std::any, std::less<>> m_positional_options;
        };

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
                if (m_composing) {
                    if constexpr (std::is_same_v<T, bool>) {
                        return [](std::any& v, const std::string&) {
                            if (!v.has_value()) {
                                v = std::vector<bool>();
                            }
                            std::vector<bool>& values =
                                std::any_cast<std::vector<bool>&>(v);
                            values.push_back(true);
                        };
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        return [](std::any& v, const std::string& s) {
                            if (!v.has_value()) {
                                v = std::vector<std::string>();
                            }
                            std::vector<std::string>& values =
                                std::any_cast<std::vector<std::string>&>(v);
                            values.push_back(s);
                        };
                    }
                    return [](std::any& v, const std::string& s) {
                        if (!v.has_value()) {
                            v = std::vector<T>();
                        }
                        std::vector<T>& values =
                            std::any_cast<std::vector<T>&>(v);
                        values.push_back(boost::lexical_cast<T>(s));
                    };
                } else {
                    if constexpr (std::is_same_v<T, bool>) {
                        return nullptr;
                    } else if constexpr (std::is_same_v<T, std::string>) {
                        return [](std::any& v, const std::string& s) { v = s; };
                    }
                    return [](std::any& v, const std::string& s) {
                        v = boost::lexical_cast<T>(s);
                    };
                }
            }

            bool m_composing{false};
        };

        program_options() = default;
        ~program_options() = default;

        inline program_options& option(const char* name,
                                       const char* description)
        {
            option(name, description, m_options);
            return *this;
        }

        template <typename T>
        program_options& option(const char*                      name,
                                const char*                      description,
                                const program_options::value<T>& value)
        {
            option(name, description);
            auto& opt = m_options.back();
            opt.on_option_found = value.option_found();
            opt.composing = value.m_composing;
            return *this;
        }

        template <typename T>
        program_options& positional(const char* name,
                                    const char* description,
                                    const program_options::value<T>& value)
        {
            option(name, description, m_positional_options);
            auto& opt = m_positional_options.back();
            opt.on_option_found = value.option_found();
            opt.composing = value.m_composing;
            return *this;
        }

        program_options& stored_unrecognized()
        {
            store_unrecognized = true;
            return *this;
        }

        size_t size() const noexcept
        {
            return m_options.size();
        }

        void print(std::ostream& os) const;

        void parse(int argc, char* argv[], program_options::options& o);

    private:
        struct option_description
        {
            std::string           short_name;
            std::string           long_name;
            std::string           description;
            option_found_callback on_option_found;
            bool                  composing{false};

            const std::string& name() const
            {
                return long_name.empty() ? short_name : long_name;
            }
        };

        void option(const char*                      name,
                    const char*                      description,
                    std::vector<option_description>& options);

        std::vector<option_description> m_options;
        std::vector<option_description> m_positional_options;
        bool                            store_unrecognized{false};
    };

    inline std::ostream& operator<<(std::ostream& os, const program_options& po)
    {
        po.print(os);
        return os;
    }

} // namespace mge