// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/dllexport.hpp"

#include <iostream>
#include <string>

namespace mge {

    class MGECORE_EXPORT dump_info_provider
        : public component<dump_info_provider>
    {
    protected:
        dump_info_provider() = default;

    public:
        virtual ~dump_info_provider() = default;

        virtual std::string section_name() const = 0;
        virtual void        dump_info(std::ostream& os) const = 0;
    };

} // namespace mge
