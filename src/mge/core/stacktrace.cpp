// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#    include <windows.h>

#    include <DbgHelp.h>

#    include <TlHelp32.h>

extern "C" {
__declspec(dllimport) void RtlCaptureContext(CONTEXT*);
}
#    include <unordered_set>
#endif

#ifdef MGE_OS_LINUX
#    include <dlfcn.h>
#    include <stacktrace>
#endif

#include "mge/core/stacktrace.hpp"
#include <iostream>

namespace mge {
#ifdef MGE_OS_WINDOWS
    static bool                        symbols_initialized = false;
    static bool                        backtrace_possible = true;
    static std::unordered_set<HMODULE> modules;

    static std::string symbol_search_path()
    {
        std::string result;
        char        buffer[1024];

        if (GetCurrentDirectory(sizeof(buffer), buffer)) {
            result += buffer;
            result += ";";
        }

        if (GetModuleFileName(0, buffer, sizeof(buffer))) {
            size_t s = strlen(buffer) - 1;
            while (s != 0) {
                if (buffer[s] == '/' || buffer[s] == ':' || buffer[s] == '\\') {
                    break;
                }
                --s;
            }
            if (s != 0) {
                if (buffer[s] == ':') {
                    buffer[s + 1] = '\0';
                } else {
                    buffer[s] = '\0';
                }
                result += buffer;
                result += ";";
            }
        }

        return result;
    }

    static void initialize_symbols()
    {
        DWORD sym_options = SymGetOptions();
        sym_options |= SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS;
        SymSetOptions(sym_options);
        auto path = symbol_search_path();
        BOOL rc = SymInitialize(GetCurrentProcess(), path.c_str(), TRUE);
        if (!rc) {
            backtrace_possible = false;
        }
        symbols_initialized = true;
    }

    template <typename T>
    void fill_stacktrace(HANDLE       thread,
                         CONTEXT*     context,
                         T&           frames,
                         string_pool& strings)
    {
        if (!symbols_initialized) {
            initialize_symbols();
        }
        if (!backtrace_possible) {
            return;
        }

        HANDLE       current_process = GetCurrentProcess();
        STACKFRAME64 frame = {};

        frame.AddrPC.Offset = context->Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context->Rsp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context->Rsp;
        frame.AddrStack.Mode = AddrModeFlat;

        while (frames.size() < 100) {
            BOOL rc = StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                                  GetCurrentProcess(),
                                  thread,
                                  &frame,
                                  context,
                                  NULL,
                                  SymFunctionTableAccess64,
                                  SymGetModuleBase64,
                                  NULL);
            if (!rc || frame.AddrPC.Offset == 0) {
                break;
            }

            IMAGEHLP_MODULE64 moduleinfo;
            memset(&moduleinfo, 0, sizeof(moduleinfo));
            moduleinfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
            if (!SymGetModuleInfo64(current_process,
                                    frame.AddrPC.Offset,
                                    &moduleinfo)) {
                moduleinfo.ImageName[0] = '\0';
            }

            // get symbol information
#    define MAX_SYMBOL_NAME_LEN 2000
            DWORD64 symbolinfo_buffer[(sizeof(IMAGEHLP_SYMBOL64) +
                                       MAX_SYMBOL_NAME_LEN) /
                                          sizeof(DWORD64) +
                                      1];
            static IMAGEHLP_SYMBOL64* p_symbolinfo;
            static DWORD64            symboldisplacement;
            const char*               methodname;

            p_symbolinfo = (IMAGEHLP_SYMBOL64*)symbolinfo_buffer;
            p_symbolinfo->SizeOfStruct = sizeof(symbolinfo_buffer);
            p_symbolinfo->MaxNameLength = MAX_SYMBOL_NAME_LEN;
            symboldisplacement = 0;

            if (SymGetSymFromAddr64(current_process,
                                    frame.AddrPC.Offset,
                                    &symboldisplacement,
                                    p_symbolinfo)) {
                methodname = (const char*)&(p_symbolinfo->Name[0]);
            } else {
                methodname = "";
            }

            static IMAGEHLP_LINE64 lineinfo;
            static DWORD           linedisplacement;
            lineinfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            if (SymGetLineFromAddr64(current_process,
                                     frame.AddrPC.Offset,
                                     &linedisplacement,
                                     &lineinfo)) {
                frames.emplace_back((void*)frame.AddrPC.Offset,
                                    strings.get(moduleinfo.ImageName),
                                    strings.get(methodname),
                                    strings.get(lineinfo.FileName),
                                    lineinfo.LineNumber);
            } else {
                frames.emplace_back((void*)frame.AddrPC.Offset,
                                    strings.get(moduleinfo.ImageName),
                                    strings.get(""),
                                    strings.get(""),
                                    0);
            }
        }
        CloseHandle(current_process);
    }

    template <typename T> void fill_stacktrace(T& frames, string_pool& strings)
    {
        HANDLE  current_thread = GetCurrentThread();
        CONTEXT context = {};
        context.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&context);
        fill_stacktrace(current_thread, &context, frames, strings);
        CloseHandle(current_thread);
    }

#elif defined(MGE_OS_LINUX)
    // Linux/Unix implementation using dladdr to get module information
    template <typename T> void fill_stacktrace(T& frames, string_pool& strings)
    {
        auto st = std::stacktrace::current();
        auto it = st.cbegin();
        ++it;
        while (it != st.cend()) {
            const auto& frame = *it;
            ++it;
            const void* address =
                reinterpret_cast<const void*>(frame.native_handle());

            // Use dladdr to find which module the address belongs to
            Dl_info     dl_info;
            const char* module_name = "";
            if (dladdr(address, &dl_info) && dl_info.dli_fname) {
                module_name = dl_info.dli_fname;
            }

            frames.emplace_back(address,
                                strings.get(module_name),
                                strings.get(frame.description()),
                                strings.get(frame.source_file()),
                                frame.source_line());
        }
    }
#else

#endif
    stacktrace::frame::frame(const void*      address,
                             std::string_view module,
                             std::string_view name,
                             std::string_view file,
                             uint32_t         line)
        : m_address(address)
        , m_name(name)
        , m_source_file(file)
        , m_module(module)
        , m_source_line(line)
    {}

    stacktrace::stacktrace()
    {
        fill_stacktrace(m_frames, m_strings);
    }

    stacktrace::stacktrace(const stacktrace& src)
    {
        for (const auto& f : src.m_frames) {
            m_frames.emplace_back(f.address(),
                                  m_strings.get(f.module()),
                                  m_strings.get(f.name()),
                                  m_strings.get(f.source_file()),
                                  f.source_line());
        }
    }

    stacktrace& stacktrace::operator=(const stacktrace& src)
    {
        if (&src != this) {
            decltype(m_frames) tmp_frames;
            for (const auto& f : src.m_frames) {
                tmp_frames.emplace_back(f.address(),
                                        m_strings.get(f.module()),
                                        m_strings.get(f.name()),
                                        m_strings.get(f.source_file()),
                                        f.source_line());
            }
            m_frames = std::move(tmp_frames);
        }
        return *this;
    }

    bool stacktrace::operator==(const stacktrace& s) const
    {
        if (m_frames.size() == s.m_frames.size()) {
            for (size_t i = 0; i < m_frames.size(); ++i) {
                if (m_frames[i].address() != s.m_frames[i].address()) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool stacktrace::operator!=(const stacktrace& s) const
    {
        if (m_frames.size() == s.m_frames.size()) {
            for (size_t i = 0; i < m_frames.size(); ++i) {
                if (m_frames[i].address() != s.m_frames[i].address()) {
                    return true;
                }
            }
            return false;
        }
        return true;
    }

    stacktrace::size_type stacktrace::size() const
    {
        return m_frames.size();
    }

#if 0
    void stacktrace::fill(const std::thread &t)
    {
        HANDLE thread_handle =
            (HANDLE) const_cast<std::thread &>(t).native_handle();

        // if thread is gone meanwhile, leave
        if (!thread_handle) {
            return;
        }
        // the retrieval method won't work if we are calling
        // in this thread
        if (GetCurrentThread() == thread_handle) {
            fill();
            return;
        }
        CONTEXT context      = {};
        context.ContextFlags = CONTEXT_FULL;
        DWORD rc             = SuspendThread(thread_handle);
        if (rc == (DWORD)-1) {
            // MGE_THROW_SYSCALL_ERROR(SuspendThread);
        }
        BOOL getcontext_rc = GetThreadContext(thread_handle, &context);
        if (!getcontext_rc) {
            ResumeThread(thread_handle);
            // MGE_THROW_SYSCALL_ERROR(GetThreadContext);
        }
        fill(thread_handle, &context);
        rc = ResumeThread(thread_handle);
        if (rc == (DWORD)-1) {
            // MGE_THROW_SYSCALL_ERROR(ResumeThread);
        }
    }
#endif
} // namespace mge
