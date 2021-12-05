// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/variable.hpp"
#include "mge/core/singleton.hpp"
#include <map>
#include <tuple>

namespace mge::script {

    class global_variable_details
    {
    public:
        global_variable_details() = default;

        variable_details_ref get_details_ref(void* address)
        {
            auto it = m_variables.find(address);
            if (it != m_variables.end()) {
                return it->second;
            }
            return variable_details_ref();
        }

    private:
        std::map<void*, variable_details_ref> m_variables;
    };

    static singleton<global_variable_details> s_global_variable_details;

    namespace details {
        variable_details_ref variable_base::get_details_ref(void* address)
        {
            return s_global_variable_details->get_details_ref(address);
        }

        const variable_details_ref& variable_base::details() const
        {
            return m_details;
        }
    } // namespace details

} // namespace mge::script
