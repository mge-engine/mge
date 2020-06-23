// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/enum_set.hpp"
#include "mge/core/version.hpp"
#include <iosfwd>
#include <string_view>

namespace mge {

    enum class license_type : uint8_t
    {
        UNKNOWN,       //!< Yet unknown license.
        MIT,           //!< MIT License
        BSD,           //!< BSD License
        BOOST,         //!< Boost Software License
        GPL2,          //!< GPL 2 License
        LGPL2,         //!< LGPL 2 License
        GPL3,          //!< GPL 3 License
        LGPL3,         //!< LGPL 3 License
        PUBLIC_DOMAIN, //!< Public Domain software
        CC,            //!< Any creative commons license
        COMMERCIAL,    //!< Any commercial license
        OPEN_SOURCE,   //!< Any Open Source compatible license
        OTHER          //!< Other license.
    };

    MGE_CORE_EXPORT std::ostream &operator<<(std::ostream &os,
                                             license_type  type);

    /**
     * License. An asset, component, library or application - any kind
     * of artifact may have a license information, which refers to a license.
     */
    class MGE_CORE_EXPORT license
    {
    public:
        /**
         * @brief Create license of type UNKNOWN.
         */
        license();
        /**
         * @brief Create license of well-known type.
         *
         * @param type license type, licenses that require further
         *   explanation like generic open source or commercial
         *   will raise an error.
         */
        license(license_type type);

        /**
         * @brief Construct a license
         *
         * @param type license type, COMMERCIAL or OPEN_SOURCE
         * @param text license text
         * @param version version of the license, if any
         */
        license(license_type type, std::string_view text,
                const mge::version &version);
        ~license()               = default;
        license(const license &) = default;
        license(license &&)      = default;

        /**
         * @brief Get license for public domain works.
         *
         * @return license describing public domain works
         */
        static license public_domain()
        {
            return license(license_type::PUBLIC_DOMAIN);
        }

        /**
         * @brief Get license text.
         *
         * @return license text
         */
        std::string_view text() const;
        /**
         * @brief Get license type.
         *
         * @return license type
         */
        license_type type() const;

        /**
         * @brief Get license version
         *
         * @return license version
         */
        mge::version version() const;

    private:
        license_type m_type;
        std::string  m_text;
        mge::version m_version;
    };

} // namespace mge
