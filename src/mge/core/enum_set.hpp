#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/stdexceptions.hpp"
#include <bitset>
#include <iterator>
#include <iostream>
#include <type_traits>

namespace mge {

    /**
     * A bitset defined using an enum E and a maximum enum value
     * M.
     */
    template <typename E, size_t M>
    class enum_set
    {
    private:
        typedef typename std::underlying_type<E>::type base_type;
    public:
        inline enum_set()
        {}

        inline enum_set(const enum_set<E, M>& s)
            :m_bits(s.m_bits())
        {}

        inline enum_set(enum_set<E,M>&& s)
            :m_bits(std::move(s))
        {}

        inline enum_set<E,M>& operator =(const enum_set<E,M>& s)
        {
            m_bits = s.m_bits;
            return *this;
        }

        inline void set(E value)
        {
            m_bits.set(static_cast<base_type>(value));
        }

        bool test(E value) const
        {
             return m_bits.test(static_cast<base_type>(value));
        }
    private:
        std::bitset<M> m_bits;
    };


}
