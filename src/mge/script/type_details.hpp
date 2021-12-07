// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_classification.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>

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

        virtual void apply(visitor& v);

    private:
        friend class module_details;

        std::type_index         m_index;
        std::string             m_name;
        type_classification     m_type_class;
        module_details_weak_ref m_module;
    };

} // namespace mge::script