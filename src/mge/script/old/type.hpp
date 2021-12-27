// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/nth_type.hpp"
#include "mge/core/sfinae.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_classification.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

#include <iostream>
#include <list>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeindex>

namespace mge::script {

    namespace details {
        class MGESCRIPT_EXPORT type_base
        {
        public:
            type_base() = default;
            virtual ~type_base() = default;
            std::type_index         type_index() const;
            const std::string&      name() const;
            mge::script::module     module() const;
            const type_details_ref& details() const;

            static type_details_ref get_details(std::type_index index);
            static type_details_ref get_details_required(std::type_index index);

        protected:
            type_details_ref
            create_details(std::type_index            index,
                           const std::string&         name,
                           const type_classification& type_class,
                           size_t                     size = 0);

            type_details_ref m_details;
        };

    } // namespace details

    template <typename T, typename = void>
    class type : public details::type_base
    {
    public:
        inline type()
        {
            static_assert(false, "Default expansion of type detected");
        }
    };

    template <> class type<void, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(void));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_void = 1};
                m_details = create_details(ti, "void", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::get_details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<char, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(char));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_char = 1};
                m_details = create_details(ti, "char", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<unsigned char, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(unsigned char));
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_char = 1};
                m_details = create_details(ti, "unsigned char", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<signed char, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(signed char));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_char = 1};
                m_details = create_details(ti, "signed char", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<short, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(short));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_short = 1};
                m_details = create_details(ti, "short", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<unsigned short, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(unsigned short));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_short = 1};
                m_details = create_details(ti, "unsigned short", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<int, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(int));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_int = 1};
                m_details = create_details(ti, "int", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<unsigned int, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(unsigned int));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_int = 1};
                m_details = create_details(ti, "unsigned int", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<int64_t, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(int64_t));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_long = 1};
                m_details = create_details(ti, "long", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<uint64_t, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(uint64_t));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_long = 1};
                m_details = create_details(ti, "unsigned long", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<float, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(float));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_float = 1};
                m_details = create_details(ti, "float", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<double, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(double));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_double = 1};
                m_details = create_details(ti, "double", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    // std types
    template <> class type<std::string, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(std::string));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{};
                m_details = create_details(ti, "string", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
    };

    // enum type
    template <typename T>
    class type<T, typename std::enable_if<std::is_enum<T>::value>::type>
        : public details::type_base
    {
    public:
        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::base_type_name<T>();
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_enum = 1};
                m_details = create_details(ti, n, cls, sizeof(T));
                m_enum_details =
                    std::dynamic_pointer_cast<enum_type_details>(m_details);
            }
        }

        template <typename I> auto& enum_value(const std::string& name, I value)
        {
            m_enum_details->add_value(name, value);
            return *this;
        }

        using details::type_base::details;
        using details::type_base::name;

    private:
        enum_type_details_ref m_enum_details;
    };

    namespace details {
        template <typename T, typename... Args> struct constructor_invoke_helper
        {
            template <std::size_t... I>
            static inline void invoke_constructor(call_context& context,
                                                  std::index_sequence<I...>)
            {
                void* ptr = context.this_ptr();
                new (ptr) T(context.parameter<nth_type<I, Args...>>(I)...);
            }
        };
    } // namespace details

    // class type
    template <typename T>
    class type<T, typename std::enable_if<std::is_class<T>::value>::type>
        : public details::type_base
    {
    public:
        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::type_name<T>();
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_class = 1};
                m_details = create_details(ti, n, cls, sizeof(T));
                m_class_details =
                    std::dynamic_pointer_cast<class_type_details>(m_details);
            }
            if constexpr (std::is_destructible_v<T> &&
                          !std::is_trivially_destructible_v<T>) {
                m_class_details->use_destructor([](call_context& ctx) {
                    T* thisptr_typed = reinterpret_cast<T*>(ctx.this_ptr());
                    if (thisptr_typed) {
                        thisptr_typed->~T();
                    }
                });
            }
            if constexpr (std::is_constructible_v<T> &&
                          std::is_default_constructible_v<T>) {
                m_class_details->add_constructor([](call_context& ctx) {
                    void* this_ptr = ctx.this_ptr();
                    new (this_ptr) T();
                });
            }
        }

        inline explicit type(const std::string& alias_name)
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::type_name<T>();
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_class = 1};
                m_details = create_details(ti, n, cls);
                m_details->set_alias_name(alias_name);
                m_class_details =
                    std::dynamic_pointer_cast<class_type_details>(m_details);
            }
            if constexpr (std::is_destructible_v<T> &&
                          !std::is_trivially_destructible_v<T>) {
                m_class_details->use_destructor([](call_context& ctx) {
                    T* thisptr_typed = reinterpret_cast<T*>(ctx.this_ptr());
                    if (thisptr_typed) {
                        thisptr_typed->~T();
                    }
                });
            }
            if constexpr (std::is_constructible_v<T> &&
                          std::is_default_constructible_v<T>) {
                m_class_details->add_constructor([](call_context& ctx) {
                    void* this_ptr = ctx.this_ptr();
                    new (this_ptr) T();
                });
            }
        }

        template <typename T, typename F>
        auto& field(const char* name, F T::*member_ptr)
        {
            auto field_ti = std::type_index(typeid(F));
            m_class_details->add_field(
                name,
                field_ti,
                std::move([member_ptr](call_context& ctx) {
                    void*    self = ctx.this_ptr();
                    const T* self_object = static_cast<const T*>(self);
                    ctx.store_result(self_object->*member_ptr);
                }),
                std::move([member_ptr](call_context& ctx) {
                    void* self = ctx.this_ptr();
                    T*    self_object = static_cast<T*>(self);
                    self_object->*member_ptr = ctx.parameter<F>(0);
                }));
            return *this;
        }

        template <typename... Args> auto& constructor()
        {
            static_assert(std::is_constructible_v<T, Args...>);
            signature s({std::type_index(typeid(Args))...});
            if constexpr (sizeof...(Args) >= 1) {
                m_class_details->add_constructor(s, [](call_context& context) {
                    details::constructor_invoke_helper<T, Args...>::
                        invoke_constructor(
                            context,
                            std::make_index_sequence<sizeof...(Args)>{});
                });
            } else {
                m_class_details->add_constructor([](call_context& ctx) {
                    void* this_ptr = ctx.this_ptr();
                    new (this_ptr) T();
                });
            }
            return *this;
        }

        using details::type_base::details;
        using details::type_base::name;

    private:
        class_type_details_ref m_class_details;
    };

} // namespace mge::script