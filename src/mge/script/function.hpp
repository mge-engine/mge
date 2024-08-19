// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    template <typename R, typename... Args> class function
    {
    public:
        ~function() = default;

    private:
        function_data_ref m_data;
    };

} // namespace mge::script