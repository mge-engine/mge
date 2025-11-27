// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include "mge/core/callable.hpp"
#include "mge/core/enum.hpp"
#include "mge/reflection/call_context.hpp"
#include "mge/reflection/function_parameter_helper.hpp"
#include "mge/reflection/signature.hpp"
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
        constexpr bool   is_lvalue_reference() const noexcept;
        constexpr bool   is_rvalue_reference() const noexcept;
        constexpr size_t size() const noexcept;
        constexpr size_t alignment_of() const noexcept;

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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return 0;
        }
        constexpr size_t alignment_of() const noexcept
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(bool);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(bool);
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(float);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(float);
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(double);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(double);
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(long double);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(long double);
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
        constexpr bool is_lvalue_reference() const noexcept                    \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr bool is_rvalue_reference() const noexcept                    \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        constexpr size_t size() const noexcept                                 \
        {                                                                      \
            return sizeof(T);                                                  \
        }                                                                      \
        constexpr size_t alignment_of() const noexcept                         \
        {                                                                      \
            return alignof(T);                                                 \
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
#if defined(MGE_COMPILER_MSVC)
    MGE_DEFINE_INTEGER_TYPE(int64_t);
    MGE_DEFINE_INTEGER_TYPE(uint64_t);
#elif defined(MGE_COMPILER_GCC)
    MGE_DEFINE_INTEGER_TYPE(long long);
    MGE_DEFINE_INTEGER_TYPE(unsigned long long);
#endif

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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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

        template <typename T>
        inline T parameter_helper(call_context& ctx, size_t index)
        {
            if constexpr (std::is_pointer_v<T>) {
                return static_cast<T>(
                    ctx.pointer_parameter(index,
                                          *get_or_create_type_details<T>()));
            } else {
                return ctx.template parameter<T>(index);
            }
        }
    } // namespace

    namespace {
        template <typename T>
        concept by_value_result_v =
            (std::is_pointer_v<T> || std::is_same_v<T, std::string_view> ||
             std::is_same_v<T, bool> || std::is_same_v<T, int8_t> ||
             std::is_same_v<T, uint8_t> || std::is_same_v<T, int16_t> ||
             std::is_same_v<T, uint16_t> || std::is_same_v<T, int32_t> ||
             std::is_same_v<T, uint32_t> || std::is_same_v<T, int64_t> ||
             std::is_same_v<T, uint64_t> || std::is_same_v<T, float> ||
             std::is_same_v<T, double> || std::is_same_v<T, long double>);
    }

    namespace {
        template <typename T>
        inline T new_parameter_helper(call_context& ctx, size_t index)
        {
            if constexpr (std::is_pointer_v<T>) {
                return static_cast<T>(
                    ctx.pointer_parameter(index,
                                          *get_or_create_type_details<T>()));
            } else {
                return ctx.template parameter<T>(index);
            }
        }

        template <typename T, typename... Args, size_t... Is>
        inline void new_object_impl(call_context& ctx,
                                    void*         ptr,
                                    std::index_sequence<Is...>)
        {
            new (ptr) T(new_parameter_helper<Args>(ctx, Is)...);
        }

        template <typename T, typename... Args>
        inline void new_object(call_context& ctx, void* ptr)
        {
            new_object_impl<T, Args...>(
                ctx,
                ptr,
                std::make_index_sequence<sizeof...(Args)>{});
        }

    } // namespace

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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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

        template <typename F>
        self_type& field(const char* name, F T::* field_ptr)
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            auto  get_field = [field_ptr](call_context& ctx) {
                void* obj_ptr = ctx.this_ptr();
                if (obj_ptr) {
                    T* obj = static_cast<T*>(obj_ptr);
                    F& field_ref = obj->*field_ptr;
                    if constexpr (by_value_result_v<F>) {
                        ctx.template result<std::remove_cv_t<F>>(field_ref);
                    } else {
                        ctx.template result<F&>(field_ref);
                    }
                }
            };
            invoke_function set_field;
            if constexpr (!std::is_const_v<F>) {
                set_field = [field_ptr](call_context& ctx) {
                    void*        obj_ptr = ctx.this_ptr();
                    const size_t index = 0;
                    if (obj_ptr) {
                        T* obj = static_cast<T*>(obj_ptr);
                        obj->*field_ptr = parameter_helper<F>(ctx, index);
                    }
                };
            }
            auto type_id = make_type_identifier<F>();
            specific.fields.emplace_back(name, type_id, get_field, set_field);
            return *this;
        }

        template <typename... Args> self_type& constructor()
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            signature sig(make_type_identifier<void>(),
                          {make_type_identifier<Args>()...});

            // Check if constructor with this signature already exists
            for (const auto& [existing_sig, _] : specific.constructors) {
                if (existing_sig == sig) {
                    return *this;
                }
            }

            auto invoke_fn = [](call_context& ctx) {
                void* ptr = ctx.this_ptr();
                if (ptr) {
                    try {
                        if constexpr (sizeof...(Args) == 0) {
                            new (ptr) T();
                        } else {
                            new_object<T, Args...>(ctx, ptr);
                        }
                    } catch (const mge::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (const std::exception& ex) {
                        ctx.exception_thrown(ex);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                }
            };
            specific.constructors.emplace_back(sig, invoke_fn);
            return *this;
        }

        template <typename R, typename... Args>
        self_type& static_method(const char* name, R (*method_ptr)(Args...))
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            signature sig(make_type_identifier<R>(),
                          {make_type_identifier<Args>()...});

            auto invoke_fn = [method_ptr](call_context& ctx) {
                try {
                    function_caller<R (*)(Args...), Args...>(ctx, method_ptr);
                } catch (const mge::exception& ex) {
                    ctx.exception_thrown(ex);
                } catch (const std::exception& ex) {
                    ctx.exception_thrown(ex);
                } catch (...) {
                    ctx.exception_thrown();
                }
            };

            specific.static_methods.emplace_back(name, sig, invoke_fn);
            return *this;
        }

        // Member function: non-const, non-noexcept
        template <typename R, typename... Args>
        self_type& method(const char* name, R (T::*method_ptr)(Args...))
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            signature sig(make_type_identifier<R>(),
                          {make_type_identifier<Args>()...});

            auto invoke_fn = [method_ptr](call_context& ctx) {
                try {
                    T& obj = ctx.template this_reference<T>();
                    method_caller<T, R (T::*)(Args...), Args...>(ctx,
                                                                 obj,
                                                                 method_ptr);
                } catch (const mge::exception& ex) {
                    ctx.exception_thrown(ex);
                } catch (const std::exception& ex) {
                    ctx.exception_thrown(ex);
                } catch (...) {
                    ctx.exception_thrown();
                }
            };

            specific.methods.emplace_back(name, sig, invoke_fn, false, false);
            return *this;
        }

        // Member function: const, non-noexcept
        template <typename R, typename... Args>
        self_type& method(const char* name, R (T::*method_ptr)(Args...) const)
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            signature sig(make_type_identifier<R>(),
                          {make_type_identifier<Args>()...});

            auto invoke_fn = [method_ptr](call_context& ctx) {
                try {
                    const T& obj = ctx.template this_reference<T>();
                    method_caller<const T, R (T::*)(Args...) const, Args...>(
                        ctx,
                        obj,
                        method_ptr);
                } catch (const mge::exception& ex) {
                    ctx.exception_thrown(ex);
                } catch (const std::exception& ex) {
                    ctx.exception_thrown(ex);
                } catch (...) {
                    ctx.exception_thrown();
                }
            };

            specific.methods.emplace_back(name, sig, invoke_fn, true, false);
            return *this;
        }

        // Member function: non-const, noexcept
        template <typename R, typename... Args>
        self_type& method(const char* name,
                          R (T::*method_ptr)(Args...) noexcept)
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            signature sig(make_type_identifier<R>(),
                          {make_type_identifier<Args>()...});

            auto invoke_fn = [method_ptr](call_context& ctx) {
                T& obj = ctx.template this_reference<T>();
                method_caller<T, R (T::*)(Args...) noexcept, Args...>(
                    ctx,
                    obj,
                    method_ptr);
            };

            specific.methods.emplace_back(name, sig, invoke_fn, false, true);
            return *this;
        }

        // Member function: const, noexcept
        template <typename R, typename... Args>
        self_type& method(const char* name,
                          R (T::*method_ptr)(Args...) const noexcept)
        {
            auto& specific = get_or_create_type_details<T>()->class_specific();
            signature sig(make_type_identifier<R>(),
                          {make_type_identifier<Args>()...});

            auto invoke_fn = [method_ptr](call_context& ctx) {
                const T& obj = ctx.template this_reference<T>();
                method_caller<const T,
                              R (T::*)(Args...) const noexcept,
                              Args...>(ctx, obj, method_ptr);
            };

            specific.methods.emplace_back(name, sig, invoke_fn, true, true);
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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
        requires std::is_lvalue_reference_v<T>
    class type<T>
    {
        static_assert(std::is_lvalue_reference_v<T>,
                      "This should be an lvalue reference type");

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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return true;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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
        requires std::is_rvalue_reference_v<T>
    class type<T>
    {
        static_assert(std::is_rvalue_reference_v<T>,
                      "This should be an rvalue reference type");

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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return true;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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
        requires std::is_const_v<T> &&
                 (!std::is_pointer_v<T> && !std::is_array_v<T> &&
                  !std::is_reference_v<T> && !is_basic_class_v<T>)
    class type<T>
    {
        static_assert(std::is_const_v<T>, "This should be a const type");

    public:
        using self_type = type<T>;
        using non_const_type = std::remove_const_t<T>;

        type() = default;
        type(const type&) = default;
        type(type&&) noexcept = default;
        type& operator=(const type&) = default;
        type& operator=(type&&) noexcept = default;
        ~type() = default;

        constexpr bool is_void() const noexcept
        {
            return std::is_void_v<non_const_type>;
        }
        constexpr bool is_integral() const noexcept
        {
            return std::is_integral_v<non_const_type>;
        }
        constexpr bool is_bool() const noexcept
        {
            return std::is_same_v<non_const_type, bool>;
        }
        constexpr bool is_floating_point() const noexcept
        {
            return std::is_floating_point_v<non_const_type>;
        }
        constexpr bool is_enum() const noexcept
        {
            return std::is_enum_v<non_const_type>;
        }
        constexpr bool is_class() const noexcept
        {
            return std::is_class_v<non_const_type>;
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
        constexpr bool is_lvalue_reference() const noexcept
        {
            return false;
        }
        constexpr bool is_rvalue_reference() const noexcept
        {
            return false;
        }
        constexpr size_t size() const noexcept
        {
            return sizeof(T);
        }
        constexpr size_t alignment_of() const noexcept
        {
            return alignof(T);
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

        details->name = type<T>::name();
        if constexpr (std::is_void_v<T>) {
            details->size = 0;
            details->alignment_of = 0;
        } else {
            details->size = sizeof(T);
            details->alignment_of = alignof(T);
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
            auto& class_details = details->class_specific();
            class_details.is_constructible = std::is_constructible_v<T>;
            class_details.is_default_constructible =
                std::is_default_constructible_v<T>;
            class_details.is_default_constructor_noexcept =
                std::is_nothrow_default_constructible_v<T>;
            class_details.is_copy_constructible =
                std::is_copy_constructible_v<T>;
            class_details.is_copy_constructor_noexcept =
                std::is_nothrow_copy_constructible_v<T>;
            class_details.is_move_constructible =
                std::is_move_constructible_v<T>;
            class_details.is_move_constructor_noexcept =
                std::is_nothrow_move_constructible_v<T>;
            class_details.is_copy_assignable = std::is_copy_assignable_v<T>;
            class_details.is_copy_assignment_noexcept =
                std::is_nothrow_copy_assignable_v<T>;
            class_details.is_move_assignable = std::is_move_assignable_v<T>;
            class_details.is_move_assignment_noexcept =
                std::is_nothrow_move_assignable_v<T>;
            class_details.is_abstract = std::is_abstract_v<T>;
            class_details.has_virtual_destructor =
                std::has_virtual_destructor_v<T>;
            class_details.is_destructible = std::is_destructible_v<T>;
            class_details.is_empty = std::is_empty_v<T>;

            // Register default constructor if available
            if constexpr (std::is_default_constructible_v<T>) {
                signature sig(make_type_identifier<void>());
                auto      invoke_fn = [](call_context& ctx) {
                    void* ptr = ctx.this_ptr();
                    if (ptr) {
                        try {
                            new (ptr) T();
                        } catch (const mge::exception& ex) {
                            ctx.exception_thrown(ex);
                        } catch (const std::exception& ex) {
                            ctx.exception_thrown(ex);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    }
                };
                class_details.constructors.emplace_back(sig, invoke_fn);
            }

            // Register destructor if available
            if constexpr (std::is_destructible_v<T>) {
                class_details.destructor = [](call_context& ctx) {
                    void* ptr = ctx.this_ptr();
                    if (ptr) {
                        try {
                            static_cast<T*>(ptr)->~T();
                        } catch (const mge::exception& ex) {
                            ctx.exception_thrown(ex);
                        } catch (const std::exception& ex) {
                            ctx.exception_thrown(ex);
                        } catch (...) {
                            ctx.exception_thrown();
                        }
                    }
                };
            }
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