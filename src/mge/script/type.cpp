// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

#include <map>
namespace mge {
    MGE_USE_TRACE(SCRIPT);
}
namespace mge::script {

    namespace details {

        class global_type_details
        {
        public:
            global_type_details() = default;

            type_details_ref get_or_add_details(const std::string_view name)
            {
                auto it = m_types.find(name);
                if (it != m_types.end()) {
                    return it->second;
                } else {
                    MGE_DEBUG_TRACE(SCRIPT) << "Register type: " << name;
                    auto t = std::make_shared<type_details>(name);
                    m_types.insert({t->name(), t});
                    return t;
                }
            }

        private:
            std::map<std::string_view, type_details_ref> m_types;
        };

        static singleton<global_type_details> s_global_type_details;

        void type_base::get_or_create_details(const std::string_view name)
        {
            m_details = s_global_type_details->get_or_add_details(name);
        }

        std::string_view type_base::name() const { return m_details->name(); }
    } // namespace details

} // namespace mge::script