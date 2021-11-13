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
        type_details(const module_details_ref& type_module,
                     const std::string_view    name);
        ~type_details();
    };

} // namespace mge::script