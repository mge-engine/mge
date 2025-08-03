// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/package.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/version.hpp"

#include <set>
#include <string_view>

using namespace std::literals::string_view_literals;

namespace mge {
    MGE_REGISTER_COMPONENT(package);

    MGE_DEFINE_PACKAGE(mge,
                       semantic_version(MGE_VERSION_MAJOR,
                                        MGE_VERSION_MINOR,
                                        MGE_PATCHLEVEL),
                       MGE_BUILD);

    class package_list
    {
    public:
        package_list() = default;
        ~package_list() = default;

        package_ref get(std::string_view name,
                        bool             throw_no_such_element = false)
        {
            auto it = m_packages.find(name);
            if (it != m_packages.end()) {
                return it->second;
            }
            package::implementations([&](std::string_view impl_name) {
                auto impl_it = m_package_implementations.find(impl_name);
                if (impl_it == m_package_implementations.end()) {
                    m_package_implementations.insert(
                        std::string(impl_name.begin(), impl_name.end()));
                    auto pkg = package::create(impl_name);
                    m_packages.insert(std::make_pair(pkg->name(), pkg));
                }
            });
            it = m_packages.find(name);
            if (it != m_packages.end()) {
                return it->second;
            }
            if (throw_no_such_element) {
                MGE_THROW(mge::no_such_element)
                    << "Package '" << name << "' not found";
            }
            return nullptr;
        }

        std::map<std::string, package_ref, std::less<>> m_packages;
        std::set<std::string, std::less<>> m_package_implementations;
    };

    static singleton<package_list> s_package_list;

    package_ref package::get(std::string_view name)
    {
        return s_package_list->get(name);
    }

} // namespace mge
