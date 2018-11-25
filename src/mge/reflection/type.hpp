// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/module.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/type_definition.hpp"
#include "mge/reflection/parameter_source.hpp"
#include "mge/reflection/detail.hpp"
#include <string>
#include <map>
#include <typeindex>
#include <type_traits>
#include <boost/type_traits.hpp>

#include <iostream>

namespace mge {
    namespace reflection {

        template <typename T>
        class type
        {
        public:
            typedef type<T> self_type;

            type()
            {
                module& m = module::get<T>();
                m_definition = m.type<T>();
                if(!m_definition) {
                    m_definition = std::make_shared<type_definition>
                        (base_type_name<T>(),
                         std::type_index(typeid(T)),
                         detail::size_of_type<T>(),
                         boost::is_enum<T>::value);
                    m.type(m_definition);
                }
            }
            const std::string name() const { return m_definition->name(); }
            size_t size() const { return m_definition->size(); }

            MGE_ENABLE_MEMBER_IF(boost::is_enum<T>::value)
            self_type& enum_value(const char *name, T value)
            {
                m_definition->enum_value(name, (int64_t)value);
                return *this;
            }

            self_type& constructor()
            {
                auto cf = [](void *ptr, const parameter_source&) -> void {
                    new (ptr) T();
                };
                m_definition->constructor(signature(), cf);
                return *this;
            }

            template <typename... Args>
            self_type& constructor()
            {
                signature s({std::type_index(typeid(Args))...});
                auto cf = [](void *ptr, const parameter_source& src) -> void {
                    new (ptr) T(parameter<Args>(src)...);
                };
                m_definition->constructor(signature(), cf);
                return *this;
            }

        private:
            type_definition_ref m_definition;
        };

        template <>
        class type<void>
        {
        public:
            typedef type<void> self_type;

            type()
            {
                module& m = module::get<void>();
                m_definition = m.type<void>();
                if(!m_definition) {
                    m_definition = std::make_shared<type_definition>
                        (base_type_name<void>(),
                         std::type_index(typeid(void)),
                         detail::size_of_type<void>(),
                         false);
                    m.type(m_definition);
                }
            }

            const std::string name() const { return m_definition->name(); }
            size_t size() const { return m_definition->size(); }

        private:
            type_definition_ref m_definition;
        };

#if 0
        /**
         * A type.
         */
        class MGE_REFLECTION_EXPORT type
        {
        public:
            template <typename T> static type& get()
            {

                type_ref t = m.type<T>();
                if(!t) {
                    t = create<T>();
                    m.type(t);
                }
                return *t;
            }

            type(const std::string& name,
                 std::type_index index,
                 size_t size,
                 bool is_enum,
                 bool is_pod);
            ~type() = default;

            template <typename T>
            type& enum_value(const char *enum_name,
                             const T& enum_value)
            {
                static_assert(boost::is_enum<T>::value, "Type must be enum");
                if(!m_is_enum) {
                    MGE_THROW(mge::illegal_state(),
                              "Type '",
                              name(),
                              " is not an enum");
                }
                if(m_type_index != typeid(T)) {
                    MGE_THROW(mge::illegal_state(),
                              "Enum value is not of type ",
                              name());
                }
                m_enum_values[enum_name] = (int64_t)enum_value;
                return *this;
            }

            const std::string& name() const
            {
                return m_name;
            }

            void apply(visitor& v) const;
        private:
            template <typename T>
            static type_ref create()
            {
                auto t = std::make_shared<type>
                    (base_type_name<T>(),
                     std::type_index(typeid(T)),
                     sizeof(T),
                     boost::is_enum<T>::value,
                     boost::is_pod<T>::value);
                return t;
            }

            template <>
            static type_ref create<void>()
            {
                auto t = std::make_shared<type>
                    (base_type_name<void>(),
                     std::type_index(typeid(void)),
                     0,
                     boost::is_enum<void>::value,
                     boost::is_pod<void>::value);
                return t;
            }

            std::string m_name;
            std::type_index m_type_index;
            size_t m_size;
            bool m_is_enum;
            bool m_is_pod;
            std::map<std::string, int64_t> m_enum_values;
        };
#endif

}
}
