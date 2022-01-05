// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_details.hpp"

namespace mge::script {
    function_details::function_details(const std::string& name)
        : m_name(name)
    {}

    function_details::~function_details() {}

} // namespace mge::script