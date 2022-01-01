// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>
#include <typeindex>

namespace mge::script {

    class MGESCRIPT_EXPORT type_details
    {
    public:
        const std::string&     name() const;
        module_details*        module() const;
        const std::type_index& type_index() const;

        void set_module(module_details* m);

        /**
         * @brief Lookup details by type index.
         *
         * @param ti type index
         * @return found type details, if not found exception is raised
         */
        static type_details* get(const std::type_index& ti);

    private:
        std::string     m_name;
        module_details* m_module;
        std::type_index m_type_index;
    };
} // namespace mge::script