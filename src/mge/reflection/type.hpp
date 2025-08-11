// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include "mge/reflection/type_details.hpp"
#include <string_view>

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
        constexpr bool   is_bool() const noexcept;
        constexpr bool   is_integral() const noexcept;
        constexpr bool   is_floating_point() const noexcept;
        constexpr size_t size() const noexcept;

        std::string_view name() const;

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
        constexpr bool is_bool() const noexcept
        {
            return false;
        }
        constexpr bool is_integral() const noexcept
        {
            return false;
        }
        constexpr bool is_floating_point() const noexcept
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

        std::string_view name() const noexcept
        {
            return "void";
        }
    };

    template <> class type<bool>
    {
    public:
        type() = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return false;
        }
        constexpr bool is_integral() const noexcept
        {
            return false;
        }
        constexpr bool is_bool() const noexcept
        {
            return true;
        }
        constexpr bool is_floating_point() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(bool);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<bool>();
        }

        std::string_view name() const noexcept
        {
            return "bool";
        }
    };

    template <> class type<float>
    {
    public:
        type() = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return false;
        }
        constexpr bool is_integral() const noexcept
        {
            return false;
        }
        constexpr bool is_bool() const noexcept
        {
            return false;
        }
        constexpr bool is_floating_point() const noexcept
        {
            return true;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(float);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<float>();
        }

        std::string_view name() const noexcept
        {
            return "float";
        }
    };

    template <> class type<double>
    {
    public:
        type() = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return false;
        }
        constexpr bool is_integral() const noexcept
        {
            return false;
        }
        constexpr bool is_bool() const noexcept
        {
            return false;
        }
        constexpr bool is_floating_point() const noexcept
        {
            return true;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(double);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<double>();
        }

        std::string_view name() const noexcept
        {
            return "double";
        }
    };

#define MGE_DEFINE_INTEGER_TYPE(T)                                             \
    template <> class type<T>                                                  \
    {                                                                          \
    public:                                                                    \
        type() = default;                                                      \
        ~type() = default;                                                     \
                                                                               \
        constexpr bool is_void() const noexcept                                \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_integral() const noexcept                            \
        {                                                                      \
            return true;                                                       \
        }                                                                      \
        constexpr bool is_bool() const noexcept                                \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_floating_point() const noexcept                      \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr size_t size() const noexcept                                 \
        {                                                                      \
            return sizeof(T);                                                  \
        }                                                                      \
                                                                               \
        const type_details_ref& details() const noexcept                       \
        {                                                                      \
            return get_or_create_type_details<T>();                            \
        }                                                                      \
                                                                               \
        std::string_view name() const noexcept                                 \
        {                                                                      \
            return #T;                                                         \
        }                                                                      \
    }

    MGE_DEFINE_INTEGER_TYPE(char);
    MGE_DEFINE_INTEGER_TYPE(unsigned char);
    MGE_DEFINE_INTEGER_TYPE(short);
    MGE_DEFINE_INTEGER_TYPE(unsigned short);
    MGE_DEFINE_INTEGER_TYPE(int);
    MGE_DEFINE_INTEGER_TYPE(unsigned int);
    MGE_DEFINE_INTEGER_TYPE(long);
    MGE_DEFINE_INTEGER_TYPE(unsigned long);
    MGE_DEFINE_INTEGER_TYPE(int64_t);
    MGE_DEFINE_INTEGER_TYPE(uint64_t);

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
        details->is_bool = std::is_same_v<T, bool>;
        details->is_integral = std::is_integral_v<T>;
        details->is_floating_point = std::is_floating_point_v<T>;
        if constexpr (std::is_void_v<T>) {
            details->size = 0;
        } else {
            details->size = sizeof(T);
        }
        return type_details::put(id, details);
    }

} // namespace mge::reflection