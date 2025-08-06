// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"
namespace mge::reflection {

    module::module() :m_details(module_details::root()) {}

    module::module(const std::string_view name) :m_details(
        module_details::get(name))
    {}

    bool module::is_root() const
    {
        return m_details->is_root();
    }

} // namespace mge::reflection
