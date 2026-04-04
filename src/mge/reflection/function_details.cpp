// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function_details.hpp"

namespace mge::reflection {

    function_details::function_details()
        : m_noexcept(false)
    {}

    function_details::~function_details() {}

    void function_details::apply(visitor& v) const
    {
        v.before(*this);
        v.on(*this);
        v.after(*this);
    }

} // namespace mge::reflection
