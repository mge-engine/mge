// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
namespace mge {
    namespace {
        template <typename T>
        concept resettable = requires(T t) {
            { t.reset() } -> std::same_as<void>;
        };

    } // namespace

    template <typename T> void reset(T& t)
    {
        if constexpr (resettable<T>) {
            t.reset();
        } else {
            t = {};
        }
    }
} // namespace mge