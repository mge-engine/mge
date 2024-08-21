// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_data.hpp"

namespace mge::script {

    function_data::function_data(const char* name, void* fptr)
        : m_name(name)
        , m_fptr(fptr)
    {}

} // namespace mge::script
