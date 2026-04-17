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
     * @brief A shader profile.
     *
     * A shader profile consists of a name and a version, describing a
     * target profile for shader compilation.
     */
    class MGEGRAPHICS_EXPORT shader_profile
    {
    public:
        /**
         * @brief Construct an empty uninitialized profile object.
         */
        shader_profile() = default;

        /**
         * @brief Create a shader profile object.
         *
         * @param name      shader profile name
         * @param version   shader profile version
         */
        shader_profile(std::string_view name, const semantic_version& version);

        /**
         * @brief Default destructor.
         */
        ~shader_profile() = default;

        shader_profile(const shader_profile&) = default;
        shader_profile& operator=(const shader_profile&) = default;

        /**
         * @brief Get shader profile name.
         *
         * @return shader profile name
         */
        std::string_view name() const;

        /**
         * @brief Get shader profile version.
         *
         * @return shader profile version
         */
        const semantic_version& version() const;

        /**
         * @brief Comparison operator.
         *
         * @param p compared profile
         * @return @c true if equal
         */
        bool operator==(const shader_profile& p) const;

        /**
         * @brief Comparison operator.
         * @param p compared profile
         * @return @c true if not equal
         */
        bool operator!=(const shader_profile& p) const;

        /**
         * @brief Return this profile is compatible with the other.
         *
         * @param other
         * @return @c true if compatible, i.e. same name, and this has less or
         * equal version.
         */
        bool compatible(const shader_profile& other) const;

    private:
        std::string      m_name;
        semantic_version m_version;
    };

    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&         os,
                                                const shader_profile& prof);

} // namespace mge

template <>
struct fmt::formatter<mge::shader_profile>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::shader_profile& prof, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "{} {}", prof.name(), prof.version());
        return ctx.out();
    }
};
