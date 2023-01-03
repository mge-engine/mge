// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/script/dllexport.hpp"

namespace mge::script {

    class MGESCRIPT_EXPORT component
    {
    public:
        using registry_entry =
            std::shared_ptr<mge::implementation_registry_entry_base>;

        static void register_implementation(const registry_entry& entry);
    };
} // namespace mge::script