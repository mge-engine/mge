// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include <memory>
#include <string>
#include <string_view>

namespace mge::script {

    /**
     * @brief Details of a type.
     */
    class MGESCRIPT_EXPORT type_details
        : public std::enable_shared_from_this<type_details>
    {
    public:
        /**
         * @brief Primitive type classification.
         *
         * Basic types, expected to be built-in.
         */
        struct classification
        {
            unsigned int is_void : 1;
            unsigned int is_unsigned : 1;
            unsigned int is_char : 1;
            unsigned int is_short : 1;
            unsigned int is_int : 1;
            unsigned int is_long : 1;
            unsigned int is_bool : 1;
            unsigned int is_float : 1;
            unsigned int is_double : 1;
        };

        type_details(const std::string& name);
        type_details(const std::string& name, const classification& c);
        virtual ~type_details() = default;

        const std::string&    name() const;
        mge::script::module   module() const;
        const classification& type_class() const;

        virtual void apply(visitor& v);

    private:
        friend class module_details;

        std::string             m_name;
        classification          m_type_class;
        module_details_weak_ref m_module;
    };

} // namespace mge::script