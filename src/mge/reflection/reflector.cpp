// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/reflector.hpp"
#include "mge/core/trace.hpp"

#include <set>

namespace mge {
    MGE_USE_TRACE(REFLECTION);
}

namespace mge::reflection {

    reflector::reflector() {}

    reflector::~reflector() {}

    void reflector::reflect_all()
    {
        MGE_DEBUG_TRACE(REFLECTION, "Reflecting all types");
        std::vector<std::shared_ptr<reflector>> reflectors;
        reflector::implementations([&](std::string_view impl_name) {
            reflectors.emplace_back(reflector::create(impl_name));
        });
        MGE_DEBUG_TRACE(REFLECTION,
                        "Found {} reflector implementations",
                        reflectors.size());
        std::set<std::string, std::less<>> reflectors_done;
        size_t                             current_size = reflectors.size();
        while (!reflectors.empty()) {
            for (auto it = reflectors.begin(); it != reflectors.end();) {
                auto&      reflector = *it;
                const auto deps = reflector->dependencies();
                for (const auto& dep : deps) {
                    if (reflectors_done.find(dep) == reflectors_done.end()) {
                        MGE_DEBUG_TRACE(REFLECTION,
                                        "Delaying reflector '{}' due to "
                                        "missing dependency '{}'",
                                        reflector->implementation_name(),
                                        dep);
                        ++it;
                        continue;
                    }
                }
                MGE_DEBUG_TRACE(REFLECTION,
                                "Reflecting types using reflector '{}'",
                                reflector->implementation_name());
                reflector->reflect();
                reflectors_done.insert(
                    std::string(reflector->implementation_name()));
                auto aliases = reflector->alias_names();
                if (!aliases.empty()) {
                    size_t start = 0;
                    size_t end = aliases.find(' ');
                    while (end != std::string_view::npos) {
                        reflectors_done.insert(
                            std::string(aliases.substr(start, end - start)));
                        start = end + 1;
                        end = aliases.find(' ', start);
                    }
                    reflectors_done.insert(std::string(aliases.substr(start)));
                }
                it = reflectors.erase(it);
            }

            if (reflectors.size() == current_size) {
                MGE_WARNING_TRACE(
                    REFLECTION,
                    "No progress in reflector resolution, possible "
                    "cyclic dependency?");
                break;
            }
            current_size = reflectors.size();
        }

        if (!reflectors.empty()) {
            for (const auto& reflector : reflectors) {
                const auto  deps = reflector->dependencies();
                std::string deps_str;
                for (const auto& dep : deps) {
                    if (!deps_str.empty()) {
                        deps_str += ", ";
                    }
                    deps_str += dep;
                }
                MGE_WARNING_TRACE(REFLECTION,
                                  "Reflector '{}' could not be resolved, "
                                  "dependencies: [{}]",
                                  reflector->implementation_name(),
                                  deps_str);
            }
        }
    }

    MGE_REGISTER_COMPONENT(reflector);

} // namespace mge::reflection