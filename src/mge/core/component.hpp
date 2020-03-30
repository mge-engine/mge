// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/type_name.hpp"
#include "mge/core/types.hpp"

#include <functional>
#include <memory>
#include <string_view>
#include <type_traits>

namespace mge {

    class component_base;

    class MGE_CORE_EXPORT component_registry_entry_base : noncopyable
    {
    public:
        component_registry_entry_base()           = default;
        virtual ~component_registry_entry_base()  = default;
        virtual const char *name() const noexcept = 0;

    protected:
        void register_component(component_registry_entry_base *c);
    };

    class MGE_CORE_EXPORT implementation_registry_entry_base
    {
    public:
        implementation_registry_entry_base()                = default;
        virtual ~implementation_registry_entry_base()       = default;
        virtual const char *component_name() const noexcept = 0;
        virtual const char *name() const noexcept           = 0;
        virtual const char *alias_names() const noexcept    = 0;
        virtual std::shared_ptr<component_base> create()    = 0;

    protected:
        void register_implementation(implementation_registry_entry_base *i);
    };

    class MGE_CORE_EXPORT component_base
    {
    protected:
        component_base() = default;

    public:
        virtual ~component_base() = default;

        template <typename T>
        static inline std::shared_ptr<T> create(const char *implementation_name)
        {
            return std::dynamic_pointer_cast<T, component_base>(
                create(type_name<T>(), implementation_name));
        }

        static std::shared_ptr<component_base>
        create(const std::string &component_name,
               const char *       implementation_name);

        template <typename T>
        static inline void implementations(
            const std::function<void(const std::string &)> &callback)
        {
            implementations(type_name<T>(), callback);
        }

        static void implementations(
            const std::string &                             component_name,
            const std::function<void(const std::string &)> &callback);

        static bool component_registered(const std::string &name);
        static bool
        implementation_registered(const std::string &component_name,
                                  const std::string &implementation_name);

        inline std::string_view implementation_name() const noexcept
        {
            if (m_impl_regentry) {
                return m_impl_regentry->name();
            } else {
                return "";
            }
        }

    private:
        friend class component_registry;

        implementation_registry_entry_base *m_impl_regentry;
    };

    template <typename Class>
    class component_registry_entry : public component_registry_entry_base
    {
    public:
        inline component_registry_entry() : m_name(type_name<Class>())
        {
            static_assert(
                std::is_base_of<component_base, Class>::value,
                "Component class is not derived from base component class");
            register_component(this);
        }
        virtual ~component_registry_entry() = default;
        const char *name() const noexcept override { return m_name.c_str(); }

    private:
        std::string m_name;
    };

    template <typename ComponentType, typename ImplementationType>
    class implementation_registry_entry
        : public implementation_registry_entry_base
    {
    public:
        inline implementation_registry_entry(const char *alias_names)
            : m_component_name(type_name<ComponentType>()),
              m_name(type_name<ImplementationType>()),
              m_alias_names(alias_names)
        {
            static_assert(
                std::is_base_of<ComponentType, ImplementationType>::value,
                "Implementation is not derived from component type");
            static_assert(std::is_base_of<component_base, ComponentType>::value,
                          "Implementation is not derived from mge::component");
            register_implementation(this);
        }

        virtual ~implementation_registry_entry() noexcept = default;
        const char *component_name() const noexcept override
        {
            return m_component_name.c_str();
        }
        const char *name() const noexcept override { return m_name.c_str(); }
        const char *alias_names() const noexcept override
        {
            return m_alias_names.c_str();
        }
        std::shared_ptr<component_base> create() override
        {
            return std::make_shared<ImplementationType>();
        }

    private:
        std::string m_component_name;
        std::string m_name;
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

    public:
        /**
         * Destructor-
         */
        virtual ~component() noexcept = default;

        /**
         * @brief Create an instance of a registered implementation.
         *
         * @param implementation name of the implementation
         * @return @c std::shared_ptr<Class> referring to the implementation
         */
        static inline std::shared_ptr<Class> create(const char *implementation)
        {
            return component_base::create<Class>(implementation);
        }

        /**
         * @brief Create an instance of a registered implementation.
         *
         * @param implementation name of the implementation
         * @return @c std::shared_ptr<Class> referring to the implementation
         */
        static inline std::shared_ptr<Class>
        create(const std::string &implementation)
        {
            return component_base::create<Class>(implementation.c_str());
        }

        /**
         * @brief Enumerate over all implementations.
         *
         * @param callback callback function called for each implementation name
         */
        static inline void implementations(
            const std::function<void(const std::string &)> &callback)
        {
            component_base::implementations<Class>(callback);
        }
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
