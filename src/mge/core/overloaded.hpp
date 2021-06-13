// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
namespace mge {

    /**
     * @brief Helper class for visitor.
     *
     * @tparam Ts types composing the visitor
     */
    template <class... Ts> struct overloaded : Ts...
    {
        using Ts::operator()...;
    };

    /**
     * @brief Helper function for vistor composition.
     *
     * @tparam Ts types composing the visitor
     */
    template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
} // namespace mge