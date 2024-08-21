// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {
    class MGESCRIPT_EXPORT function_data
    {
    public:
        function_data(const char* name, void* fptr);
        ~function_data() = default;

    private:
        std::string          m_name;
        void*                m_fptr;
        module_data_weak_ref m_module;
    };
} // namespace mge::script