// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/graphics/dllexport.hpp"

#include <string>
#include <string_view>

namespace mge {

    /**
     * @brief A shader language.
     *
     * A shader language consists of a name and a version. Well known shader
     * languages are HLSL and glsl.
     *
     */
    class MGEGRAPHICS_EXPORT shader_language
    {
    public:
        /**
         * @brief Construct an empty uninitialized language object.
         */
        shader_language() = default;

        /**
         * @brief Create a shader language object.
         *
         * @param name      shader language name
         * @param version   shader language version
         */
        shader_language(std::string_view name, const semantic_version& version);

        /**
         * @brief Default destructor.
         */
        ~shader_language() = default;

        shader_language(const shader_language&) = default;
        shader_language& operator=(const shader_language&) = default;

        /**
         * @brief Get shader language name.
         *
         * @return shader language name
         */
        std::string_view name() const;

        /**
         * @brief Get shader language version.
         *
         * @return shader language version
         */
        const semantic_version& version() const;

        /**
         * @brief Comparison operator.
         *
         * @param l compared language
         * @return @c true if equal
         */
        bool operator==(const shader_language& l) const;

        /**
         * @brief Comparison operator.
         * @param l compared language
         * @return @c true if not equal
         */
        bool operator!=(const shader_language& l) const;

        /**
         * @brief Return this language is compatible with the other
         *
         * @param other
         * @return @c true if compatible, i.e. same name, and this has less or
         * equal version.
         */
        bool compatible(const shader_language& other) const;

    private:
        std::string      m_name;
        semantic_version m_version;
    };

} // namespace mge

template <>
struct fmt::formatter<mge::shader_language>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::shader_language& lang, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "{} {}", lang.name(), lang.version());
        return ctx.out();
    }
};