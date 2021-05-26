// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/noncopyable.hpp"

#include <boost/iterator/iterator_facade.hpp>

#include <functional>
#include <map>
namespace mge {

    /**
     * @brief Callback map
     *
     * @tparam T function signature
     * @tparam K key type
     * @tparam C container type
     */
    template <typename T, typename K = unsigned int,
              typename C = std::map<typename K, std::function<typename T>>>
    class callback_map : public noncopyable
    {
    public:
        using self_type = callback_map<T, K, C>;

        using function_type  = std::function<T>;
        using key_type       = K;
        using container_type = C;

        class const_iterator
            : public boost::iterator_facade<self_type::const_iterator,
                                            const function_type,
                                            boost::forward_traversal_tag>
        {
        public:
            const_iterator(const typename container_type &m, bool end)
                : m_it(end ? m.cend() : m.cbegin())
            {}
            const_iterator(const const_iterator &) = default;
            const_iterator &operator=(const const_iterator &) = default;

        private:
            friend class boost::iterator_core_access;

            void increment() { ++m_it; }
            bool equal(const const_iterator &i) const { return m_it == i.m_it; }
            const function_type &dereference() const { return m_it->second; }

            typename container_type::const_iterator m_it;
        };

        callback_map() : m_sequence(0) {}

        /**
         * @brief Inserts a callback.
         *
         * @param f callback
         * @return key that can be used to erase the callback
         */
        key_type insert(const function_type &f)
        {
            key_type new_key = ++m_sequence;
            m_data[new_key]  = f;
            return new_key;
        }

        /**
         * @brief Erase callback using a registered key.
         *
         * @param k key
         * @return erased value, it is empty if the key
         *   was not found
         */
        function_type erase(key_type k)
        {
            auto it = m_data.find(k);
            if (it != m_data.end()) {
                auto result = it->second;
                m_data.erase(it);
                return result;
            } else {
                return function_type();
            }
        }
        /**
         * @brief Retrieve whether map is empty.
         *
         * @return @c true if empty
         */
        bool empty() const { return m_data.empty(); }

        const_iterator begin() const { return const_iterator(m_data, false); }
        const_iterator end() const { return const_iterator(m_data, true); }

    private:
        key_type       m_sequence;
        container_type m_data;
    };
} // namespace mge