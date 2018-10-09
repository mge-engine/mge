// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"

namespace mge {

    exception::~exception()
    {}

    exception::exception()
    {}

    exception::exception(const exception& e)
        : std::exception(e)
        ,m_infos(e.m_infos)
    {}

    exception::exception(exception&& e)
        : std::exception(std::move(e))
        ,m_infos(std::move(e.m_infos))
    {}

    exception&
    exception::operator =(const exception& e)
    {
        std::exception::operator =(e);
        m_infos = e.m_infos;
        return *this;
    }

    exception&
    exception::operator =(exception&& e)
    {
        std::exception::operator =(std::move(e));
        m_infos = std::move(e.m_infos);
        return *this;
    }

    const char *
    exception::what() const
    {
        if (!m_message.empty()) {
            return m_message.c_str();
        }

        auto potential_message = get<mge::exception::message>();
        if(potential_message) {
            m_message = potential_message.value();
            return m_message.c_str();
        }

        return std::exception::what();
    }

}
