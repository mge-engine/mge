// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_operators.hpp"
#include "mge/core/small_vector.hpp"
#include "mge/script/dllexport.hpp"

#include <array>
#include <typeindex>

namespace mge::script {

    /**
     * Signature of a function.
     */
    class MGESCRIPT_EXPORT signature
        : public boost::equality_comparable<signature>
    {
    public:
        signature();

        template <typename... T> static signature create()
        {
            std::array<std::type_index, sizeof...(T)> arg_types = {
                std::type_index(typeid(T))...};
            return signature(arg_types);
        }

        template <size_t I>
        signature(const std::array<std::type_index, I>& data)
        {
            m_types.resize(data.size());
            std::copy(data.begin(), data.end(), m_types.begin());
        }

        signature(const signature&) = default;
        signature(signature&&) = default;

        signature& operator=(const signature&) = default;
        signature& operator=(signature&&) = default;

        ~signature();

        bool operator==(const signature& s) const;

        const auto& operator[](size_t i) const { return *m_types[i]; }

        const auto& at(size_t i) const { return *m_types.at(i); }

        bool matches(const signature& other) const;

        bool empty() const;

        auto size() const { return m_types.size(); }

        /**
         * @brief Returns the empty signature.
         *
         * @return signature with no elements
         */
        static const signature& empty_signature();

        /**
         * Provides gist of contained information.
         * @param context format context
         */
        void gist(std::format_context& context) const;

    private:
        small_vector<std::optional<std::type_index>, 3> m_types;
    };
} // namespace mge::script