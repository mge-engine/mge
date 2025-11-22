// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include "mge/core/callable.hpp"
#include "mge/core/enum.hpp"
#include "mge/reflection/type_details.hpp"
#include <iostream>
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
        constexpr bool   is_enum() const noexcept;
        constexpr bool   is_class() const noexcept;
        constexpr bool   is_pointer() const noexcept;
        constexpr bool   is_array() const noexcept;
        constexpr bool   is_reference() const noexcept;
        constexpr size_t size() const noexcept;

        static constexpr std::string_view name();

        const type_details_ref& details() const noexcept;
    };

    template <> class type<void>
    {
    public:
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
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

        static constexpr std::string_view name() noexcept
        {
            return "void";
        }
    };

    template <> class type<bool>
    {
    public:
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
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

        static constexpr std::string_view name() noexcept
        {
            return "bool";
        }
    };

    template <> class type<float>
    {
    public:
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(float);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<float>();
        }

        static constexpr std::string_view name() noexcept
        {
            return "float";
        }
    };

    template <> class type<double>
    {
    public:
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(double);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<double>();
        }

        static constexpr std::string_view name() noexcept
        {
            return "double";
        }
    };

    template <> class type<long double>
    {
    public:
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(long double);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<long double>();
        }

        static constexpr std::string_view name() noexcept
        {
            return "long double";
        }
    };

#define MGE_DEFINE_INTEGER_TYPE(T)                                             \
    template <> class type<T>                                                  \
    {                                                                          \
    public:                                                                    \
        type() = default;                                                      \
        type(const type&) = default;                                           \
        type(type&&) noexcept = default;                                       \
        type& operator=(const type&) = default;                                \
        type& operator=(type&&) noexcept = default;                            \
                                                                               \
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
        constexpr bool is_enum() const noexcept                                \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_class() const noexcept                               \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_pointer() const noexcept                             \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_array() const noexcept                               \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_reference() const noexcept                           \
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
        static constexpr std::string_view name() noexcept                      \
        {                                                                      \
            return #T;                                                         \
        }                                                                      \
    }

    MGE_DEFINE_INTEGER_TYPE(char);
    MGE_DEFINE_INTEGER_TYPE(signed char);
    MGE_DEFINE_INTEGER_TYPE(unsigned char);
    MGE_DEFINE_INTEGER_TYPE(short);
    MGE_DEFINE_INTEGER_TYPE(unsigned short);
    MGE_DEFINE_INTEGER_TYPE(int);
    MGE_DEFINE_INTEGER_TYPE(unsigned int);
    MGE_DEFINE_INTEGER_TYPE(long);
    MGE_DEFINE_INTEGER_TYPE(unsigned long);
    MGE_DEFINE_INTEGER_TYPE(int64_t);
    MGE_DEFINE_INTEGER_TYPE(uint64_t);

#undef MGE_DEFINE_INTEGER_TYPE

    template <typename T>
        requires std::is_enum_v<T>
    class type<T>
    {
    public:
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
            return false;
        }
        constexpr bool is_enum() const noexcept
        {
            return true;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }

        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<T>();
        }

        static constexpr std::string_view name() noexcept
        {
            return mge::enum_type_name<T>();
        }

        auto& value(const std::string_view& name, T value)
        {
            auto& specific_values =
                get_or_create_type_details<T>()->enum_specific().values;
            using UT = std::underlying_type_t<T>;
            auto it = std::find_if(
                specific_values.begin(),
                specific_values.end(),
                [&name](const auto& element) { return element.first == name; });
            if (it != specific_values.end()) {
                it->second = std::is_signed_v<UT>
                                 ? static_cast<int64_t>(value)
                                 : static_cast<uint64_t>(value);
            } else {
                if constexpr (std::is_signed_v<UT>) {
                    specific_values.emplace_back(name,
                                                 static_cast<int64_t>(value));
                } else {
                    specific_values.emplace_back(name,
                                                 static_cast<uint64_t>(value));
                }
            }
            return *this;
        }
    };

    namespace {
        template <typename T>
        concept is_basic_class_v = (std::is_class_v<T> && !(mge::callable<T>));
    }

    template <typename T>
        requires is_basic_class_v<T>
    class type<T>
    {
    public:
        using self_type = type<T>;

        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
            return false;
        }
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return true;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<T>();
        }
        static constexpr std::string_view name() noexcept
        {
            return mge::type_name<T>();
        }

        template <typename B>
            requires std::is_base_of_v<B, T>
        self_type& base()
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            specific.add_base(get_or_create_type_details<B>());
            return *this;
        }
    };

    template <typename T>
        requires std::is_pointer_v<T>
    class type<T>
    {
    public:
        using self_type = type<T>;

        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;

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
            return false;
        }
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return true;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<T>();
        }
        static constexpr std::string_view name() noexcept
        {
            return mge::type_name<T>();
        }
    };

    template <typename T>
        requires std::is_array_v<T>
    class type<T>
    {
    public:
        using self_type = type<T>;
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;
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
            return false;
        }
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return true;
        }
        constexpr bool is_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<T>();
        }
        static constexpr std::string_view name() noexcept
        {
            return mge::type_name<T>();
        }
    };

    template <typename T>
        requires std::is_reference_v<T>
    class type<T>
    {
        static_assert(std::is_reference_v<T>,
                      "This should be a reference type");

    public:
        using self_type = type<T>;
        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;
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
            return false;
        }
        constexpr bool is_enum() const noexcept
        {
            return false;
        }
        constexpr bool is_class() const noexcept
        {
            return false;
        }
        constexpr bool is_pointer() const noexcept
        {
            return false;
        }
        constexpr bool is_array() const noexcept
        {
            return false;
        }
        constexpr bool is_reference() const noexcept
        {
            return true;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        const type_details_ref& details() const noexcept
        {
            return get_or_create_type_details<T>();
        }
        static constexpr std::string_view name() noexcept
        {
            return mge::type_name<T>();
        }
    };

    template <typename T>
    inline const type_details_ref& get_or_create_type_details()
    {
        std::cerr << "get_or_create_type_details<" << mge::type_name<T>()
                  << ">() called.\n";
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
        details->is_enum = std::is_enum_v<T>;
        details->is_class = std::is_class_v<T>;
        details->is_pointer = std::is_pointer_v<T>;
        details->is_array = std::is_array_v<T>;
        details->is_reference = std::is_reference_v<T>;

        // Debug output for reference types
        if constexpr (std::is_same_v<T, int&>) {
            // This should only trigger for int&
            static bool debug_printed = false;
            if (!debug_printed) {
                debug_printed = true;
                // We're in the right function for int&
            }
        }

        details->name = type<T>::name();
        if constexpr (std::is_void_v<T>) {
            details->size = 0;
        } else {
            details->size = sizeof(T);
        }
        if constexpr (std::is_enum_v<T>) {
            using UT = std::underlying_type_t<T>;
            details->enum_specific().underlying_type =
                get_or_create_type_details<UT>();
            if constexpr (std::is_signed_v<UT>) {
                for (const auto& value : mge::enum_values<T>()) {
                    details->enum_specific().values.emplace_back(
                        mge::enum_name(value),
                        static_cast<int64_t>(value));
                }
            } else {
                for (const auto& value : mge::enum_values<T>()) {
                    details->enum_specific().values.emplace_back(
                        mge::enum_name(value),
                        static_cast<uint64_t>(value));
                }
            }
        }
        if constexpr (is_basic_class_v<T>) {
            details->class_specific();
        }
        if constexpr (std::is_pointer_v<T>) {
            details->pointer_specific().element_type =
                get_or_create_type_details<std::remove_pointer_t<T>>();
        }
        if constexpr (std::is_array_v<T>) {
            auto& specific = details->array_specific();
            specific.element_type =
                get_or_create_type_details<std::remove_extent_t<T>>();
            specific.size = std::extent_v<T>;
        }
        if constexpr (std::is_reference_v<T>) {
            details->reference_specific().referenced_type =
                get_or_create_type_details<std::remove_reference_t<T>>();
        }
        return type_details::put(id, details);
    }

} // namespace mge::reflection