// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <atomic>
#include <functional>

namespace mge {

    /**
     * @brief A sequenced object having a sequence number.
     *
     * @tparam T The type that is sequenced here, different types have different
     * numbers.
     */
    template <typename T> class sequenced
    {
    public:
        using sequence_type = uint64_t;
        sequenced()
            : m_sequence(++sequence_instance())
        {}

        sequenced(const sequenced&) noexcept = default;
        sequenced& operator=(const sequenced&) noexcept = default;

        sequenced(sequenced&& other) noexcept
            : m_sequence(other.m_sequence)
        {
            other.m_sequence = 0;
        }

        sequenced& operator=(sequenced&& other) noexcept
        {
            m_sequence = other.m_sequence;
            other.m_sequence = 0;
            return *this;
        }

        sequence_type sequence() const noexcept
        {
            return m_sequence;
        }

        bool operator<(const sequenced& other) const noexcept
        {
            return m_sequence < other.m_sequence;
        }

        bool operator<=(const sequenced& other) const noexcept
        {
            return m_sequence <= other.m_sequence;
        }

        bool operator>(const sequenced& other) const noexcept
        {
            return m_sequence > other.m_sequence;
        }

        bool operator>=(const sequenced& other) const noexcept
        {
            return m_sequence >= other.m_sequence;
        }

        bool operator==(const sequenced& other) const noexcept
        {
            return m_sequence == other.m_sequence;
        }

        bool operator!=(const sequenced& other) const noexcept
        {
            return m_sequence != other.m_sequence;
        }

        ~sequenced() = default;

        constexpr explicit operator bool() const noexcept
        {
            return m_sequence != 0;
        }

        constexpr bool has_value() const noexcept
        {
            return m_sequence != 0;
        }

    private:
        sequence_type m_sequence;

        static std::atomic<sequence_type>& sequence_instance()
        {
            static std::atomic<sequence_type> s;
            return s;
        }
    };
} // namespace mge
namespace std {
    template <typename T> struct hash<mge::sequenced<T>>
    {
        std::size_t operator()(const mge::sequenced<T>& s) const noexcept
        {
            return std::hash<typename mge::sequenced<T>::sequence_type>()(
                s.sequence());
        }
    };

} // namespace std