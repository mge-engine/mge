// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.

#include "mge/reflection/module.hpp"
#include "mge/reflection/module_definition.hpp"

namespace mge {
    namespace reflection {

        module::module()
            :m_module_id(0)
        {}

        module::module(const std::string& name)
            :m_module_id(0)
        {

        }
    }
}
