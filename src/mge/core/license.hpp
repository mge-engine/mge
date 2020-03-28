#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/enum_set.hpp"
#include "mge/core/version.hpp"
#include <string_view>
#include <iosfwd>

namespace mge {

    enum class license_type : uint8_t {
        UNKNOWN,            //!< Yet unknown license.
        MIT,                //!< MIT License
        BSD,                //!< BSD License
        GPL2,               //!< GPL 2 License
        LGPL2,              //!< LGPL 2 License
        GPL3,               //!< GPL 3 License
        LGPL3,              //!< LGPL 3 License
        PUBLIC_DOMAIN,      //!< Public Domain software
        CC,                 //!< Any creative commons license
        COMMERCIAL,         //!< Any commercial license
        OTHER               //!< Other license.
    };

    enum class license_permission_value : uint8_t
    {
        USAGE            = 0,
        REPRODUCTION     = 1,
        DISTRIBUTION     = 2,
        DERIVATIVE_WORKS = 3,
    };

    using license_permission = mge::enum_set<license_permission_value, 3>;

    enum class license_requirement_value : uint8_t
    {
        NONE            = 0,
        NOTICE          = 1,
        ATTRIBUTION     = 2,
        SHARE_ALIKE     = 3,
        SOURCE_CODE     = 4
    };

    using license_requirement = mge::enum_set<license_requirement_value, 4>;

    /**
     * Properties of a license.
     */
    struct license_properties
    {
        license_permission  permissions;
        license_requirement requirements;
        bool                noncommercial;
    };

    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os, license_type type);

    /**
     * License. An asset, component, library or application - any kind
     * of artifact may have a license information, which refers to a license.
     */
    class MGE_CORE_EXPORT license
    {
    public:
        license();
        license(license_type type);
        license(license_type type,
                std::string_view text,
                const mge::version& version,
                const license_properties& properties);
        ~license() = default;
        license(const license&) = default;
        license(license&&) = default;

        std::string_view text() const;
        license_type type() const;
        mge::version version() const;
        const license_properties& properties() const;
    private:
        license_type       m_type;
        std::string        m_text;
        license_properties m_properties;
    };
}
