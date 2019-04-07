// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/constructor.hpp"
namespace mge {
    namespace reflection {
        void
        constructor::invoke(void *object, const parameter_source &parameters)
        {
            m_function(object, parameters);
        }
    }
}
