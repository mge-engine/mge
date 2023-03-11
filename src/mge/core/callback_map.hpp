// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/noncopyable.hpp"
#include "mge/core/stdexceptions.hpp"

#include <boost/iterator/iterator_facade.hpp>

#include <cstdint>
#include <functional>
#include <map>
#include <vector>

namespace mge {

    /**
     * @brief Callback map
     *
     * @tparam Args function arguments
     */
    template <typename... Args> class callback_map : public noncopyable
    {
    public:
        using self_type = callback_map<Args...>;

        using function_type = std::function<void(Args...)>;
        using value_type = function_type;
        using key_type = uint32_t;
        using container_type = std::map<key_type, function_type>;
        using key_container_type = std::vector<key_type>;

        callback_map()
            : m_sequence(0)
            , m_calling(false)
        {}

        /**
         * @brief Inserts a callback.
         *
         * @param f callback
         * @return key that can be used to erase the callback
         */
        key_type insert(const function_type& f)
        {
            if (m_calling) {
                MGE_THROW(illegal_state)
                    << "Cannot insert new callback during callback processing";
            }
            key_type new_key = ++m_sequence;
            m_data[new_key] = f;
            return new_key;
        }

        /**
         * @brief Erase callback using a registered key.
         *
         * @param k key
         * @return erased value, it is empty if the key
         *   was not found
         */
        void erase(key_type k)
        {
            if (m_calling) {
                m_delayed_erase.emplace_back(k);
            } else {
                auto it = m_data.find(k);
                if (it != m_data.end()) {
                    m_data.erase(it);
                }
            }
        }

        void operator()(Args... args) const
        {
            m_calling = true;
            try {
                for (const auto& [key, value] : m_data) {
                    value(args...);
                }
                m_calling = false;
                for (const auto& k : m_delayed_erase) {
                    const_cast<self_type*>(this)->erase(k);
                }
                m_delayed_erase.clear();
            } catch (...) {
                m_calling = false;
                try {
                    for (const auto& k : m_delayed_erase) {
                        const_cast<self_type*>(this)->erase(k);
                    }
                    m_delayed_erase.clear();
                } catch (...) {
                    // prevent double exception, but just drop this one
                }
                throw;
            }
        }

        void clear() { m_data.clear(); }

    private:
        key_type                   m_sequence;
        container_type             m_data;
        mutable key_container_type m_delayed_erase;
        mutable bool               m_calling;
    };

    template <> class callback_map<void> : public noncopyable
    {
    public:
        using self_type = callback_map<void>;

        using function_type = std::function<void(void)>;
        using value_type = function_type;
        using key_type = uint32_t;
        using container_type = std::map<key_type, function_type>;
        using key_container_type = std::vector<key_type>;

        callback_map()
            : m_sequence(0)
            , m_calling(false)
        {}

        /**
         * @brief Inserts a callback.
         *
         * @param f callback
         * @return key that can be used to erase the callback
         */
        key_type insert(const function_type& f)
        {
            if (m_calling) {
                MGE_THROW(illegal_state)
                    << "Cannot insert new callback during callback processing";
            }
            key_type new_key = ++m_sequence;
            m_data[new_key] = f;
            return new_key;
        }

        /**
         * @brief Erase callback using a registered key.
         *
         * @param k key
         * @return erased value, it is empty if the key
         *   was not found
         */
        void erase(key_type k)
        {
            if (m_calling) {
                m_delayed_erase.emplace_back(k);
            } else {
                auto it = m_data.find(k);
                if (it != m_data.end()) {
                    m_data.erase(it);
                }
            }
        }

        void operator()(void) const
        {
            m_calling = true;
            try {
                for (const auto& [key, value] : m_data) {
                    value();
                }
                m_calling = false;
                for (const auto& k : m_delayed_erase) {
                    const_cast<self_type*>(this)->erase(k);
                }
                m_delayed_erase.clear();
            } catch (...) {
                m_calling = false;
                try {
                    for (const auto& k : m_delayed_erase) {
                        const_cast<self_type*>(this)->erase(k);
                    }
                    m_delayed_erase.clear();
                } catch (...) {
                    // prevent double exception, but just drop this one
                }
                throw;
            }
        }

        void clear() { m_data.clear(); }

    private:
        key_type                   m_sequence;
        container_type             m_data;
        mutable key_container_type m_delayed_erase;
        mutable bool               m_calling;
    };
} // namespace mge
