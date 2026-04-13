// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/trace_topic.hpp"

namespace mge {

    class trace_dump_info_provider : public dump_info_provider
    {
    public:
        trace_dump_info_provider() = default;
        trace_dump_info_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Trace Configuration", m_resource);
        }

        void dump_info(markdown_document& doc) const override
        {
            bool globally_enabled = false;
            bool print_to_stdout = false;

            trace_topic::topics([&](const trace_topic& t) {
                if (t.global()) {
                    globally_enabled =
                        t.enabled(trace_level::DEBUG);
                }
            });

            if (std::getenv("MGE_TRACE_TO_STDOUT")) {
                print_to_stdout = true;
            }
            if (std::getenv("MGE_TRACE_ENABLED")) {
                globally_enabled = true;
            }

            std::pmr::vector<std::pmr::string> items(m_resource);
            items.emplace_back(std::pmr::string(
                globally_enabled ? "Global trace: enabled"
                                 : "Global trace: disabled",
                m_resource));
            items.emplace_back(std::pmr::string(
                print_to_stdout ? "Print to stdout: enabled"
                                : "Print to stdout: disabled",
                m_resource));
            doc.unordered_list(items);

            std::pmr::vector<std::pmr::string> enabled_topics(m_resource);
            trace_topic::topics([&](const trace_topic& t) {
                if (!t.global() && t.enabled_levels() != 0) {
                    std::pmr::string entry(t.name(), m_resource);
                    entry += " (";
                    bool first = true;
                    if (t.enabled_levels() &
                        static_cast<uint8_t>(trace_level::DEBUG)) {
                        entry += "DEBUG";
                        first = false;
                    }
                    if (t.enabled_levels() &
                        static_cast<uint8_t>(trace_level::INFO)) {
                        if (!first) entry += ", ";
                        entry += "INFO";
                        first = false;
                    }
                    if (t.enabled_levels() &
                        static_cast<uint8_t>(trace_level::WARNING)) {
                        if (!first) entry += ", ";
                        entry += "WARNING";
                        first = false;
                    }
                    if (t.enabled_levels() &
                        static_cast<uint8_t>(trace_level::LEVEL_ERROR)) {
                        if (!first) entry += ", ";
                        entry += "ERROR";
                        first = false;
                    }
                    if (t.enabled_levels() &
                        static_cast<uint8_t>(trace_level::FATAL)) {
                        if (!first) entry += ", ";
                        entry += "FATAL";
                    }
                    entry += ")";
                    enabled_topics.emplace_back(std::move(entry));
                }
            });

            if (!enabled_topics.empty()) {
                doc.heading(3, "Enabled Topics");
                doc.unordered_list(enabled_topics);
            }
        }
    };

    MGE_REGISTER_IMPLEMENTATION(trace_dump_info_provider,
                                dump_info_provider);

} // namespace mge
