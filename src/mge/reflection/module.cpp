// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.

#include "mge/reflection/module.hpp"
#include "mge/reflection/type.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/reflection/bind_types.hpp"
#include <boost/algorithm/string.hpp>

#include <iostream>

namespace mge {
    namespace reflection {

        module::module()
        {
        }

        module::module(const std::string& name)
            :m_module_name(name)
        {}

        static module_ref s_global_module = std::make_shared<module>();
        static std::once_flag s_global_module_init;

        static void init_global_module()
        {
            bind_builtin();
            bind_std();
            bind_core();
        }

        module&
        module::global_module()
        {
            std::call_once(s_global_module_init, init_global_module);
            return *s_global_module;
        }

        module&
        module::get(const std::string& n)
        {
            if(n.empty()) {
                // do not call 'global_module' as we may just
                // bind the global known types
                return *s_global_module;
            } else {
                auto it = boost::make_split_iterator(n, boost::first_finder("::"));
                decltype(it) end;
                auto m = s_global_module->get_or_add(std::string(it->begin(), it->end()));
                ++it;
                while(it != end) {
                    m = m->get_or_add(std::string(it->begin(), it->end()));
                    ++it;
                }
                return *m;
            }
        }

        module_ref
        module::get_or_add(const std::string& local_name)
        {
            auto it = m_modules.find(local_name);
            if(it == m_modules.end()) {
                auto m = std::make_shared<module>(local_name);
                m->set_parent(shared_from_this());
                m_modules[m->name()] = m;
                return m;
            } else {
                return it->second;
            }
        }

        void
        module::set_parent(const module_ref& parent)
        {
            m_parent = parent;
        }

        void
        module::type(const type_ref& tr)
        {
            m_types[tr->name()] = tr;
        }

        void
        module::apply(visitor& v) const
        {
            v.start(*this);
            for(const auto& m: m_modules) {
                m.second->apply(v);
            }
            for(const auto& t: m_types) {
                t.second->apply(v);
            }
            v.finish(*this);
        }
    }
}
