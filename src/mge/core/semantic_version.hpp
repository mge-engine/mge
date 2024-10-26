// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_operators.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/format.hpp"
#include <iosfwd>
#include <string_view>
#include <tuple>
#include <cstdint>

namespace mge {

    /**
     * @brief A semantic version, comprised of three levels of versioning.
     *
     * Semantic versioning (https://semver.org) applies certain rules to
     * version number changes:
     *     * a change in the major version number implies possibility
     *       of incompatible API changes
     *     * a change of the minor version number implies added functionality,
     *       but in a backward-compatible manner
     *     * a change in patch level implies only backward compatible bug fixes
     */
    class MGECORE_EXPORT semantic_version
        : public boost::less_than_comparable<semantic_version>,
          public boost::equality_comparable<semantic_version>
    {
    public:
        /**
         * @brief Create version with all elements initialized to 0.
         */
        semantic_version();

        /**
         * @brief Create version as specified.
         *
         * @param major major version
         * @param minor minor version
         * @param patch patch level
         */
        explicit semantic_version(uint32_t major,
                                  uint32_t minor = 0u,
                                  uint32_t patch = 0u);

        /**
         * @brief Construct a new semantic version.
         *
         * @param version_str string in format major.minor.patch
         */
        explicit semantic_version(std::string_view version_str);

        /**
         * @brief Copy constructor.
         *
         * @param v copied version
         */
        semantic_version(const semantic_version& v);
        /**
         * @brief Move constructor.
         *
         * @param v moved version
         */
        semantic_version(semantic_version&& v);

        /**
         * @brief Assignment.
         *
         * @param v assigned version
         * @return @c *this
         */
        semantic_version& operator=(const semantic_version& v);

        /**
         * @brief Move assigment.
         *
         * @param v assigned version
         * @return @c *this
         */
        semantic_version& operator=(semantic_version&& v);

        /**
         * Compares two versions.
         * @param v version to compare
         * @return @c true if this version is less than @c v
         */
        bool operator<(const semantic_version& v) const noexcept;

        /**
         * Compares two versions.
         * @param v version to compare
         * @return @c true if this version and @c v are equal
         */
        bool operator==(const semantic_version& v) const noexcept;

        /**
         * @brief Get whether this version is empty (all components set to 0)
         *
         * @return true if version is empty
         */
        bool empty() const noexcept;

        /**
         * @brief Get major component.
         *
         * @return major component
         */
        uint32_t major() const noexcept;
        /**
         * @brief Get minor component.
         *
         * @return minor component
         */
        uint32_t minor() const noexcept;

        /**
         * @brief Get patch component.
         *
         * @return patch component
         */
        uint32_t patch() const noexcept;

        inline void format(std::format_context& ctx) const
        {
            if (patch() == 0) {
                if (minor() == 0) {
                    std::format_to(ctx.out(), "{}", major());
                } else {
                    std::format_to(ctx.out(), "{}.{}", major(), minor());
                }
            } else
                std::format_to(ctx.out(),
                               "{}.{}.{}",
                               major(),
                               minor(),
                               patch());
        }

    private:
        std::tuple<uint32_t, uint32_t, uint32_t> m_data;
    };
} // namespace mge
