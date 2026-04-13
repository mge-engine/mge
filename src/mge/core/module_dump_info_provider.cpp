// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/module.hpp"

#include <algorithm>

namespace mge {

    class module_dump_info_provider : public dump_info_provider
    {
    public:
        module_dump_info_provider() = default;
        module_dump_info_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Loaded Modules", m_resource);
        }

        bool has_info() const override
        {
            bool found = false;
            module::for_each_loaded_module(
                [&](const module&) { found = true; });
            return found;
        }

        void dump_info(markdown_document& doc) const override
        {
            struct module_entry
            {
                std::pmr::string name;
                std::pmr::string library_path;
            };

            std::pmr::vector<module_entry> entries(m_resource);

            module::for_each_loaded_module([&](const module& m) {
                auto path_str =
                    m.library()->name().generic_string();
                entries.push_back(
                    {std::pmr::string(m.name(), m_resource),
                     std::pmr::string(path_str, m_resource)});
            });

            std::sort(entries.begin(),
                      entries.end(),
                      [](const module_entry& a, const module_entry& b) {
                          return a.name < b.name;
                      });

            std::pmr::vector<std::pmr::string> headers(m_resource);
            headers.emplace_back(
                std::pmr::string("Module", m_resource));
            headers.emplace_back(
                std::pmr::string("Library", m_resource));

            std::pmr::vector<std::pmr::vector<std::pmr::string>> rows(
                m_resource);
            for (const auto& e : entries) {
                std::pmr::vector<std::pmr::string> row(m_resource);
                row.emplace_back(doc.inline_code(e.name));
                row.emplace_back(doc.inline_code(e.library_path));
                rows.emplace_back(std::move(row));
            }

            doc.table(headers, rows);
        }
    };

    MGE_REGISTER_IMPLEMENTATION(module_dump_info_provider,
                                dump_info_provider);

} // namespace mge
