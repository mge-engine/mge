// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

namespace mge::dx12 {
    MGE_DEFINE_EXCEPTION_CLASS(error);

    error& error::set_info_from_hresult(HRESULT     rc,
                                        const char* file,
                                        int         line,
                                        const char* clazz,
                                        const char* method)
    {
        std::string called_function(clazz);
        called_function.append("::");
        called_function.append(method);
        set_info(mge::exception::source_file(file))
                .set_info(mge::exception::source_line(line))
                .set_info(
                    mge::exception::called_function(called_function.c_str()))
                .set_info(mge::exception::stack(mge::stacktrace()))
                .set_info(
                    mge::exception::type_name(mge::type_name<dx12::error>()))
            << "Call to " << called_function << " failed: " << std::hex << rc;
        return *this;
    }

    void error::check_hresult(HRESULT     rc,
                              const char* file,
                              int         line,
                              const char* clazz,
                              const char* method)
    {
        if (rc == S_OK) {
            return;
        } else {
            dx12::error err;
            throw err.set_info_from_hresult(rc, file, line, clazz, method);
        }
    }

    static const char* dxgi_message(HRESULT rc)
    {
        switch (rc) {
        case DXGI_ERROR_ACCESS_DENIED:
            return "Access denied";
        case DXGI_ERROR_ACCESS_LOST:
            return "Access lost";
        case DXGI_ERROR_ALREADY_EXISTS:
            return "Already exists";
        case DXGI_ERROR_CANNOT_PROTECT_CONTENT:
            return "Cannot protect content";
        case DXGI_ERROR_DEVICE_HUNG:
            return "Device hung";
        case DXGI_ERROR_DEVICE_REMOVED:
            return "Device removed";
        case DXGI_ERROR_DEVICE_RESET:
            return "Device reset";
        case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
            return "Driver internal error";
        case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:
            return "Frame statistics disjoint";
        case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:
            return "Graphics vidpn source in use";
        case DXGI_ERROR_INVALID_CALL:
            return "Invalid call";
        case DXGI_ERROR_MORE_DATA:
            return "More data";
        case DXGI_ERROR_NAME_ALREADY_EXISTS:
            return "Name already exists";
        case DXGI_ERROR_NONEXCLUSIVE:
            return "Nonexclusive";
        case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
            return "Not currently available";
        case DXGI_ERROR_NOT_FOUND:
            return "Not found";
        case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:
            return "Remote client disconnected";
        case DXGI_ERROR_REMOTE_OUTOFMEMORY:
            return "Remote out of memory";
        case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:
            return "Restrict to output stale";
        case DXGI_ERROR_SDK_COMPONENT_MISSING:
            return "SDK component missing";
        case DXGI_ERROR_SESSION_DISCONNECTED:
            return "Session disconnected";
        case DXGI_ERROR_UNSUPPORTED:
            return "Unsupported";
        case DXGI_ERROR_WAIT_TIMEOUT:
            return "Wait timeout";
        case DXGI_ERROR_WAS_STILL_DRAWING:
            "Was still drawing";
        default:
            return "Unknown error";
        }
    }

    void error::check_dxgi_hresult(HRESULT     rc,
                                   const char* file,
                                   int         line,
                                   const char* clazz,
                                   const char* method)
    {
        if (rc == S_OK) {
            return;
        } else {
            dx12::error err;
            std::string called_function(clazz);
            called_function.append("::");
            called_function.append(method);
            err.set_info(mge::exception::source_file(file))
                    .set_info(mge::exception::source_line(line))
                    .set_info(mge::exception::called_function(
                        called_function.c_str()))
                    .set_info(mge::exception::stack(mge::stacktrace()))
                    .set_info(mge::exception::type_name(
                        mge::type_name<dx12::error>()))
                << "Call to " << called_function << " failed: " << std::hex
                << rc << " (" << dxgi_message(rc) << ")";
        }
    }

} // namespace mge::dx12