// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module_data.hpp"

#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include "mge/script/type_data.hpp"

namespace mge::script {

    struct root_module_data_holder
    {
        module_data_ref root;

        root_module_data_holder()
            : root(std::make_shared<module_data>())
        {
            mge::script::module m(root);
            m(type<bool>(),
              type<int8_t>(),
              type<int16_t>(),
              type<int32_t>(),
              type<int64_t>(),
              type<uint8_t>(),
              type<uint16_t>(),
              type<uint32_t>(),
              type<uint64_t>(),
              type<float>(),
              type<double>(),
              type<long double>());
        }
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

    void module_data::add(const type_data_ref& t)
    {
        if (!t->m_module.expired()) {
            MGE_THROW(illegal_argument)
                << "Type '" << t->name() << "' already belongs to a module";
        }
        t->m_module = shared_from_this();
        for (const auto& m : m_types) {
            if (m->name() == t->name()) {
                MGE_THROW(illegal_argument)
                    << "Type '" << t->name() << "' already exists in module";
            }
        }
        m_types.push_back(t);
    }

    const function_data& module_data::function(const char* name) const
    {
        for (const auto& f : m_functions) {
            if (f->name() == name) {
                return *f;
            }
        }
        MGE_THROW(no_such_element) << "Function '" << name << "' not found";
    }

    const type_data& module_data::type(const char* name) const
    {
        for (const auto& t : m_types) {
            if (t->name() == name) {
                return *t;
            }
        }
        MGE_THROW(no_such_element) << "Type '" << name << "' not found";
    }
} // namespace mge::script