// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/module.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

namespace fs = std::filesystem;

using namespace std::string_view_literals;
namespace mge {

    MGE_USE_TRACE(CORE);

    module::module(const fs::path& path)
    {
        auto stem = path.stem();
        auto stem_str = stem.string();
#ifdef MGE_OS_WINDOWS
        m_name = std::string(stem_str.begin() + 11, stem_str.end());
#elif defined(MGE_OS_LINUX)
        m_name = std::string(stem_str.begin() + 14, stem_str.end());
#elif defined(MGE_OS_MACOSX)
        m_name = std::string(stem_str.begin() + 14, stem_str.end());
#else
#    error Missing port
#endif
        MGE_TRACE_OBJECT(CORE, DEBUG)
            << "Loading module '" << m_name << "' from shared library " << path;

        m_library = std::make_shared<shared_library>(path);
    }

    module::~module() {}

    std::string_view module::name() const
    {
        return m_name;
    }

    const shared_library_ref& module::library() const
    {
        return m_library;
    }

    class module_dictionary
    {
    public:
        module_dictionary() = default;
        ~module_dictionary() = default;

        std::map<std::string, module_ref> modules;
    };

    singleton<module_dictionary> s_all_modules;

    MGE_DEFINE_PARAMETER(std::string,
                         core,
                         module_path,
                         "Paths from which to load modules");

    static std::vector<fs::path>
    compute_load_paths(const std::vector<std::string>& paths)
    {
        std::vector<fs::path> load_paths;

        for (const auto& p : paths) {
            MGE_TRACE_OBJECT(CORE, DEBUG) << "Searching for modules in " << p;
            try {
                fs::path current_path(p);
                load_paths.push_back(fs::canonical(current_path));
            } catch (const std::exception& e) {
                MGE_TRACE_OBJECT(CORE, WARNING)
                    << "Exception inspecting module path " << p << ": "
                    << e.what();
            }
        }

        return load_paths;
    }

    static bool is_module(const fs::path& name)
    {
#ifdef MGE_OS_WINDOWS
        auto stem = name.stem();
        if (stem.string().starts_with("mge_module_")) {
            auto suffix = name.extension();
            if (suffix.string() == ".dll"sv) {
                return true;
            }
        }
#elif defined(MGE_OS_LINUX)
        auto stem = name.stem();
        if (stem.string().starts_with("libmge_module_")) {
            auto suffix = name.extension();
            if (suffix.string() == ".so"sv) {
                return true;
            }
        }
#elif defined(MGE_OS_MACOSX)
        auto stem = name.stem();
        if (stem.string().starts_with("libmge_module_")) {
            auto suffix = name.extension();
            if (suffix.string() == ".so"sv) {
                return true;
            }
        }
#else
#    error Missing port
#endif

        return false;
    }

    static std::vector<fs::path>
    enumerate_modules(std::vector<fs::path>& module_paths)
    {
        std::vector<fs::path> result;
        for (const auto& module_path : module_paths) {
            try {
                if (fs::is_directory(module_path)) {
                    for (const auto& de : fs::directory_iterator(module_path)) {
                        if (de.is_regular_file()) {
                            if (is_module(de.path())) {
                                result.push_back(de.path());
                            }
                        }
                    }
                }
            } catch (std::exception& e) {
                MGE_TRACE_OBJECT(CORE, WARNING)
                    << "Exception enumerating modules in path " << module_path
                    << ": " << e.what();
            }
        }
        return result;
    }

    void module::load_all()
    {
        std::vector<std::string> paths;
        paths.push_back(".");
        paths.push_back("modules");

        if (MGE_PARAMETER(core, module_path).has_value()) {
            std::string paths_str = MGE_PARAMETER(core, module_path).get();
            boost::split(paths,
                         paths_str,
                         boost::is_any_of(";"),
                         boost::token_compress_on);
        }

        std::vector<fs::path> load_paths = compute_load_paths(paths);

        std::vector<fs::path> modules = enumerate_modules(load_paths);

        for (const auto& m : modules) {
            auto current_ref = std::make_shared<module>(m);

            s_all_modules->modules.insert(
                std::make_pair(current_ref->name(), current_ref));
        }
    }

} // namespace mge
