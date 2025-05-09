// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/graphics/dllexport.hpp"

#include <iosfwd>
#include <string>
#include <string_view>

namespace mge {

    /**
     * @brief Shader format.
     *
     * A shader format describes the binary format accepted by the graphics
     * engine to define shader programs. Well-known formats are HLSL and
     * SPIRV.
     */
    class MGEGRAPHICS_EXPORT shader_format
    {
    public:
        /**
         * @brief Construct an empty uninitialized format.
         */
        shader_format() = default;

        /**
         * @brief Construct a shader format.
         *
         * @param name shader format name
         */
        shader_format(std::string_view name);

        /**
         * @brief Construct a shader format.
         *
         * @param name shader format name
         * @param version shader format version
         */
        shader_format(std::string_view name, const semantic_version& version);

        shader_format(const shader_format&) = default;
        shader_format& operator=(const shader_format&) = default;

        /**
         * Destructor
         */
        ~shader_format() = default;

        /**
         * Comparison operator.
         * @param other compared shader format
         * @return @c true if equal
         */
        bool operator==(const shader_format& other) const;

        /**
         * Comparison operator.
         * @param other compared shader format
         * @return @c true if not equal
         */
        bool operator!=(const shader_format& other) const;

        /**
         * @brief Get shader format name.
         * @return shader format name
         */
        const std::string& name() const
        {
            return m_name;
        }

        /**
         * @brief Get shader format version.
         * @return shader format version
         */
        const semantic_version& version() const
        {
            return m_version;
        }

    private:
        std::string      m_name;
        semantic_version m_version;
    };
} // namespace mge

template <>
struct fmt::formatter<mge::shader_format>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::shader_format& f, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(),
                       "shader_format{{name={}, version={}}}",
                       f.name(),
                       f.version());
        return ctx.out();
    }
};