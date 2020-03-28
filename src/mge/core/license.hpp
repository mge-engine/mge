#pragma once
#include "mge/core/dllexport.hpp"
#include <string_view>

namespace mge {

    /**
     * License. An asset, component, library or application - any kind
     * of artifact may have a license information, which refers to a license.
     */
    class MGE_CORE_EXPORT license
    {
    public:
        /**
         * Well known license types.
         */
        enum license_type {
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

        license();
        license(license_type type);
        license(std::string_view text);

    };
}
