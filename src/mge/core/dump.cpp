// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dump.hpp"
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/trace.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
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
        return executable_name() + "-crash-" + dump_timestamp() + ".md";
    }

    dump::dump(std::string_view purpose)
    {
        MGE_INFO_TRACE(DUMP, "Creating dump for {}", purpose);
    }

    dump::~dump() {}

    static void write_dump(const stacktrace& st)
    {
        auto filename = dump_filename();

        MGE_INFO_TRACE(DUMP, "Writing crash dump to {}", filename);

        std::ofstream os(filename);
        if (!os) {
            MGE_ERROR_TRACE(DUMP, "Cannot open dump file: {}", filename);
            return;
        }

        os << "# Crash Dump" << std::endl;
        os << std::endl;
        os << "- Executable: " << executable_name() << std::endl;
        os << "- Timestamp: " << dump_timestamp() << std::endl;
        os << std::endl;

        os << "## Stack Trace" << std::endl;
        os << std::endl;
        os << "```" << std::endl;
        os << st;
        os << "```" << std::endl;
        os << std::endl;

        dump_info_provider::implementations(
            [&os](std::string_view implementation_name) {
                try {
                    auto provider =
                        dump_info_provider::create(implementation_name);
                    if (provider) {
                        os << "## " << provider->section_name() << std::endl;
                        os << std::endl;
                        provider->dump_info(os);
                        os << std::endl;
                    }
                } catch (...) {
                    os << "## " << implementation_name << std::endl;
                    os << std::endl;
                    os << "Error collecting dump info." << std::endl;
                    os << std::endl;
                }
            });

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
