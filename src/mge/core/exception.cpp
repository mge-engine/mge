// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"

namespace mge {


    exception::exception(const exception& e)
        : std::exception(e)
        ,m_infos(e.m_infos)
    {
        copy_message_or_materialize(e);
    }

    exception::exception(exception&& e)
        : std::exception(std::move(e))
        ,m_infos(std::move(e.m_infos))
        ,m_raw_message_stream(std::move(e.m_raw_message_stream))
        ,m_raw_message(std::move(e.m_raw_message))
    {}

    exception&
    exception::operator =(const exception& e)
    {
        std::exception::operator =(e);
        m_infos = e.m_infos;
        copy_message_or_materialize(e);
        return *this;
    }

    exception&
    exception::operator =(exception&& e)
    {
        std::exception::operator =(std::move(e));
        m_infos = std::move(e.m_infos);
        m_raw_message_stream = std::move(e.m_raw_message_stream);
        m_raw_message = std::move(e.m_raw_message);
        return *this;
    }

    void
    exception::copy_message_or_materialize(const exception& e)
    {
        if(e.m_raw_message_stream) {
            m_raw_message = e.m_raw_message_stream->str();
        } else {
            m_raw_message = e.m_raw_message;
        }
    }

    void
    exception::materialize_message() const
    {
        if(m_raw_message_stream) {
            m_raw_message = m_raw_message_stream->str();
            m_raw_message_stream.reset();
        }
    }

    const char *
    exception::what() const
    {
        materialize_message();

        if (!m_raw_message.empty()) {
            return m_raw_message.c_str();
        }
        return std::exception::what();
    }

    std::ostream&
    operator <<(std::ostream& os, const mge::exception& ex)
    {
        return os << ex.what();
    }

    std::ostream&
    operator <<(std::ostream& os, const mge::exception::exception_details& d)
    {
        if(d.ex()) {
            os << "Exception details:" << std::endl;
            auto type = d.ex()->get<mge::exception::type_name>();
            if(type) {
                os << "Exception type: " << type.value() << std::endl;
            } else {
                os << "Exception type: unknown mge::exception" << std::endl;
            }

            auto file = d.ex()->get<mge::exception::source_file>();
            auto line = d.ex()->get<mge::exception::source_line>();
            if(file && line) {
                os <<   "Exception location: " << file.value() << ":" << line.value() << std::endl;
            }

            auto function = d.ex()->get<mge::exception::function>();
            if(function) {
                os << "Exception raising function: " << function.value() << std::endl;
            }
            auto called_function = d.ex()->get<mge::exception::called_function>();
            if(called_function) {
                os << "Calling library/system function: " << called_function.value() << std::endl;
            }

            auto stack = d.ex()->get<mge::exception::stack>();
            if(stack) {
                os << "Exception stack: " << stack.value() << std::endl;
            }

            os << "Exception message: " << d.ex()->what() << std::endl;

            auto cause = d.ex()->get<mge::exception::cause>();
            if(cause) {
                os << "Exception caused by: " << std::endl << cause.value().details();
            }

        } else {
            os << "Invalid exception details" << std::endl;
        }
        return os;
    }



}
