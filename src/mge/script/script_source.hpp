// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"

#include <string>

namespace mge {
    /**
     * A script source that is named. This can be e.g. a file name,
     * or an internal resource name.
     */
    class MGE_SCRIPT_EXPORT script_source
    {
    public:
        script_source(const std::string &name, const std::string &source);

        ~script_source() = default;

        const std::string &name() const noexcept { return m_name; }
        const std::string &source() const noexcept { return m_source; }

    private:
        std::string m_name;
        std::string m_source;
    };
} // namespace mge
