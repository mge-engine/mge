// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/visitor.hpp"
#include "mge/core/stdexceptions.hpp"

#include <string>
#include <map>
#include <typeindex>
#include <type_traits>
#include <boost/type_traits.hpp>

#include <iostream>

namespace mge {
    namespace reflection {

        /**
         * A type.
         */
        class MGE_REFLECTION_EXPORT type
        {
        public:
            template <typename T> static type& get()
            {
                module& m = module::get<T>();
                type_ref t = m.type<T>();
                if(!t) {
                    t = create<T>();
                    m.type(t);
                }
                return *t;
            }

            type(const std::string& name,
                 std::type_index index,
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
                     boost::is_enum<T>::value,
                     boost::is_pod<T>::value);
                return t;
            }

            std::string m_name;
            std::type_index m_type_index;
            bool m_is_enum;
            bool m_is_pod;
            std::map<std::string, int64_t> m_enum_values;
        };

}
}
