// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <array>
#include <concepts>
#include <tuple>

namespace mge {

    namespace {

        template <typename T>
        concept allows_tuple_size = requires { std::tuple_size<T>::value; };

    } // namespace

    template <typename T>
    concept not_std_array =
        !allows_tuple_size<T> ||
        (allows_tuple_size<T> &&
         !std::is_same_v<
             T,
             std::array<typename T::value_type, std::tuple_size_v<T>>>);

    template <typename T>
    concept is_container = requires(T t) {
        typename T::value_type;
        typename T::size_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;

        { t.begin() } -> std::same_as<typename T::iterator>;
        { t.end() } -> std::same_as<typename T::iterator>;
        { t.cbegin() } -> std::same_as<typename T::const_iterator>;
        { t.cend() } -> std::same_as<typename T::const_iterator>;
        { t.empty() } -> std::convertible_to<bool>;
    };

    template <typename T>
    concept is_sequence_container = is_container<T> && requires(T t) {
        { t.clear() } -> std::same_as<void>;
        {
            t.insert(t.begin(), *t.begin())
        } -> std::same_as<typename T::iterator>;
        {
            t.insert(t.begin(), 1, *t.begin())
        } -> std::same_as<typename T::iterator>;
        {
            t.insert(t.begin(), t.begin(), t.end())
        } -> std::same_as<typename T::iterator>;
        { t.erase(t.begin()) } -> std::same_as<typename T::iterator>;
        { t.erase(t.begin(), t.end()) } -> std::same_as<typename T::iterator>;
    };

} // namespace mge