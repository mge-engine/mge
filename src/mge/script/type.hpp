// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/nth_type.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/traits.hpp"
#include <string>

namespace mge::script {

    class MGESCRIPT_EXPORT type_base
    {
    public:
        type_base();
        type_base& operator=(const type_base& t);

        const std::string&      name() const;
        mge::script::module     module() const;
        const std::type_index&  type_index() const;
        const type_details_ref& details() const;
        type_details_ref&       details();

    protected:
        void init_details(const std::type_index& index);
        void init_enum_details(const std::type_index& index,
                               const std::string&     name,
                               const traits&          tr);
        void init_class_details(const std::type_index& index,
                                const std::string&     name,
                                const traits&          tr);
        void get_class_details(const std::type_index& index);
        void set_base(const type_details_ref& base_details);
        void set_destructor(const invoke_function& dtor);
        void add_constructor(const signature&       signature,
                             const invoke_function& ctor);
        void add_field(const std::string&     name,
                       const type_base&       type,
                       const invoke_function& getter);
        void add_field(const std::string&     name,
                       const type_base&       type,
                       const invoke_function& getter,
                       const invoke_function& setter);
        void enum_value(const std::string& name, int64_t value);

    private:
        type_details_ref m_details;
    };

    template <typename T, typename = void> class type : public type_base
    {
    public:
        inline type()
        {
            static_assert(false, "Default expansion of type detected");
        }
    };

    template <typename T>
    class type<T, typename std::enable_if<is_simple_type<T>>::type>
        : public type_base
    {
    public:
        inline type() { init_details(std::type_index(typeid(T))); }

        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;
    };

    template <typename T>
    class type<T, typename std::enable_if<std::is_enum_v<T>>::type>
        : public type_base
    {
    public:
        inline explicit type(const std::string& n)
        {
            auto ti = std::type_index(typeid(T));
            auto tr = traits_of<T>();
            init_enum_details(ti, n, tr);
        }

        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::base_type_name<T>();
            auto tr = traits_of<T>();
            init_enum_details(ti, n, tr);
        }

        auto& enum_value(const std::string& name, T value)
        {
            type_base::enum_value(name, static_cast<int64_t>(value));
            return *this;
        }

    private:
        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;
    };

    template <typename T>
    class type<T, typename std::enable_if<std::is_class_v<T>>::type>
        : public type_base
    {
    private:
        template <typename... ConstructorArgs> struct constructor_helper
        {
            template <std::size_t... I>
            static inline void construct(call_context& context,
                                         std::index_sequence<I...>)
            {
                if constexpr ((sizeof...(I) == 1) &&
                              std::is_same_v<nth_type<0, ConstructorArgs...>,
                                             void>) {
                    new (context.this_ptr()) T();
                } else {
                    new (context.this_ptr())
                        T(context.parameter<nth_type<I, ConstructorArgs...>>(
                            I)...);
                }
            }
        };

    public:
        using self_type =
            type<T, typename std::enable_if<std::is_class_v<T>>::type>;

        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            init_details(ti);
        }

        inline explicit type(const std::string& n)
        {
            auto ti = std::type_index(typeid(T));
            auto tr = traits_of<T>();
            init_class_details(ti, n, tr);
            if (!std::is_trivially_destructible_v<T>) {
                set_destructor([](call_context& ctx) {
                    T* self = static_cast<T*>(ctx.this_ptr());
                    self->~T();
                });
            }
        }

        template <typename... ConstructorArgs> self_type& constructor()
        {
            std::array<std::type_index, sizeof...(ConstructorArgs)> arg_types =
                {std::type_index(typeid(ConstructorArgs))...};
            auto construct = [](call_context& ctx) {
                constructor_helper<ConstructorArgs...>::construct(
                    ctx,
                    std::make_index_sequence<sizeof...(ConstructorArgs)>{});
            };
            signature s(arg_types);
            add_constructor(s, construct);
            return *this;
        }

        template <typename TB, typename TV>
        std::enable_if<std::is_base_of_v<TB, T>, self_type>::type&
        base(const type<TB, TV>& base_type)
        {
            set_base(base_type.details());
            return *this;
        }

        template <typename F>
        self_type& field(const std::string& name, F T::*fieldptr)
        {
            type<F> field_type;
            if constexpr (std::is_const_v<F>) {
                auto getter = [fieldptr](call_context& ctx) {
                    const T* objptr = static_cast<const T*>(ctx.this_ptr());
                    ctx.store_result(objptr->*fieldptr);
                };
                add_field(name, field_type, getter);
            } else {
                auto getter = [fieldptr](call_context& ctx) {
                    T* objptr = static_cast<T*>(ctx.this_ptr());
                    ctx.store_result(objptr->*fieldptr);
                };
                auto setter = [fieldptr](call_context& ctx) {
                    T* objptr = static_cast<T*>(ctx.this_ptr());
                    objptr->*fieldptr = ctx.parameter<T>(0);
                };
                add_field(name, field_type, getter, setter);
            }
            return *this;
        }
    };

} // namespace mge::script