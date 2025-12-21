// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mge/core/trace.hpp"
#include "mge/reflection/reflector.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(reflector, reflect_all)
    {
        if (!mge::configuration::loaded()) {
            mge::configuration::load();
        }
        mge::module::load_all();
        reflector::reflect_all();
    }

} // namespace mge::reflection
