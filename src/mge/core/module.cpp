// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/module.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/path.hpp"

#include <vector>

namespace mge {

    class module_registry
    {
    public:
        module_registry()
        {
            m_load_paths.emplace_back(path("."));
            m_load_paths.emplace_back(path({".", "modules"}));

            m_module_prefixes.emplace_back("mge_module");
            m_module_prefixes.emplace_back("libmge_module");
        }

        ~module_registry() = default;

        void load_all()
        {

        }

    private:
        std::vector<path> m_load_paths;
        std::vector<std::string> m_module_prefixes;
    };

    static singleton<module_registry> s_module_registry;

    void
    module::load_all()
    {
        s_module_registry->load_all();
    }
}
