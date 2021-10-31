// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include <map>
#include <string>
#include <string_view>

namespace mge::script {

    class MGESCRIPT_EXPORT module : public std::enable_shared_from_this<module>
    {
    protected:
        module(std::string_view name);

    public:
        ~module();

        std::string_view name() const;
        std::string      full_name() const;
        bool             is_root() const;

        void apply(visitor& v);

        module_ref parent() const;

        static module_ref root();
        static module_ref get(std::string_view name);

        /**
         * @brief Lookup or create type information for type.
         *
         * @tparam T        type added
         * @return type_ref type reference
         */
        template <typename T> type_ref type()
        {
            type_ref result;
            return result;
        }

    private:
        type_ref type_void()
        {
            if (!is_root()) {
                MGE_THROW(illegal_state)
                    << "Void type in non root module requested";
            }
            type_ref result;
            return result;
        }

        std::map<std::string, type_ref>   m_types;
        std::map<std::string, module_ref> m_children;
        mge::script::module_weak_ref      m_parent;
        std::string                       m_name;
    };

} // namespace mge::script
