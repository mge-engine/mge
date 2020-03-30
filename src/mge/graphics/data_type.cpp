// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/core/stdexceptions.hpp"

#include <iostream>
namespace mge {
    std::ostream &operator<<(std::ostream &os, const data_type t)
    {
        if (t <= data_type::LAST) {
            const char *values[] = {
                "UNKNOWN",    "UINT8",      "INT8",        "UINT16",  "INT16",
                "HALF",       "UINT32",     "INT32",       "FLOAT",   "UINT64",
                "INT64",      "DOUBLE",     "LONG_DOUBLE", "UINT128", "INT128",
                "FLOAT_VEC2", "FLOAT_VEC3", "FLOAT_VEC4"};
            return os << values[(uint8_t)t];
        } else {
            return os << "INVALID(" << (uint8_t)t << ")";
        }
    }

    data_type_set::data_type_set() {}

    data_type_set::~data_type_set() {}

    data_type_set::data_type_set(const data_type_set &s) : m_data(s.m_data) {}

    data_type_set::data_type_set(const std::initializer_list<data_type> &types)
    {
        for (const auto &t : types) {
            (*this)[t] = true;
        }
    }

    data_type_set::reference data_type_set::operator[](data_type t)
    {
        return m_data[static_cast<uint8_t>(t) - 1];
    }

    bool data_type_set::operator[](data_type t) const
    {
        return m_data[static_cast<uint8_t>(t) - 1];
    }

    bool data_type_set::test(data_type t) const
    {
        if (t == data_type::UNKNOWN || t > data_type::LAST) {
            MGE_THROW(mge::illegal_argument)
                << "Invalid type " << t << " to test";
        }
        return operator[](t);
    }

    data_type_set &data_type_set::operator=(const data_type_set &s)
    {
        m_data = s.m_data;
        return *this;
    }

    bool data_type_set::operator==(const data_type_set &s) const
    {
        return m_data == s.m_data;
    }

    bool data_type_set::operator!=(const data_type_set &s) const
    {
        return m_data == s.m_data;
    }

} // namespace mge
