// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <boost/lexical_cast.hpp>
#include "mge/core/stdexceptions.hpp"

namespace mge {

    template <typename Target, typename Source>
    inline Target lexical_cast(const Source& source)
    {
        try {
            return boost::lexical_cast<Target>(source);
        } catch(const boost::bad_lexical_cast& e) {
            MGE_THROW(mge::bad_cast(),
                      e.what());
        }
    }
}
