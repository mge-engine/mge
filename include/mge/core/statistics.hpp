// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/overloaded.hpp"
#include <any>
#include <atomic>
#include <chrono>
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
        using value =
            std::variant<std::string, std::string_view, uint64_t, int64_t,
                         float, double, std::chrono::duration<int64_t>,
                         std::chrono::duration<double>>;

        using counter = std::atomic<uint64_t>;
        class MGECORE_EXPORT description
        {
        public:
            class MGECORE_EXPORT field_description
            {
            public:
                field_description();

                field_description(
                    std::string_view                           field_name,
                    std::function<statistics::value(void *)> &&getter);
                field_description(
                    std::string_view                                field_name,
                    const std::function<statistics::value(void *)> &getter);
                field_description(const field_description &desc);
                field_description(field_description &&desc);
                ~field_description();
                field_description &operator=(const field_description &desc);
                field_description &operator=(field_description &&desc);

                std::string_view name() const { return m_name; }

                template <typename S>
                inline statistics::value get(S &statistics_object) const
                {
                    void *raw_statistics =
                        reinterpret_cast<void *>(&statistics_object);
                    return m_getter(raw_statistics);
                }

            private:
                std::string_view                         m_name;
                std::function<statistics::value(void *)> m_getter;
            };

            using size_type = std::vector<field_description>::size_type;

            description(std::string_view name, std::string_view comment);

            description(std::string_view name, std::string_view comment,
                        std::initializer_list<field_description> fields);
            ~description();

            std::string_view name() const noexcept { return m_name; }
            std::string_view comment() const noexcept { return m_comment; }

            template <typename C>
            static field_description field(std::string_view    name,
                                           statistics::counter C::*p)
            {
                return field_description(name, [p](void *raw_class_p) {
                    C *class_p = reinterpret_cast<C *>(raw_class_p);

                    statistics::value r = (class_p->*p).load();

                    return r;
                });
            }

            const field_description &at(size_type index) const
            {
                return m_fields.at(index);
            }

            size_type size() const { return m_fields.size(); }

        private:
            std::string_view               m_name;
            std::string_view               m_comment;
            std::vector<field_description> m_fields;
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

        statistics(statistics &parent, std::string_view name)
            : m_name(name), m_owned(true)
        {
            parent.add_child(this);
        }

        statistics(std::string_view name) : m_name(name), m_owned(true)
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