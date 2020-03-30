// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include <atomic>

namespace mge {

    /**
     * @brief Thread safe sequence to count values.
     */
    template <typename T> class sequence : public noncopyable
    {
    public:
        /**
         * Constructor. Sequence counts from 0.
         */
        sequence() : m_value(0) {}

        /**
         * Constructor with start value-
         * @param value start value
         */
        sequence(T value) : m_value(value) {}

        /**
         * Get next value.
         * @return next value
         */
        T next() { return ++m_value; }

        /**
         * Get current value.
         * @return current value
         */
        T current() const { return m_value; }

    private:
        std::atomic<T> m_value;
    };
} // namespace mge
