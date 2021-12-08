// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    namespace details {
        class MGESCRIPT_EXPORT function_base
        {
        public:
            function_base() = default;
            virtual ~function_base() = default;
            const std::string& name() const;

        protected:
            function_details_ref get_details(void* address);
            static const std::string
            details_name(const function_details_ref& f);

            function_details_ref m_details;
        };
    } // namespace details

    /**
     * @brief Register a callable object (function).
     *
     * @tparam R result type
     * @tparam Args argument types
     */
    template <typename R, typename... Args>
    class function : public details::function_base
    {
    public:
        /**
         * @brief Register a callable object using function pointer.
         *
         * @param name function name
         * @param fptr function pointer
         */
        function(const std::string& name, R (*fptr)(Args... args))
        {
            m_details = get_details(fptr);
            if (m_details) {
                if (details_name(m_details) != name) {
                    MGE_THROW(illegal_state)
                        << "Function '" << name
                        << "' already registered as function '"
                        << details_name(m_details);
                }
            } else {
            }
        }
    };
} // namespace mge::script