// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/component.hpp"
#include "mge/core/dump_info_provider.hpp"

#include <algorithm>

namespace mge {

    class component_dump_info_provider : public dump_info_provider
    {
    public:
        component_dump_info_provider() = default;
        component_dump_info_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Components", m_resource);
        }

        bool has_info() const override
        {
            return !registered_components().empty();
        }

        void dump_info(markdown_document& doc) const override
        {
            struct component_entry
            {
                std::pmr::string component;
                std::pmr::string implementations;
            };

            std::pmr::vector<component_entry> entries(m_resource);

            auto components = registered_components();
            std::sort(components.begin(), components.end());

            for (const auto& c : components) {
                std::pmr::vector<std::pmr::string> impls(m_resource);
                component_base::implementations(
                    c,
                    [&](std::string_view impl_name) {
                        impls.emplace_back(
                            std::pmr::string(impl_name, m_resource));
                    });
                std::sort(impls.begin(), impls.end());

                std::pmr::string impl_list(m_resource);
                for (size_t i = 0; i < impls.size(); ++i) {
                    if (i > 0) {
                        impl_list += ", ";
                    }
                    impl_list += impls[i];
                }

                entries.push_back(
                    {std::pmr::string(c, m_resource),
                     std::move(impl_list)});
            }

            std::pmr::vector<std::pmr::string> headers(m_resource);
            headers.emplace_back(
                std::pmr::string("Component", m_resource));
            headers.emplace_back(
                std::pmr::string("Implementations", m_resource));

            std::pmr::vector<std::pmr::vector<std::pmr::string>> rows(
                m_resource);
            for (const auto& e : entries) {
                std::pmr::vector<std::pmr::string> row(m_resource);
                row.emplace_back(doc.inline_code(e.component));
                row.emplace_back(
                    e.implementations.empty()
                        ? std::pmr::string("(none)", m_resource)
                        : doc.inline_code(e.implementations));
                rows.emplace_back(std::move(row));
            }

            doc.table(headers, rows);
        }
    };

    MGE_REGISTER_IMPLEMENTATION(component_dump_info_provider,
                                dump_info_provider);

} // namespace mge
