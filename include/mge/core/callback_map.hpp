// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dllexport.hpp"
#include "mge/core/noncopyable.hpp"

#include <functional>
#include <map>
namespace mge {

    /**
     * @brief Callback map
     *
     * @tparam T function signature
     * @tparam K key type
     * @tparam C container typpe
     */
    template <typename T, typename K = unsigned int,
              typename C = std::map<typename K, std::function<typename T>>>
    class callback_map : public noncopyable
    {
    public:
        using function_type  = std::function<T>;
        using key_type       = K;
        using container_type = C;

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

    private:
        key_type       m_sequence;
        container_type m_data;
    };
} // namespace mge