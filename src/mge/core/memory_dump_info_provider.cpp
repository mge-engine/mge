// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/memory_resource.hpp"
#include "mge/core/statistics.hpp"

#include <algorithm>

namespace mge {

    class memory_dump_info_provider : public dump_info_provider
    {
    public:
        memory_dump_info_provider() = default;
        memory_dump_info_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Memory", m_resource);
        }

        bool has_info() const override
        {
            return statistics::root().child("memory") != nullptr;
        }

        void dump_info(markdown_document& doc) const override
        {
            const statistics* mem = statistics::root().child("memory");
            if (!mem) {
                return;
            }

            struct entry
            {
                std::pmr::string name;
                uint64_t         allocations{0};
                uint64_t         deallocations{0};
                uint64_t         allocated_bytes{0};
            };

            std::pmr::vector<entry> entries(m_resource);

            for (auto* child : mem->children()) {
                entry e;
                e.name = std::pmr::string(child->name(), m_resource);
                const auto& desc = child->describe();
                for (statistics::description::size_type i = 0; i < desc.size();
                     ++i) {
                    const auto& f = desc.at(i);
                    auto        val = f.get(*child);
                    if (f.name() == "allocations") {
                        e.allocations = std::get<uint64_t>(val);
                    } else if (f.name() == "deallocations") {
                        e.deallocations = std::get<uint64_t>(val);
                    } else if (f.name() == "allocated_bytes") {
                        e.allocated_bytes = std::get<uint64_t>(val);
                    }
                }
                entries.push_back(std::move(e));
            }

            std::sort(entries.begin(),
                      entries.end(),
                      [](const entry& a, const entry& b) {
                          return a.allocated_bytes > b.allocated_bytes;
                      });

            std::pmr::vector<std::pmr::string> headers(m_resource);
            headers.emplace_back(std::pmr::string("Name", m_resource));
            headers.emplace_back(std::pmr::string("Allocations", m_resource));
            headers.emplace_back(std::pmr::string("Deallocations", m_resource));
            headers.emplace_back(
                std::pmr::string("Allocated Bytes", m_resource));

            std::pmr::vector<std::pmr::vector<std::pmr::string>> rows(
                m_resource);
            for (const auto& e : entries) {
                std::pmr::vector<std::pmr::string> row(m_resource);
                row.emplace_back(e.name);
                row.emplace_back(std::pmr::string(std::to_string(e.allocations),
                                                  m_resource));
                row.emplace_back(
                    std::pmr::string(std::to_string(e.deallocations),
                                     m_resource));
                row.emplace_back(
                    std::pmr::string(std::to_string(e.allocated_bytes),
                                     m_resource));
                rows.emplace_back(std::move(row));
            }

            doc.table(headers, rows);
        }
    };

    MGE_REGISTER_IMPLEMENTATION(memory_dump_info_provider, dump_info_provider);

} // namespace mge
