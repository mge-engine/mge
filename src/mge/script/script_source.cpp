// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_source.hpp"

namespace mge {
    script_source::script_source(const std::string& name,
                                 const std::string& source)
        :m_name(name)
        ,m_source(source)
    {}
}
