// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/overloaded.hpp"
#include <string>
#include <variant>
namespace mge {
    /**
     * @brief A statistics entry.
     *
     * A statistics entry covers a statistics topic,
     * like the application, or a window, or a frame.
     *
     */
    class MGECORE_EXPORT statistics
    {
    public:
        template <size_t N>
        statistics(const char (&name)[N])
            : m_name(std::string_view(&name[0], &name[N - 1]))
        {}
        statistics(std::string &&name) : m_name(std::move(name)) {}
        std::string_view name() const
        {
            return std::visit(
                overloaded{[](const std::string_view &n) { return n; },
                           [](const std::string &n) {
                               return std::string_view(n.begin(), n.end());
                           }},
                m_name);
            /*
            std::visit(overloaded {
            [](auto arg) { std::cout << arg << ' '; },
            [](double arg) { std::cout << std::fixed << arg << ' '; },
            [](const std::string& arg) { std::cout << std::quoted(arg) << ' ';
        },
        }, v);
        */
        }

    public:
    protected:
    private:
        std::variant<std::string_view, std::string> m_name;
    };
} // namespace mge