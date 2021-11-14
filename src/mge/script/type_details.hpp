// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include <memory>
#include <string>
#include <string_view>

namespace mge::script {

    class MGESCRIPT_EXPORT type_details
        : public std::enable_shared_from_this<type_details>
    {
    public:
        type_details(const std::string& name);
        ~type_details() = default;

        const std::string&  name() const;
        mge::script::module module() const;

    private:
        friend class module_details;

        std::string             m_name;
        module_details_weak_ref m_module;
    };

} // namespace mge::script