#pragma once
#include "mge/core/component.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge {

    /**
     * @brief A binder instance makes an API available for the script system.
     */
    class MGESCRIPT_EXPORT script_binder : public mge::component<script_binder>
    {
    public:
        script_binder() = default;
        virtual ~script_binder() = default;

        /**
         * @brief Binds the types and methods.
         */
        virtual void bind() = 0;
    };
} // namespace mge
