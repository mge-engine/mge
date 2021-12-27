// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    class MGESCRIPT_EXPORT variable_details
        : public std::enable_shared_from_this<variable_details>
    {
    public:
        variable_details(const std::string&      name,
                         void*                   address,
                         const type_details_ref& type);

        virtual ~variable_details() = default;

        const std::string&      name() const;
        void*                   address() const;
        const type_details_ref& type() const;

        virtual void apply(visitor& v);

    private:
        friend class module_details;

        std::string             m_name;
        void*                   m_address;
        type_details_ref        m_type;
        module_details_weak_ref m_module;
    };
} // namespace mge::script