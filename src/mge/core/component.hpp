// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/type_name.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <string_view>

using namespace std::string_view_literals;

namespace mge {

    class component_base;

    class MGECORE_EXPORT component_registry_entry_base
    {
    public:
        component_registry_entry_base() = default;
        virtual ~component_registry_entry_base() = default;
        virtual std::string_view name() const noexcept = 0;

    protected:
        void register_component(component_registry_entry_base*);
    };

    class MGECORE_EXPORT implementation_registry_entry_base
    {
    public:
        implementation_registry_entry_base() = default;
        virtual ~implementation_registry_entry_base() = default;
        virtual std::string_view name() const noexcept = 0;
        virtual std::string_view component_name() const noexcept = 0;
        virtual std::string_view alias_names() const noexcept = 0;
        virtual std::shared_ptr<component_base> create() const = 0;

    protected:
        void register_implementation(implementation_registry_entry_base*);
    };

    class MGECORE_EXPORT component_base
    {
    protected:
        component_base() = default;
        component_base(const component_base&) = default;
        component_base& operator=(const component_base&) = default;

    public:
        virtual ~component_base() = default;

        template <typename T>
        static inline std::shared_ptr<T>
        create(std::string_view implementation_name)
        {
            return std::dynamic_pointer_cast<T, component_base>(
                create(type_name<T>(), implementation_name));
        }

        static std::shared_ptr<component_base>
        create(std::string_view component_name,
               std::string_view implementation_name);

        template <typename T>
        static inline void
        implementations(const std::function<void(std::string_view)>& callback)
        {
            implementations(type_name<T>(), callback);
        }

        static void
        implementations(std::string_view component_name,
                        const std::function<void(std::string_view)>& callback);

        static bool component_registered(std::string_view name);
        static bool
        implementation_registered(std::string_view component_name,
                                  std::string_view implementation_name);

        inline std::string_view implementation_name() const noexcept
        {
            if (m_impl_regentry) {
                return m_impl_regentry->name();
            } else {
                return ""sv;
            }
        }

    private:
        friend class component_registry;

        implementation_registry_entry_base* m_impl_regentry;
    };
    template <typename Class>
    class component_registry_entry : public component_registry_entry_base
    {
    public:
        inline component_registry_entry()
        {
            static_assert(
                std::is_base_of<component_base, Class>::value,
                "Component class is not derived from base component class");
            register_component(this);
        }
        virtual ~component_registry_entry() = default;
        std::string_view name() const noexcept override
        {
            return m_name;
        }

    private:
        std::string m_name{mge::type_name<Class>()};
    };

    template <typename ComponentType, typename ImplementationType>
    class implementation_registry_entry
        : public implementation_registry_entry_base
    {
    public:
        inline implementation_registry_entry()
        {
            static_assert(
                std::is_base_of<ComponentType, ImplementationType>::value,
                "Implementation is not derived from component type");
            static_assert(std::is_base_of<component_base, ComponentType>::value,
                          "Implementation is not derived from mge::component");
            register_implementation(this);
        }

        inline implementation_registry_entry(const char* alias_names)
            : m_alias_names(alias_names)
        {
            static_assert(
                std::is_base_of<ComponentType, ImplementationType>::value,
                "Implementation is not derived from component type");
            static_assert(std::is_base_of<component_base, ComponentType>::value,
                          "Implementation is not derived from mge::component");
            register_implementation(this);
        }

        virtual ~implementation_registry_entry() noexcept = default;
        std::string_view component_name() const noexcept override
        {
            return m_component_name;
        }
        std::string_view name() const noexcept override
        {
            return m_name;
        }
        std::string_view alias_names() const noexcept override
        {
            return m_alias_names;
        }
        std::shared_ptr<component_base> create() const override
        {
            return std::make_shared<ImplementationType>();
        }

    private:
        std::string m_component_name{mge::type_name<ComponentType>()};
        std::string m_name{mge::type_name<ImplementationType>()};
        std::string m_alias_names;
    };

    /**
     * @brief Base class for defining a component.
     *
     * Classes defining an interface are defined as subclass
     * of this class. This class implements factory and
     * introspection methods to instantiate an implementation.
     *
     * @tparam Class interface class
     */
    template <typename Class> class component : public component_base
    {
    protected:
        /**
         * Default constructor.
         */
        component() noexcept = default;

        component(const component&) = delete;
        component& operator=(const component&) = delete;

    public:
        /**
         * Destructor.
         */
        virtual ~component() noexcept = default;

        /**
         * @brief Create an instance of a registered implementation.
         *
         * @param implementation name of the implementation
         * @return @c std::shared_ptr<Class> referring to the implementation
         */
        static inline std::shared_ptr<Class>
        create(std::string_view implementation)
        {
            auto instance = component_base::create<Class>(implementation);
            if (instance) {
                instance->set_self(instance);
            }
            return instance;
        }

        /**
         * @brief Enumerate over all implementations.
         *
         * @param callback callback function called for each implementation name
         */
        static inline void
        implementations(const std::function<void(std::string_view)>& callback)
        {
            component_base::implementations<Class>(callback);
        }

        /**
         * @brief Get reference of this instance.
         * @return weak reference to this instance
         */
        const std::weak_ptr<Class>& self() const noexcept
        {
            return m_self;
        }

    private:
        void set_self(const std::shared_ptr<Class>& instance)
        {
            m_self = instance;
        }

        std::weak_ptr<Class> m_self;
    };

/**
 * @brief Register a class as component. The macro must be used within the
 * definition namespaces of the argument class.
 *
 * @param clazz class name
 */
#define MGE_REGISTER_COMPONENT(clazz)                                          \
    ::mge::component_registry_entry<clazz> __mge_component_registry_entry##clazz

/**
 * @brief Register an implementation. The macro must be used within the
 * definition namespace of the argument class.
 *
 * @param clazz implementation class name
 * @param component component class (interface class)
 * @param ... alias names for registration
 */
#define MGE_REGISTER_IMPLEMENTATION(clazz, component, ...)                     \
    ::mge::implementation_registry_entry<component, clazz>                     \
        __mge_implementation_registry_entry_##clazz =                          \
            ::mge::implementation_registry_entry<component, clazz>(            \
                MGE_STRINGIFY(__VA_ARGS__))
} // namespace mge