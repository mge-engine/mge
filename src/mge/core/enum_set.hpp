// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/stdexceptions.hpp"
#include <bitset>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace mge {

    /**
     * @brief A bitset defined using an enum E and a maximum enum value
     * M.
     *
     * @tparam E enum type
     * @tparam M maximum value of enum
     */
    template <typename E, E M> class enum_set
    {
    private:
        using base_type = typename std::underlying_type<E>::type;

        static constexpr size_t bitset_size() { return (size_t)M; }

    public:
        /**
         * @brief Construct a new enum set object.
         */
        enum_set() = default;

        /**
         * @brief Copy constructor.
         *
         * @param s copied set
         */
        inline enum_set(const enum_set<E, M>& s)
            : m_bits(s.m_bits)
        {}

        /**
         * @brief Move constructor.
         *
         * @param s moved set
         */
        inline enum_set(enum_set<E, M>&& s)
            : m_bits(std::move(s.m_bits))
        {}

        /**
         * @brief Assignment.
         *
         * @param s assigned set
         * @return @c *this
         */
        inline enum_set<E, M>& operator=(const enum_set<E, M>& s)
        {
            m_bits = s.m_bits;
            return *this;
        }

        /**
         * @brief Set value of an enum value.
         *
         * @param value enum value
         */
        inline void set(E value) { m_bits.set(static_cast<base_type>(value)); }

        /**
         * @brief Resets value of an enum value.
         *
         * @param value enum value
         */
        inline void reset(E value)
        {
            m_bits.reset(static_cast<base_type>(value));
        }

        /**
         * @brief Flips value of an enum value.
         *
         * @param value enum value
         */
        inline void flip(E value)
        {
            m_bits.flip(static_cast<base_type>(value));
        }

        /**
         * @brief Test one enum value.
         *
         * @param value enum value
         * @return @c true if set
         */
        bool test(E value) const
        {
            return m_bits.test(static_cast<base_type>(value));
        }

        /**
         * @brief Get whether no value is set.
         *
         * @return @c true if no value set
         */
        bool empty() const { return m_bits.none(); }

        /**
         * @brief Resets values set in argument
         * @param modifiers value to be reset
         */
        void reset(const enum_set<E, M>& other)
        {
            for (uint32_t pos = 0; pos < other.m_bits.size(); ++pos) {
                if (other.m_bits.test(pos)) {
                    m_bits.reset(pos);
                }
            }
        }

        /**
         * @brief Return whether this set includes the other.
         * @param other other set
         * @return @c true if other is contained in this set
         */
        bool includes(const enum_set<E, M>& other) const
        {
            return (m_bits & other.m_bits) == other.m_bits;
        }

        /**
         * @brief Clears the whole set.
         */
        void reset() { m_bits.reset(); }

        /**
         * @brief Conversion to base bitset.
         *
         * @return underlying bitseta
         */
        operator const std::bitset<bitset_size()>&() const { return m_bits; }

        /**
         * @brief Conversion to base bitset.
         *
         * @return underlying bitset
         */
        operator std::bitset<bitset_size()>&() { return m_bits; }

        /**
         * @brief Combine this enum set with another.
         * @param other other set
         * @return this set combined (or combination)
         */
        enum_set<E, M>& operator|=(const enum_set<E, M>& other)
        {
            m_bits |= other.m_bits;
            return *this;
        }

    private:
        std::bitset<bitset_size()> m_bits;
    };

} // namespace mge
