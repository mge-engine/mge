// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/reflector.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(REFLECTION);
}

namespace mge::reflection {

    reflector::reflector() {}

    reflector::~reflector() {}

    void reflector::reflect_all()
    {
        MGE_DEBUG_TRACE(REFLECTION, "Reflecting all types");
        std::vector<std::shared_ptr<reflector>> reflectors;
        reflector::implementations([&](std::string_view impl_name) {
            reflectors.emplace_back(reflector::create(impl_name));
        });
        MGE_DEBUG_TRACE(REFLECTION,
                        "Found {} reflector implementations",
                        reflectors.size());
    }

    MGE_REGISTER_COMPONENT(reflector);

} // namespace mge::reflection