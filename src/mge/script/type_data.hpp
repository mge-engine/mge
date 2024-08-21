// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>
#include <typeindex>
#include <typeinfo>

namespace mge::script {

    /**
     * Data on a type.
     */
    class MGESCRIPT_EXPORT type_data
    {
    public:
        type_data(const module_data_ref& module, const std::string& name)
            : m_module(module)
            , m_name(name)
        {}

        ~type_data() = default;

    private:
        module_data_weak_ref m_module;
        std::string          m_name;
    };

} // namespace mge::script