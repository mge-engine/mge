// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/thread.hpp"

#ifdef MGE_OS_WINDOWS
#  include <windows.h>
#  include <DbgHelp.h>
#  include <TlHelp32.h>
extern "C" {
    __declspec(dllimport) void RtlCaptureContext(CONTEXT *);
}
#include <unordered_set>
#endif
#ifdef MGE_COMPILER_GNUC
extern "C" char *
__cxa_demangle(const char* __mangled_name, char* __output_buffer,
               size_t* __length, int* __status);
#endif


namespace mge {

    string_pool
    stacktrace::s_strings;
    std::mutex
    stacktrace::s_strings_lock;

    stacktrace::stacktrace()
    {
        fill();
    }

    stacktrace::stacktrace(const std::thread& t)
    {
        fill(t);
    }

    stacktrace::stacktrace(const thread& t)
    {
        fill(t.m_running_thread);
    }

    stacktrace::stacktrace(const stacktrace& s)
        :m_elements(s.m_elements)
    {}

    stacktrace::stacktrace(stacktrace&& s)
        :m_elements(std::move(s.m_elements))
    {}

    stacktrace&
    stacktrace::operator =(const stacktrace& s)
    {
        m_elements = s.m_elements;
        return *this;
    }

    stacktrace::~stacktrace()
    {}

    bool
    stacktrace::operator ==(const stacktrace& s) const
    {
        if(m_elements.size() == s.m_elements.size()) {
            for(size_t i=0; i<m_elements.size(); ++i) {
                if(m_elements[i].address != s.m_elements[i].address) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    bool
    stacktrace::operator !=(const stacktrace& s) const
    {
        return !operator ==(s);
    }

    stacktrace::size_type
    stacktrace::size() const
    {
        return m_elements.size();
    }

    stacktrace::const_iterator
    stacktrace::begin() const
    {
        return m_elements.begin();
    }

    stacktrace::const_iterator
    stacktrace::end() const
    {
        return m_elements.end();
    }


    std::ostream&
    operator <<(std::ostream& os, const stacktrace& b)
    {
        for(const auto& el: b) {
            if (el.file && *el.file) {
                os << (void*)el.address << " " << el.function << "(" << el.module << ") "
                   << el.file << ":" << el.line  << std::endl;
            } else {
                os << (void *)el.address << " " << el.function
                   << "(" << el.module << ")" << std::endl;
            }

        }
        return os;
    }


#ifdef MGE_OS_WINDOWS
    static bool symbols_initialized = false;
    static bool backtrace_possible = true;
    static std::unordered_set<HMODULE> modules;


    static std::string symbol_search_path()
    {
        std::string result;
        char buffer[1024];

        if (GetCurrentDirectory(sizeof(buffer), buffer)) {
            result += buffer;
            result += ";";
        }

        if (GetModuleFileName(0, buffer, sizeof(buffer))) {
            size_t s = strlen(buffer) - 1;
            while (s != 0) {
                if (buffer[s] == '/' || buffer[s] == ':'
                    || buffer[s] == '\\') {
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

#ifdef MGE_COMPILER_GNUC
    static std::string demangle(const char *src)
    {
        __SIZE_TYPE__ sz = 511;
        int status;
        char *buffer;
        buffer = (char *)alloca(sz + 1);
        __cxa_demangle(src, buffer, &sz, &status);
        if (status == 0) {
            return std::string(buffer);
        } else {
            return std::string(src);
        }
    }
#endif

    const char *
    stacktrace::intern_str(const char *str)
    {
        std::lock_guard<std::mutex> lock(s_strings_lock);
        return s_strings.intern(str);
    }


    void
    stacktrace::fill(const std::thread& t)
    {
        HANDLE thread_handle = (HANDLE)const_cast<std::thread&>(t).native_handle();

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
        CONTEXT context = {};
        context.ContextFlags = CONTEXT_FULL;
        DWORD rc = SuspendThread(thread_handle);
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

    void
    stacktrace::fill()
    {
        HANDLE current_thread = GetCurrentThread();
        CONTEXT context = {};
        context.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&context);
        fill(current_thread, &context);
        CloseHandle(current_thread);
    }

    void
    stacktrace::fill(HANDLE thread_handle,
                     CONTEXT *context)
    {
        if (!symbols_initialized) {
            initialize_symbols();
        }
        if (!backtrace_possible) {
            return;
        }

        HANDLE current_process = GetCurrentProcess();
        STACKFRAME64 frame = {};

        frame.AddrPC.Offset = context->Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context->Rsp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context->Rsp;
        frame.AddrStack.Mode = AddrModeFlat;

        while (size() < 100) {
            BOOL rc = StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                            GetCurrentProcess(), thread_handle,
                            &frame,
                            context,
                            NULL, SymFunctionTableAccess64,
                            SymGetModuleBase64,
                            NULL);
            if(!rc || frame.AddrPC.Offset == 0) {
                break;
            }


            IMAGEHLP_MODULE64 moduleinfo;
            memset(&moduleinfo, 0, sizeof(moduleinfo));
            moduleinfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
            if (!SymGetModuleInfo64(current_process,
                                    frame.AddrPC.Offset,
                                    &moduleinfo)) {
                strcpy(moduleinfo.ImageName, "");
            }

            // get symbol information
#define MAX_SYMBOL_NAME_LEN  2000
            DWORD64 symbolinfo_buffer[(sizeof(IMAGEHLP_SYMBOL64) + MAX_SYMBOL_NAME_LEN) / sizeof(DWORD64) + 1];
            static IMAGEHLP_SYMBOL64* p_symbolinfo;
            static DWORD64 symboldisplacement;
            const char *methodname;

            p_symbolinfo = (IMAGEHLP_SYMBOL64 *) symbolinfo_buffer;
            p_symbolinfo->SizeOfStruct = sizeof(symbolinfo_buffer);
            p_symbolinfo->MaxNameLength = MAX_SYMBOL_NAME_LEN;
            symboldisplacement = 0;

            if (SymGetSymFromAddr64(current_process,
                                    frame.AddrPC.Offset,
                                    &symboldisplacement,
                                    p_symbolinfo)) {
                methodname = (const char *) &(p_symbolinfo->Name[0]);
            } else {
                methodname = "";
            }

            static IMAGEHLP_LINE64 lineinfo;
            static DWORD linedisplacement;
            lineinfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            if (SymGetLineFromAddr64(current_process,
                                      frame.AddrPC.Offset,
                                      &linedisplacement, &lineinfo)) {
                m_elements.emplace_back(element((void *)frame.AddrPC.Offset,
                                                intern_str(methodname),
                                                intern_str(lineinfo.FileName),
                                                (int)lineinfo.LineNumber,
                                                intern_str(moduleinfo.ImageName)));
            } else {
                m_elements.emplace_back(element((void *)frame.AddrPC.Offset,
                                                intern_str(methodname),
                                                intern_str(""),
                                                0,
                                                intern_str(moduleinfo.ImageName)));
            }
#ifdef MGE_COMPILER_GNUC
            m_elements.back().function = demangle(m_elements.back().function.c_str());
#endif

        }
        CloseHandle(current_process);
    }
#else
    void
    backtrace::fill()
    {}

    void
    backtrace::fill(const std::thread&)
    {}
#endif



}

