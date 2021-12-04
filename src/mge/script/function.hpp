// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
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
            void create_details(const std::string& name, void* fptr);

            function_details_ref m_details;
        };
    } // namespace details

    template <typename R, typename... Args>
    class function : public details::function_base
    {
    public:
        function(const std::string& name, R (*fptr)(Args... args)) {}
    };
} // namespace mge::script