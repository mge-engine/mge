// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    namespace {
        template <typename T> struct type_base
        {
            static constexpr bool is_void()
            {
                return std::is_void_v<T>;
            }
            static constexpr bool is_integral()
            {
                return std::is_integral_v<T>;
            }
            static constexpr size_t size()
            {
                return sizeof(T);
            }
        };
    } // namespace

    template <typename T> class type : public type_base<T>
    {
    public:
        type() = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return type_base<T>::is_void();
        }

        constexpr bool is_integral() const noexcept
        {
            return type_base<T>::is_integral();
        }

        constexpr size_t size() const noexcept
        {
            return type_base<T>::size();
        }

        // const type_details_ref& details() const noexcept;
    };

    template <> class type<void>
    {
    public:
        type() = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return true;
        }
        constexpr bool is_integral() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return 0;
        }
    };

} // namespace mge::reflection