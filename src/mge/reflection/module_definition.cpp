// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module_definition.hpp"
#include <map>

namespace mge {
    namespace reflection {

        sequence<identifier>
        module_definition::s_sequence(1);

        static singleton<std::map<std::string, module_definition *>>
        s_modules;

        module_definition::module_definition(const char* name)
            :m_name(name)
            ,m_id(s_sequence.next())
        {}

        module_definition::module_definition()
            :m_id(1)
        {}

        module_definition::~module_definition()
        {}

        identifier
        module_definition::id() const
        {
            return m_id;
        }

        const std::string&
        module_definition::name() const
        {
            return m_name;
        }

        module_definition&
        module_definition::get(const char *name)
        {
            auto it = s_modules->find(name);
            if(it == s_modules->end()) {
                module_definition *d = new module_definition(name);
                s_modules->insert(std::make_pair(d->name(), d));
                it = s_modules->find(name);
            }
            return * it->second;
        }
    }
}
