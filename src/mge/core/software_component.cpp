// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/software_component.hpp"
#include "mge/version.hpp"

namespace mge {
    MGE_REGISTER_COMPONENT(software_component);

    MGE_DEFINE_SOFTWARE_COMPONENT(mge,
                                  semantic_version(MGE_VERSION_MAJOR,
                                                   MGE_VERSION_MINOR,
                                                   MGE_PATCHLEVEL),
                                  MGE_BUILD);

    software_component_ref software_component::mge()
    {
        software_component_ref result;
        return result;
    }
} // namespace mge