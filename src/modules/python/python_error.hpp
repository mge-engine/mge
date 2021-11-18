// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
#include "mge/core/stdexceptions.hpp"
#include "python.hpp"

namespace mge::python {

    class error : public exception
    {
    public:
        error();
        error(const error& e);
        error(error&& e);
        ~error();

        error& operator=(const error& e);

        /**
         * @brief Python class (repr)
         */
        struct exception_class : public tag<exception_class, std::string>
        {
            /**
             * @brief Capture python class name that raised the error.
             *
             * @param clazz Python exception class.
             */
            exception_class(const std::string& clazz)
                : m_value(clazz)
            {}

            std::string_view value() const noexcept { return m_value; }

            std::string m_value;
        };

        template <typename Info> error& set_info(const Info& info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> error& operator<<(const T& value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

} // namespace mge::python