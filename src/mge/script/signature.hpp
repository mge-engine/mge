// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <typeindex>
#include <vector>

namespace mge::script {
    class MGESCRIPT_EXPORT signature
    {
    public:
        signature(const type_data_ref&              return_type,
                  const std::vector<type_data_ref>& argument_types)
            : m_return_type(return_type)
            , m_argument_types(argument_types)
        {}

        ~signature() = default;

    private:
        type_data_ref              m_return_type;
        std::vector<type_data_ref> m_argument_types;
    };
} // namespace mge::script