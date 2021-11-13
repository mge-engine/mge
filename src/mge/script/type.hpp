// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/sfinae.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

#include <list>
#include <string_view>
#include <tuple>

namespace mge::script {

    template <typename T> class type
    {};

} // namespace mge::script