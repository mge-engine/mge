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

            type_details_ref get_details(std::type_index index)
            {
                auto it = m_types.find(index);
                if (it != m_types.end()) {
                    return it->second;
                } else {
                    return type_details_ref();
                }
            }

            type_details_ref
            create_details(std::type_index            index,
                           const std::string&         name,
                           const type_classification& type_class)
            {
                MGE_DEBUG_TRACE(SCRIPT) << "Register type: " << name;
                auto t =
                    std::make_shared<type_details>(index, name, type_class);

                m_types.insert({t->type_index(), t});
                return t;
            }

        private:
            std::map<std::type_index, type_details_ref> m_types;
        };

        static singleton<global_type_details> s_global_type_details;

        type_details_ref type_base::get_details(std::type_index index)
        {
            return s_global_type_details->get_details(index);
        }

        type_details_ref
        type_base::create_details(std::type_index            index,
                                  const std::string&         name,
                                  const type_classification& type_class)
        {
            return s_global_type_details->create_details(index,
                                                         name,
                                                         type_class);
        }

        std::type_index type_base::type_index() const
        {
            return m_details->type_index();
        }

        const std::string& type_base::name() const { return m_details->name(); }

        const type_details_ref& type_base::details() const { return m_details; }

        mge::script::module type_base::module() const
        {
            return m_details->module();
        }
    } // namespace details

} // namespace mge::script