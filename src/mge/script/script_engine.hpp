#pragma once
#include "mge/core/component.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
namespace mge {

    /**
     * @brief A script engine.
     *
     * A script engine wraps up a language or system that is integrated.
     *
     */
    class MGESCRIPT_EXPORT script_engine : public component<script_engine>
    {
    protected:
        script_engine() = default;

    public:
        /**
         * @brief Create a new context for script execution.
         *
         * @return created context
         */
        virtual script_context_ref create_context() = 0;

        /**
         * @brief Destructor.
         */
        virtual ~script_engine() = default;
    };
} // namespace mge