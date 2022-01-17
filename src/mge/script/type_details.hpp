// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/traits.hpp"

#include <string>
#include <tuple>
#include <typeindex>
#include <vector>

namespace mge::script {

    class MGESCRIPT_EXPORT type_details
    {
    public:
        type_details(const std::string&     name,
                     const std::type_index& ti,
                     const traits&          t,
                     const char*            used_name);

        virtual ~type_details() = default;

        const std::string&             name() const;
        const std::string&             automatic_name() const;
        const module_details_weak_ref& module() const;
        const std::type_index&         type_index() const;

        void set_module(const module_details_ref& m);

        /**
         * @brief Lookup details by type index.
         *
         * @param ti type index
         * @return found type details, if not found exception is raised
         */
        static type_details_ref get(const std::type_index& ti);
        static type_details_ref get_or_create(const std::type_index& ti,
                                              const std::string&     name,
                                              const traits&          tr);

        /**
         * @brief Applies a visitor.
         *
         * @param v visitor
         */
        virtual void apply(visitor& v);

    private:
        std::string             m_automatic_name;
        std::string             m_name;
        module_details_weak_ref m_module;
        std::type_index         m_type_index;
        traits                  m_traits;
    };

    class MGESCRIPT_EXPORT enum_type_details : public type_details
    {
    public:
        enum_type_details(const std::string&     name,
                          const std::type_index& ti,
                          const traits&          t,
                          const char*            used_name = nullptr);

        virtual ~enum_type_details() = default;
        void apply(visitor& v) override;

        void enum_value(const std::string& name, int64_t value);

    private:
        using value = std::tuple<std::string, int64_t>;
        std::vector<value> m_values;
    };

    class MGESCRIPT_EXPORT class_type_details : public type_details
    {
    public:
        class_type_details(const std::string&     name,
                           const std::type_index& ti,
                           const traits&          t,
                           const char*            used_name = nullptr);

        virtual ~class_type_details() = default;
        void apply(visitor& v) override;

        void set_base(const type_details_ref& base_details);
        void set_destructor(const invoke_function& dtor);

    private:
        std::vector<type_details_ref> m_bases;
        invoke_function               m_destructor;
    };

} // namespace mge::script