// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/any_integer.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_classification.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <vector>

namespace mge::script {

    /**
     * @brief Details of a type.
     */
    class MGESCRIPT_EXPORT type_details
        : public std::enable_shared_from_this<type_details>
    {
    public:
        type_details(std::type_index index, const std::string& name);
        type_details(std::type_index            index,
                     const std::string&         name,
                     const type_classification& c);
        virtual ~type_details() = default;

        std::type_index            type_index() const;
        const std::string&         name() const;
        mge::script::module        module() const;
        const type_classification& type_class() const;

        void set_alias_name(const std::string& alias);

        virtual void apply(visitor& v);

    protected:
        friend class module_details;

        std::type_index         m_index;
        std::string             m_name;
        std::string             m_alias_name;
        type_classification     m_type_class;
        module_details_weak_ref m_module;
    };

    class MGESCRIPT_EXPORT enum_type_details : public type_details
    {
    public:
        using enum_value = std::tuple<std::string, any_integer>;

        enum_type_details(std::type_index index, const std::string& name);
        enum_type_details(std::type_index            index,
                          const std::string&         name,
                          const type_classification& c);
        virtual ~enum_type_details() = default;

        void apply(visitor& v) override;

        template <typename I> void add_value(const std::string& name, I value)
        {
            std::underlying_type_t<I> i_value;
            i_value = (std::underlying_type_t<I>)(value);
            any_integer enum_val = i_value;
            m_enum_values.push_back(std::make_tuple(name, enum_val));
        }

    private:
        type_details_ref        m_underlying_type;
        std::vector<enum_value> m_enum_values;
    };

    class MGESCRIPT_EXPORT class_type_details : public type_details
    {
    public:
        class_type_details(std::type_index index, const std::string& name);
        class_type_details(std::type_index            index,
                           const std::string&         name,
                           const type_classification& c);
        virtual ~class_type_details() = default;

        void apply(visitor& v) override;
    };

} // namespace mge::script