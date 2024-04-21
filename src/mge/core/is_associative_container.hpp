// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"

#include <array>
#include <concepts>
#include <map>
#include <string>
#include <tuple>

namespace mge {

    template <typename T>
    concept is_associative_container = requires(T t, T::key_type k) {
        typename T::key_type;
        typename T::mapped_type;
        typename T::value_type;
        typename T::size_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;
        { t[k] } -> std::same_as<typename T::mapped_type&>;
        { t.at(k) } -> std::same_as<typename T::mapped_type&>;
        { t.begin() } -> std::same_as<typename T::iterator>;
        { t.end() } -> std::same_as<typename T::iterator>;
        { t.cbegin() } -> std::same_as<typename T::const_iterator>;
        { t.cend() } -> std::same_as<typename T::const_iterator>;
        { t.empty() } -> std::convertible_to<bool>;
        { t.begin() } -> std::same_as<typename T::iterator>;
        { t.end() } -> std::same_as<typename T::iterator>;
        { t.cbegin() } -> std::same_as<typename T::const_iterator>;
        { t.cend() } -> std::same_as<typename T::const_iterator>;
        { t.empty() } -> std::convertible_to<bool>;
    };

} // namespace mge