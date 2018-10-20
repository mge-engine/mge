// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"
#include "mge/core/stdexceptions.hpp"
#include <boost/algorithm/string/predicate.hpp>

namespace mge {
    void
    system_error::clear()
    {
#ifdef MGE_OS_WINDOWS
        SetLastError(0);
#else
#  error Not implemented.
#endif
    }

    system_error::error::error()
    {
#ifdef MGE_OS_WINDOWS
        m_error_value = GetLastError();

        char *msgbuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                      nullptr,
                      (DWORD)m_error_value,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPTSTR)&msgbuf,
                      0,
                      nullptr);
        std::stringstream ss;
        ss << "(" << std::hex << m_error_value << std::dec << "): " << msgbuf;
        LocalFree(msgbuf);
        m_error_message = ss.str();
#else
#  error not implemented
#endif
    }

    const char *
    system_error::what() const
    {
        if (!m_message.empty()) {
            return m_message.c_str();
        }

        m_message = mge::exception::what();
        if(!m_message.empty()) {
            if(boost::algorithm::ends_with(m_message, ":")) {
               m_message.append(" ");
            } else if(!boost::algorithm::ends_with(m_message, ": ")) {
               m_message.append(": ");
            }
        }

        auto err = get<mge::system_error::error>();
        if(err) {
            m_message.append(std::get<1>(err.value()));
            return m_message.c_str();
        } else {
            m_message.clear();
        }

        return mge::exception::what();
    }

    MGE_DEFINE_EXCEPTION(system_error)
}
