// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
        /**
         * @brief Statistics value type.
         * Each statistics value is one of the supported types
         * - @c std::string
         * - @c std::string_view
         * - @c uint64_t
         * - @c int64_t
         * - @c float
         * - @c double
         * - @c std::chrono::duration<int64_t>
         * - @c std::chrono::duration<double>
         */
        using value_type = std::variant<std::string,
                                        std::string_view,
                                        uint64_t,
                                        int64_t,
                                        float,
                                        double,
                                        std::chrono::duration<int64_t>,
                                        std::chrono::duration<double>>;

        /**
         * @brief Type for counter.
         */
        using counter_type = std::atomic<uint64_t>;
        /**
         * @brief Description of statistics entry.
         */
        class MGECORE_EXPORT description
        {
        public:
            /**
             * @brief Description of statistics field.
             * A statistics field has a name and a getter mechanism.
             */
            class MGECORE_EXPORT field_description
            {
            public:
                /**
                 * Construct empty field description.
                 */
                field_description();

                /**
                 * Construct field description.
                 * @param field_name name
                 * @param getter functor to retrieve value
                 */
                field_description(
                    std::string_view                               field_name,
                    std::function<statistics::value_type(void*)>&& getter);
                /**
                 * Construct field description.
                 * @param field_name name
                 * @param getter functor to retrieve value
                 */
                field_description(
                    std::string_view field_name,
                    const std::function<statistics::value_type(void*)>& getter);
                /**
                 * Copy constructor.
                 * @param desc copied instance
                 */
                field_description(const field_description& desc);
                /**
                 * Move constructor.
                 * @param desc moved instance
                 */
                field_description(field_description&& desc);
                /**
                 * Destructor.
                 */
                ~field_description();
                /**
                 * @brief Assigment.
                 *
                 * @param desc assigned descriptor
                 * @return @c *this
                 */
                field_description& operator=(const field_description& desc);
                /**
                 * @brief Move assigment.
                 *
                 * @param desc assigned descriptor
                 * @return @c *this
                 */
                field_description& operator=(field_description&& desc);

                /**
                 * @brief Field name.
                 *
                 * @return name
                 */
                std::string_view name() const
                {
                    return m_name;
                }

                /**
                 * @brief Get value.
                 *
                 * @tparam S statistics object type
                 * @param statistics_object statistics object used in getter
                 * @return  statistics field value
                 */
                template <typename S>
                inline statistics::value_type get(S& statistics_object) const
                {
                    void* raw_statistics =
                        reinterpret_cast<void*>(&statistics_object);
                    return m_getter(raw_statistics);
                }

            private:
                std::string_view                             m_name;
                std::function<statistics::value_type(void*)> m_getter;
            };

            /**
             * @brief Size type for number of fields.
             */
            using size_type = std::vector<field_description>::size_type;

            /**
             * Construct statistics description.
             *
             * @param name      statistics name
             * @param comment   statistics comment
             */
            description(std::string_view name, std::string_view comment);

            /**
             * Construct statistics description.
             *
             * @param name      statistics name
             * @param comment   statistics comment
             * @param fields    list of statistics field descriptions
             */
            description(std::string_view                         name,
                        std::string_view                         comment,
                        std::initializer_list<field_description> fields);
            /**
             * @brief Destructor.
             */
            ~description();

            /**
             * @brief Statistics name.
             *
             * @return name
             */
            std::string_view name() const noexcept
            {
                return m_name;
            }
            /**
             * @brief Statistics comment.
             *
             * @return comment
             */
            std::string_view comment() const noexcept
            {
                return m_comment;
            }

            /**
             * @brief Create field descriptor for counter.
             *
             * @tparam C statistics object class
             * @param name field name
             * @param p pointer to member of @c C
             * @return field description
             */
            template <typename C>
            static field_description field(std::string_view         name,
                                           statistics::counter_type C::*p)
            {
                return field_description(name, [p](void* raw_class_p) {
                    C* class_p = reinterpret_cast<C*>(raw_class_p);

                    statistics::value_type r = (class_p->*p).load();

                    return r;
                });
            }

            /**
             * @brief Access field description.
             *
             * @param index field index
             * @return field descriptor
             */
            const field_description& at(size_type index) const
            {
                return m_fields.at(index);
            }

            /**
             * @brief Number of fields.
             *
             * @return number of fields
             */
            size_type size() const
            {
                return m_fields.size();
            }

        private:
            std::string_view               m_name;
            std::string_view               m_comment;
            std::vector<field_description> m_fields;
        };

        /**
         * @brief Construct a new statistics object.
         *
         * @tparam N length of name
         * @param parent parent statistics
         * @param name statistics name
         */
        template <size_t N>
        statistics(statistics& parent, const char (&name)[N])
            : m_name(std::string_view(&name[0], &name[N - 1]))
            , m_owned(true)
        {
            parent.add_child(this);
        }

        /**
         * @brief Construct a new top level statistics object.
         *
         * @tparam N length of name
         * @param name statistics name
         */
        template <size_t N>
        statistics(const char (&name)[N])
            : m_name(std::string_view(&name[0], &name[N - 1]))
            , m_owned(true)
        {
            root().add_child(this);
        }

        /**
         * @brief Construct a new statistics object.
         *
         * @param parent parent statistics
         * @param name statistics name
         */
        statistics(statistics& parent, std::string_view name)
            : m_name(name)
            , m_owned(true)
        {
            parent.add_child(this);
        }

        /**
         * @brief Construct a new statistics object.
         *
         * @param name statistics name
         */
        statistics(std::string_view name)
            : m_name(name)
            , m_owned(true)
        {
            root().add_child(this);
        }

        /**
         * @brief Construct a new statistics object.
         *
         * @param name statistics name
         */
        statistics(std::string&& name)
            : m_name(std::move(name))
            , m_owned(true)
        {
            root().add_child(this);
        }

        /**
         * @brief Construct a new statistics object.
         *
         * @param parent parent statistics
         * @param name statistics name
         */
        statistics(statistics& parent, std::string&& name)
            : m_name(std::move(name))
            , m_owned(true)
        {
            parent.add_child(this);
        }

        /**
         * @brief Construct a new statistics object.
         *
         * @param name statistics name
         */
        statistics(const std::string& name)
            : m_name(name)
            , m_owned(true)
        {
            root().add_child(this);
        }

        /**
         * @brief Construct a new statistics object.
         *
         * @param parent parent statistics
         * @param name statistics name
         */
        statistics(statistics& parent, const std::string& name)
            : m_name(name)
            , m_owned(true)
        {
            parent.add_child(this);
        }

        virtual ~statistics();

        /**
         * @brief Statistics name.
         *
         * @return name
         */
        std::string_view name() const
        {
            return std::visit(
                overloaded{[](const std::string_view& n) { return n; },
                           [](const std::string& n) {
                               return std::string_view(n.begin(), n.end());
                           }},
                m_name);
        }

        /**
         * @brief Root statistics node.
         *
         * @return root node of statistics
         */
        static statistics& root();

        /**
         * Release ownership. Statistics object can be destroyed.
         */
        void release();

        /**
         * @brief Describes fields of this statistics record.
         *
         * @return element and field description
         */
        virtual const description& describe() const;

    protected:
        statistics();

    private:
        void add_child(statistics* s);

        std::variant<std::string_view, std::string> m_name;
        std::vector<statistics*>                    m_children;
        bool                                        m_owned;
    };

} // namespace mge