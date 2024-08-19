// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    /**
     * Data on a type.
     */
    class MGESCRIPT_EXPORT type_data
    {
    public:
        ~type_data() = default;

    private:
        std::string m_name;
    };

} // namespace mge::script