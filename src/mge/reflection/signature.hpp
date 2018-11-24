#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include <vector>
#include <initializer_list>
#include <typeindex>
#include <iostream>

namespace mge {
    namespace reflection {

        class MGE_REFLECTION_EXPORT signature

        {
        private:
            typedef std::vector<std::type_index> container_type;
        public:
            typedef container_type::const_iterator const_iterator;
            typedef container_type::iterator iterator;
            typedef container_type::value_type value_type;
            typedef container_type::const_pointer const_pointer;

            signature() = default;
            signature(const signature& s);
            signature(signature&& s);
            signature(const std::initializer_list<std::type_index>& elememts);
            ~signature() = default;

            signature& operator=(const signature& s);
            signature& operator=(signature&& s);

            auto begin() const
            {
                return m_types.begin();
            }

            auto end() const
            {
                return m_types.end();
            }

            auto cbegin() const
            {
                return m_types.cbegin();
            }

            auto cend() const
            {
                return m_types.cend();
            }
        private:
            container_type m_types;
        };

        MGE_REFLECTION_EXPORT
        std::ostream& operator <<(std::ostream& os, const signature& s);
    }
}
