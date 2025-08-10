// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include "mge/reflection/type_details.hpp"

namespace mge::reflection {

    template <typename T>
    inline const type_details_ref& get_or_create_type_details();

    template <typename T> class type
    {
    public:
        type()
        {
            static_assert(false, "Unsupported type");
        }

        ~type() = default;

        constexpr bool   is_void() const noexcept;
        constexpr bool   is_integral() const noexcept;
        constexpr size_t size() const noexcept;

        const type_details_ref& details() const noexcept;
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

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<void>();
        }
    };

    template <typename T>
    inline const type_details_ref& get_or_create_type_details()
    {
        const auto  id = make_type_identifier<T>();
        const auto& get_result = type_details::get(id);
        if (get_result) {
            return get_result;
        }
        auto details = std::make_shared<type_details>();
        details->is_void = std::is_void_v<T>;
        details->is_integral = std::is_integral_v<T>;
        if constexpr (std::is_void_v<T>) {
            details->size = 0;
        } else {
            details->size = sizeof(T);
        }
        return type_details::put(id, details);
    }

} // namespace mge::reflection