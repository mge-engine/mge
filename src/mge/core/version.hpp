// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#endif
#include <boost/operators.hpp>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5204)
#endif

#include <iosfwd>
#include <string>
#include <tuple>

namespace mge {
    /**
     * A semantic version, which is a tuple of major, minor
     * and patch level.
     */
    class MGE_CORE_EXPORT version : public boost::less_than_comparable<version>,
                                    public boost::equality_comparable<version>
    {
    public:
        /**
         * Construct version 0.
         */
        version();
        /**
         * Constuct version from a string
         * @param version version string
         */
        version(const char *version);
        /**
         * Constuct version from a string
         * @param version version string
         */
        version(const std::string &version);
        /**
         * Constuct version from fields.
         * @param major major version
         * @param minor minor version
         * @param patch patch level
         */
        version(unsigned int major, unsigned int minor = 0u,
                unsigned int patch = 0u);
        /**
         * Copy constructor.
         * @param v copied version
         */
        version(const version &v);
        /**
         * Move constructor.
         * @param v moved version
         */
        version(version &&v);

        /**
         * Assignment operator.
         * @param v assigned version
         */
        version &operator=(const version &v);

        /**
         * Move assignment operator.
         * @param v assigned version
         */
        version &operator=(version &&v);

        /**
         * Get major version.
         * @return major version
         */
        unsigned int major() const noexcept { return std::get<0>(m_data); }

        /**
         * Get minor version.
         * @return minor version
         */
        unsigned int minor() const noexcept { return std::get<1>(m_data); }

        /**
         * Get patch level.
         * @return patch level
         */
        unsigned int patch() const noexcept { return std::get<2>(m_data); }

        /**
         * Return whether the version is empty (ie. 0.0.0)
         * @return true if the version is 0.0.0
         */
        bool empty() const noexcept;

        /**
         * Compares two versions.
         * @param v version to compare
         * @return @c true if this version is less than @c v
         */
        bool operator<(const version &v) const noexcept;

        /**
         * Compares two versions.
         * @param v version to compare
         * @return @c true if this version and @c v are equal
         */
        bool operator==(const version &v) const noexcept;

    private:
        void from_string(const std::string &s);
        std::tuple<unsigned int, unsigned int, unsigned int> m_data;
    };

    /**
     * Print operator.
     * @param os target stream
     * @param v version to print
     * @return @c os
     */
    MGE_CORE_EXPORT std::ostream &operator<<(std::ostream & os,
                                             const version &v);
} // namespace mge
