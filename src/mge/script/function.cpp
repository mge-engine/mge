// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/core/singleton.hpp"
#include "mge/script/function_details.hpp"

#include <map>
#include <tuple>

namespace mge::script {
    namespace details {

        class global_function_details
        {
        public:
            global_function_details() = default;

            function_details_ref get_details(void* address)
            {
                auto it = m_functions.find(address);
                if (it != m_functions.end()) {
                    return it->second;
                } else {
                    return function_details_ref();
                }
            }

        private:
            std::map<void*, function_details_ref> m_functions;
        };

        static singleton<global_function_details> s_global_function_details;

        function_details_ref function_base::get_details(void* address)
        {
            return s_global_function_details->get_details(address);
        }

        const std::string& function_base::name() const
        {
            return m_details->name();
        }

        const std::string
        function_base::details_name(const function_details_ref& f)
        {
            return f->name();
        }

    } // namespace details
} // namespace mge::script