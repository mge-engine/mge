// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_lexical_cast.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    template<typename Target, typename Source>
    inline Target lexical_cast(const Source& arg)
    {
        try {
            return boost::lexical_cast<Target>(arg);
        } catch(boost::bad_lexical_cast&) {
            MGE_THROW(bad_cast) << "Cannot cast " << arg << " to " << typeid(Target).name();
        }
    }
} // namespace mge
