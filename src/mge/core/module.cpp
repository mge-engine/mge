// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/module.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/path.hpp"
#include "mge/core/file.hpp"
#include "mge/core/log.hpp"

#include <vector>
#include <boost/algorithm/string.hpp>

MGE_USE_LOG(MGE);

namespace mge {

    class module_registry
    {
    public:
        module_registry()
        {
            m_load_paths.emplace_back(file(path(".")));
            m_load_paths.emplace_back(file(path("./modules")));

            m_module_prefixes.emplace_back("mge_module");
            m_module_prefixes.emplace_back("libmge_module");
        }

        ~module_registry() = default;

        void load_all()
        {
            for(const auto& p : m_load_paths) {
                MGE_DEBUG_LOG(MGE) << "Scanning module load path: " << p ;
                auto elements = p.list();
                for(const auto& f : elements) {
                    if(is_module_file(f)) {
                        load_module(f);
                    }
                }
            }
        }
    private:
        bool is_module_file(const file& f) const
        {
            for(const auto& prefix : m_module_prefixes) {
                if(boost::starts_with(f.path().filename().generic_string(), prefix)) {
                    if(f.path().extension().generic_string() == shared_library::extension()) {
                        return true;
                    }
                }
            }
            return false;
        }

        void load_module(const file& module_file)
        {
            MGE_DEBUG_LOG(MGE) << "Loading module: " << module_file;
            try {
                m_loaded_modules.emplace_back(shared_library::load(module_file.path().string()));
            } catch(...) {
                MGE_WARNING_LOG(MGE) << "Load module " << module_file << " failed";
            }
            MGE_DEBUG_LOG(MGE) << "Module " << module_file << " loaded";
        }
        std::vector<file> m_load_paths;
        std::vector<std::string> m_module_prefixes;
        std::vector<shared_library_ref> m_loaded_modules;
    };

    static singleton<module_registry> s_module_registry;

    void
    module::load_all()
    {
        s_module_registry->load_all();
    }
}
