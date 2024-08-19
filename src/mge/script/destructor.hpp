// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    template <typename T> class destructor
    {
    public:
        ~destructor() = default;

    private:
        destructor_data_ref m_data;
    };

} // namespace mge::script