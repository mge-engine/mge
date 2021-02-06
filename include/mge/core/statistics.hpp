// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/overloaded.hpp"
#include <functional>
#include <string>
#include <variant>
#include <vector>

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
        class description
        {
        public:
            description(std::string_view name, std::string_view comment);
            ~description();

            std::string_view name() const noexcept { return m_name; }
            std::string_view comment() const noexcept { return m_comment; }

        private:
            std::string_view m_name;
            std::string_view m_comment;
        };

        template <size_t N>
        statistics(statistics &parent, const char (&name)[N])
            : m_name(std::string_view(&name[0], &name[N - 1])), m_owned(true)
        {
            parent.add_child(this);
        }

        template <size_t N>
        statistics(const char (&name)[N])
            : m_name(std::string_view(&name[0], &name[N - 1])), m_owned(true)
        {
            root().add_child(this);
        }

        statistics(std::string &&name) : m_name(std::move(name)), m_owned(true)
        {
            root().add_child(this);
        }

        statistics(statistics &parent, std::string &&name)
            : m_name(std::move(name)), m_owned(true)
        {
            parent.add_child(this);
        }

        statistics(const std::string &name) : m_name(name), m_owned(true)
        {
            root().add_child(this);
        }

        statistics(statistics &parent, const std::string &name)
            : m_name(name), m_owned(true)
        {
            parent.add_child(this);
        }

        virtual ~statistics();

        std::string_view name() const
        {
            return std::visit(
                overloaded{[](const std::string_view &n) { return n; },
                           [](const std::string &n) {
                               return std::string_view(n.begin(), n.end());
                           }},
                m_name);
        }

        /**
         * @brief Root statistics node.
         *
         * @return root node of statistics
         */
        static statistics &root();

        /**
         * Release ownership. Statistics object can be destroyed.
         */
        void release();

        /**
         * @brief Describes fields of this statistics record.
         *
         * @return element and field description
         */
        virtual const description &describe() const;

    protected:
        statistics();

    private:
        void add_child(statistics *s);

        std::variant<std::string_view, std::string> m_name;
        std::vector<statistics *>                   m_children;
        bool                                        m_owned;
    };

} // namespace mge