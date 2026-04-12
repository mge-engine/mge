// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/markdown_document.hpp"

#include <memory_resource>
#include <string>

namespace mge {

    class MGECORE_EXPORT dump_info_provider
        : public component<dump_info_provider>
    {
    protected:
        dump_info_provider() = default;

    public:
        virtual ~dump_info_provider() = default;

        virtual std::pmr::string section_name(
            std::pmr::memory_resource* resource =
                std::pmr::get_default_resource()) const = 0;
        virtual void dump_info(markdown_document& doc) const = 0;
    };

} // namespace mge
