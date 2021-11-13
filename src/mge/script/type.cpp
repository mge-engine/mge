// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {

    namespace details {

        void type_base::get_or_create_details(const std::string_view name) {}
    } // namespace details

} // namespace mge::script