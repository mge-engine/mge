// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    template <typename T, typename R, typename... Args> class method
    {
    public:
        ~method() = default;

    private:
        method_data_ref m_data;
    };

} // namespace mge::script