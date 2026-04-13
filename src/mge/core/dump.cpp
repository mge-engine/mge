// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dump.hpp"
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/markdown_document.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/trace.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#else
#    include <unistd.h>
#endif

namespace mge {

    MGE_DEFINE_TRACE(DUMP);

    MGE_REGISTER_COMPONENT(dump_info_provider);

    static std::string dump_timestamp()
    {
        auto      now = std::chrono::system_clock::now();
        auto      ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now.time_since_epoch()) %
                       1000;
        auto      time = std::chrono::system_clock::to_time_t(now);
        struct tm tm_buf;
#ifdef MGE_OS_WINDOWS
        localtime_s(&tm_buf, &time);
#else
        localtime_r(&time, &tm_buf);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm_buf, "%Y%m%d%H%M%S") << std::setw(3)
            << std::setfill('0') << ms.count();
        return oss.str();
    }

    static std::string dump_filename()
    {
        auto        name = executable_name();
        std::string result(name.begin(), name.end());
        result += "-crash-";
        result += dump_timestamp();
        result += ".md";
        return result;
    }

    dump::dump(std::string_view          purpose,
               std::pmr::memory_resource* resource)
        : m_purpose(purpose, resource)
        , m_resource(resource)
    {
        MGE_INFO_TRACE(DUMP, "Creating dump for {}", purpose);
    }

    dump::~dump() {}

    static void write_dump(const stacktrace&          st,
                           std::pmr::memory_resource* resource =
                               std::pmr::get_default_resource())
    {
        auto filename = dump_filename();

        MGE_INFO_TRACE(DUMP, "Writing crash dump to {}", filename);

        std::ofstream os(filename);
        if (!os) {
            MGE_ERROR_TRACE(DUMP, "Cannot open dump file: {}", filename);
            return;
        }

        markdown_document doc(resource);

        doc.heading(1, "Crash Dump");

        std::pmr::vector<std::pmr::string> info_items(resource);
        info_items.emplace_back(
            std::pmr::string("Executable: ", resource) +
            executable_name().c_str());
        info_items.emplace_back(
            std::pmr::string("Timestamp: ", resource) +
            dump_timestamp().c_str());
#ifdef MGE_OS_WINDOWS
        info_items.emplace_back(
            std::pmr::string("Process ID: ", resource) +
            std::to_string(GetCurrentProcessId()).c_str());
#else
        info_items.emplace_back(
            std::pmr::string("Process ID: ", resource) +
            std::to_string(getpid()).c_str());
#endif
        doc.unordered_list(info_items);

        std::ostringstream st_stream;
        st_stream << st;
        doc.heading(2, "Stack Trace")
            .code_block(st_stream.str());

        dump_info_provider::implementations(
            [&doc, resource](std::string_view implementation_name) {
                try {
                    auto provider =
                        dump_info_provider::create(implementation_name);
                    if (provider) {
                        doc.heading(2, provider->section_name(resource));
                        provider->dump_info(doc);
                    }
                } catch (...) {
                    doc.heading(2, implementation_name)
                        .paragraph("Error collecting dump info.");
                }
            });

        os << doc;
        os.flush();
        MGE_INFO_TRACE(DUMP, "Crash dump written to {}", filename);
    }

#ifdef MGE_OS_WINDOWS

    static LONG
        WINAPI unhandled_exception_filter(EXCEPTION_POINTERS* exception_info)
    {
        HANDLE  thread = GetCurrentThread();
        CONTEXT context = *exception_info->ContextRecord;

        stacktrace st(thread, &context);
        write_dump(st);

        return EXCEPTION_EXECUTE_HANDLER;
    }

    void dump::install_handler()
    {
        SetUnhandledExceptionFilter(unhandled_exception_filter);
    }

    void dump::uninstall_handler()
    {
        SetUnhandledExceptionFilter(nullptr);
    }

#else

    void dump::install_handler()
    {
        // No automatic crash handler on non-Windows platforms yet
    }

    void dump::uninstall_handler()
    {
        // No automatic crash handler on non-Windows platforms yet
    }

#endif

} // namespace mge
