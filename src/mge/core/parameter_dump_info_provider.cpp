// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/parameter.hpp"

#include <algorithm>
#include <vector>

namespace mge {

    class parameter_dump_info_provider : public dump_info_provider
    {
    public:
        parameter_dump_info_provider() = default;
        parameter_dump_info_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Parameters", m_resource);
        }

        bool has_info() const override
        {
            bool found = false;
            configuration::for_each_parameter(
                [&](const basic_parameter& p) {
                    if (p.has_value()) {
                        found = true;
                    }
                });
            return found;
        }

        void dump_info(markdown_document& doc) const override
        {
            struct param_entry
            {
                std::pmr::string path;
                std::pmr::string value;
            };
            std::pmr::vector<param_entry> entries(m_resource);

            configuration::for_each_parameter(
                [&](const basic_parameter& p) {
                    if (p.has_value()) {
                        json::json j;
                        p.write_value(j);
                        auto path_str = p.path().generic_string();
                        auto value_str = j.dump();
                        entries.push_back(
                            {std::pmr::string(path_str, m_resource),
                             std::pmr::string(value_str, m_resource)});
                    }
                });

            std::sort(entries.begin(),
                      entries.end(),
                      [](const param_entry& a, const param_entry& b) {
                          return a.path < b.path;
                      });

            std::pmr::vector<std::pmr::string> headers(m_resource);
            headers.emplace_back(
                std::pmr::string("Parameter", m_resource));
            headers.emplace_back(
                std::pmr::string("Value", m_resource));

            std::pmr::vector<std::pmr::vector<std::pmr::string>> rows(
                m_resource);
            for (const auto& e : entries) {
                std::pmr::vector<std::pmr::string> row(m_resource);
                row.emplace_back(
                    doc.inline_code(e.path));
                row.emplace_back(
                    doc.inline_code(e.value));
                rows.emplace_back(std::move(row));
            }

            doc.table(headers, rows);
        }
    };

    MGE_REGISTER_IMPLEMENTATION(parameter_dump_info_provider,
                                dump_info_provider);

} // namespace mge
