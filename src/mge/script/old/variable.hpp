// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"

#include <optional>

namespace mge::script {
    namespace details {
        class MGESCRIPT_EXPORT variable_base
        {
        public:
            variable_base() = default;
            virtual ~variable_base() = default;

            const std::string&          name() const;
            void*                       address() const;
            const variable_details_ref& details() const;

        protected:
            variable_details_ref get_details_ref(void* address);
            variable_details_ref
            create_details_ref(const std::string&      name,
                               void*                   address,
                               const type_details_ref& type);

            variable_details_ref m_details;
        };
    } // namespace details

    template <typename T> class variable : public details::variable_base
    {
    public:
        variable(const std::string& name, T& value)
        {
            auto details = get_details_ref(std::addressof(value));
            if (details) {
                m_details = details;
            } else {
                m_details = create_details_ref(name,
                                               std::addressof(value),
                                               type<T>().details());
            }
        }
    };
} // namespace mge::script