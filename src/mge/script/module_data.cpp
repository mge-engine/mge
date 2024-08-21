// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_data.hpp"

#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_data.hpp"

namespace mge::script {

    struct root_module_data_holder
    {
        module_data_ref root;

        root_module_data_holder()
            : root(std::make_shared<module_data>())
        {}
    };

    mge::singleton<root_module_data_holder> s_root_module_data_holder;

    module_data_ref module_data::root()
    {
        return s_root_module_data_holder->root;
    }

    module_data_ref module_data::get(const std::string& name)
    {
        auto sep_pos = name.rfind("::");

        std::string parent_name;
        std::string module_name;

        if (sep_pos == std::string::npos) {
            if (name.empty()) {
                return root();
            } else {
                module_name = name;
            }
        } else {
            parent_name =
                name.substr(0, sep_pos); // may be empty if name starts with ::
            module_name = name.substr(sep_pos + 2);
            if (module_name.empty()) {
                MGE_THROW(illegal_argument)
                    << "Module name must not end with '::'";
            }
        }

        module_data_ref parent = get(parent_name);
        for (const auto& m : parent->m_modules) {
            if (m->name() == module_name) {
                return m;
            }
        }

        auto result = std::make_shared<module_data>(parent, module_name);
        parent->m_modules.push_back(result);
        return result;
    }

    void module_data::add(const function_data_ref& f)
    {
        for (const auto& m : m_functions) {
            if (m->name() == f->name()) {
                MGE_THROW(illegal_argument) << "Function '" << f->name()
                                            << "' already exists in module";
            }
        }

        m_functions.push_back(f);
    }
} // namespace mge::script