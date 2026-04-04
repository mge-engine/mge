// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/function.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/type.hpp"

#include <string>
#include <string_view>

namespace mge::reflection {

    /**
     * @brief Reflection module.
     *
     * A module is a container for types, functions, and submodules.
     * It usually maps directly to a namespace in C++.
     *
     * An instance of this class refers to module details stored in
     * a shared module_details instance, i.e. multiple module instances
     * can refer to the same module details (with the same name).
     *
     * Module instances are assignable, copyable, and movable.
     */
    class MGEREFLECTION_EXPORT module
    {
    public:
        /**
         * @brief Create root module.
         */
        module();
        /**
         * @brief Create or get module by name.
         *
         * The module name may contain namespace separators "::" to indicate
         * submodules.
         *
         * @param name module name
         */
        module(const std::string_view name);

        /**
         * @brief Default destructor.
         */
        ~module() = default;

        module(const module&) = default;
        module& operator=(const module&) = default;
        module(module&&) = default;
        module& operator=(module&&) = default;

        /**
         * @brief Check if this is the root module.
         */
        bool is_root() const;

        /**
         * @brief Get module name.
         * Gets the local name of the module, not the full name.
         * @return module name
         */
        std::string_view name() const;

        /**
         * @brief Get full module name.
         * Gets the full name of the module including parent module names.
         * @return full module name
         */
        std::string full_name() const;

        /**
         * @brief Get parent module.
         * @return parent module, or empty module if this is the root module
         */
        mge::reflection::module parent() const;

        /**
         * @brief Add types, functions, or submodules to this module.
         * @tparam T type of the item to add
         * @param t item to add
         * @param args additional items to add
         * @return this module
         */
        template <typename T, typename... Args>
        inline module& operator()(const T& t, const Args&... args)
        {
            add(t);
            return operator()(std::forward<Args>(args)...);
        }

        inline module& operator()()
        {
            return *this;
        }

        /**
         * @brief Get module details.
         * @return module details
         */
        const module_details_ref& details() const noexcept
        {
            return m_details;
        }

        /**
         * @brief Get root module.
         * @return root module
         */
        static module root();

    private:
        void add_details(const type_details_ref& details);
        void add_details(const function_details_ref& details);
        void add_details(const module_details_ref& details);

        template <typename T> void add(const type<T>& t)
        {
            add_details(t.details());
        }

        inline void add(const function& f)
        {
            add_details(f.details());
        }

        inline void add(const module& m)
        {
            add_details(m.details());
        }

        module(const module_details_ref& details) :m_details(details) {}
        module_details_ref m_details;
    };

} // namespace mge::reflection
