// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"

namespace mge {
    static thread_local exception* t_current_exception = nullptr;

    exception::~exception()
    {
        if (t_current_exception == this) {
            t_current_exception = nullptr;
        }
    }

    exception::exception(const std::source_location& loc)
    {
        t_current_exception = this;

        set_info(source_file(loc.file_name()));
        set_info(source_line(loc.line()));
        set_info(function(loc.function_name()));
    }

    exception::exception(const exception& e)
        : std::exception(e)
        , m_infos(e.m_infos)
    {
        t_current_exception = this;
        copy_message_or_materialize(e);
    }

    exception::exception(exception&& e)
        : std::exception(std::move(e))
        , m_infos(std::move(e.m_infos))
        , m_raw_message_stream(std::move(e.m_raw_message_stream))
        , m_raw_message(std::move(e.m_raw_message))
    {
        t_current_exception = this;
    }

    mge::exception* exception::current_exception()
    {
        return t_current_exception;
    }

    exception& exception::operator=(const exception& e)
    {
        std::exception::operator=(e);
        m_infos = e.m_infos;
        copy_message_or_materialize(e);
        return *this;
    }

    exception& exception::operator=(exception&& e)
    {
        std::exception::operator=(std::move(e));
        m_infos = std::move(e.m_infos);
        m_raw_message_stream = std::move(e.m_raw_message_stream);
        m_raw_message = std::move(e.m_raw_message);
        return *this;
    }

    void exception::copy_message_or_materialize(const exception& e)
    {
        if (e.m_raw_message_stream) {
            m_raw_message = e.m_raw_message_stream->str();
        } else {
            m_raw_message = e.m_raw_message;
        }
    }

    void exception::materialize_message() const
    {
        if (m_raw_message_stream) {
            m_raw_message = m_raw_message_stream->str();
            m_raw_message_stream.reset();
        }
    }

    const char* exception::what() const noexcept
    {
        try {
            materialize_message();
            if (!m_raw_message.empty()) {
                return m_raw_message.c_str();
            }
        } catch (...) {
            // Handle any exceptions thrown during message materialization
        }
        return std::exception::what();
    }

    std::ostream& operator<<(std::ostream&                            os,
                             const mge::exception::exception_details& ed)
    {
        fmt::print(os, "{}", ed);
        return os;
    }

} // namespace mge