// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/version.hpp"
#include "mge/graphics/dllexport.hpp"
#include <iosfwd>
#include <string>

namespace mge {

    /**
     * Description of a shader language - programming language accepted by
     * a shader compiler for a render system.
     */
    class MGE_GRAPHICS_EXPORT shader_language
    {
    public:
        shader_language();
        shader_language(const char *language);
        shader_language(const std::string &language);
        shader_language(const std::string &t, const version &v);

        const std::string & type() const noexcept { return m_type; }
        const mge::version &version() const noexcept { return m_version; }

        bool operator<(const shader_language &l) const;
        bool operator==(const shader_language &l) const;

    private:
        void         from_string(const std::string &s);
        std::string  m_type;
        mge::version m_version;
    };

    MGE_GRAPHICS_EXPORT std::ostream &operator<<(std::ostream &         os,
                                                 const shader_language &l);
} // namespace mge
