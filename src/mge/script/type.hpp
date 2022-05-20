// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/nth_type.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/traits.hpp"
#include <string>

namespace mge::script {

    /**
     * @brief Base type for type representations.
     */
    class MGESCRIPT_EXPORT type_base
    {
    public:
        /**
         * @brief Default constructor.
         */
        type_base();

        /**
         * @brief Assigment
         * @param t assigned type
         * @return @c *this
         */
        type_base& operator=(const type_base& t);

        /**
         * @brief Destructor.
         */
        ~type_base() = default;

        const std::string&      name() const;
        mge::script::module     module() const;
        const std::type_index&  type_index() const;
        const type_details_ref& details() const;
        type_details_ref&       details();

    protected:
        void init_details(const std::type_index& index);
        void init_enum_details(const std::type_index& index,
                               const std::string&     name,
                               const traits&          tr,
                               size_t                 size);
        void init_class_details(const std::type_index& index,
                                const std::string&     name,
                                const traits&          tr,
                                size_t                 size);
        void get_class_details(const std::type_index& index);
        void set_base(const type_details_ref& base_details);
        void set_destructor(const invoke_function& dtor);
        void add_constructor(const signature&       signature,
                             const invoke_function& ctor);

        void add_method(const std::string&     name,
                        const std::type_index& return_type,
                        const signature&       signature,
                        const invoke_function& invoke);

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

    /**
     * @brief Type class.
     *
     * @tparam T type represented by class
     * @tparam typename type used for SFINAE, must resolve to @c void
     */
    template <typename T, typename = void> class type : public type_base
    {
    public:
        inline type();
    };

    /**
     * @brief Simple type specialization.
     *
     * @tparam T type represented, must be simple type
     */
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
            init_enum_details(ti, n, tr, sizeof(T));
        }

        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::base_type_name<T>();
            auto tr = traits_of<T>();
            init_enum_details(ti, n, tr, sizeof(T));
        }

        auto& enum_value(const std::string& name, T value)
        {
            type_base::enum_value(name, static_cast<int64_t>(value));
            return *this;
        }

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
            init_class_details(ti, n, tr, sizeof(T));
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

        inline self_type& constructor()
        {
            auto construct = [](call_context& ctx) {
                new (ctx.this_ptr()) T();
            };
            signature empty;
            add_constructor(empty, construct);
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
                    objptr->*fieldptr = ctx.parameter<F>(0);
                };
                add_field(name, field_type, getter, setter);
            }
            return *this;
        }

    private:
        template <typename... MethodArgs> struct method_helper
        {
            template <typename R, std::size_t... I>
            static inline void call(call_context& ctx,
                                    R (T::*mptr)(MethodArgs...),
                                    std::index_sequence<I...>)
            {
                T* objptr = static_cast<T*>(ctx.this_ptr());
                if constexpr (std::is_void_v<R>) {
                    (objptr->*mptr)(
                        ctx.parameter<nth_type<I, MethodArgs...>>(I)...);
                } else {
                    ctx.store_result((objptr->*mptr)(
                        ctx.parameter<nth_type<I, MethodArgs...>>(I)...));
                }
            }

            template <typename R, std::size_t... I>
            static inline void call(call_context& ctx,
                                    R (T::*mptr)(MethodArgs...) const,
                                    std::index_sequence<I...>)
            {
                const T* objptr = static_cast<const T*>(ctx.this_ptr());
                if constexpr (std::is_void_v<R>) {
                    (objptr->*mptr)(
                        ctx.parameter<nth_type<I, MethodArgs...>>(I)...);
                } else {
                    ctx.store_result((objptr->*mptr)(
                        ctx.parameter<nth_type<I, MethodArgs...>>(I)...));
                }
            }
        };

    public:
        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name, R (T::*mptr)(MethodArgs...))
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name,
                          R (T::*mptr)(MethodArgs...) noexcept)
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name,
                          R (T::*mptr)(MethodArgs...) const)
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name,
                          R (T::*mptr)(MethodArgs...) const noexcept)
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        /**
         * @brief Add elements to the type
         *
         * @tparam SubType        type of first element
         * @tparam SubArgs     other element types
         * @param arg0      first element
         * @param args      other elements
         *
         * @return @c *this
         */
        template <typename SubType, typename... SubArgs>
        self_type& operator()(const SubType& arg0, const SubArgs&... args)
        {
            add_member(arg0);
            return operator()(args...);
        }

        self_type& operator()() { return *this; }

        template <typename SubType, typename SubTypeDetail>
        void add_member(const type<SubType, typename SubTypeDetail>& t)
        {
            MGE_THROW_NOT_IMPLEMENTED;
        }
    };

} // namespace mge::script