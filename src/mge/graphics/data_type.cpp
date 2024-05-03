// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include <iostream>
#include <map>
#include <string_view>

namespace mge {

    data_type parse_data_type(std::string_view sv)
    {
        using namespace std::literals;

        auto it = sv.begin();
        auto e = sv.end();
        if (it != e) {
            switch (*it) {
            case 'i':
                ++it;
                if (it != e && *it == 'n') {
                    ++it;
                    if (it != e && *it == 't') {
                        ++it;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
                if (it != e) {
                    switch (*it) {
                    case '1':
                        ++it;
                        if (it != e) {
                            if (*it == '6') {
                                ++it;
                                if (it == e) {
                                    return data_type::INT16;
                                }
                            } else if (*it == '2') {
                                ++it;
                                if (it != e && *it == '8') {
                                    ++it;
                                    if (it == e) {
                                        return data_type::INT128;
                                    }
                                }
                            }
                        }
                        break;
                    case '3':
                        ++it;
                        if (it != e && *it == '2') {
                            ++it;
                            if (it == e) {
                                return data_type::INT32;
                            }
                        }
                    case '6':
                        ++it;
                        if (it != e && *it == '4') {
                            ++it;
                            if (it == e) {
                                return data_type::INT64;
                            }
                        }
                    case '8':
                        ++it;
                        if (it == e) {
                            return data_type::INT8;
                        }
                    default:
                        break;
                    }
                }
                break;
            case 'u':
                ++it;
                if (it != e && *it == 'i') {
                    ++it;
                    if (it != e && *it == 'n') {
                        ++it;
                        if (it != e && *it == 't') {
                            ++it;
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                } else {
                    break;
                }
                if (it != e) {
                    switch (*it) {
                    case '1':
                        ++it;
                        if (it != e) {
                            if (*it == '6') {
                                ++it;
                                if (it == e) {
                                    return data_type::UINT16;
                                }
                            } else if (*it == '2') {
                                ++it;
                                if (it != e && *it == '8') {
                                    ++it;
                                    if (it == e) {
                                        return data_type::UINT128;
                                    }
                                }
                            }
                        }
                        break;
                    case '3':
                        ++it;
                        if (it != e && *it == '2') {
                            ++it;
                            if (it == e) {
                                return data_type::UINT32;
                            }
                        }
                    case '6':
                        ++it;
                        if (it != e && *it == '4') {
                            ++it;
                            if (it == e) {
                                return data_type::UINT64;
                            }
                        }
                    case '8':
                        ++it;
                        if (it == e) {
                            return data_type::UINT8;
                        }
                    default:
                        break;
                    }
                }
                break;
            case 'h':
                if (sv == "half"sv)
                    return data_type::HALF;
                break;
            case 'f':
                if (sv == "float"sv)
                    return data_type::FLOAT;
                break;
            case 'd':
                if (sv == "double"sv)
                    return data_type::DOUBLE;
                break;
            case 'l':
                if (sv == "long double"sv)
                    return data_type::LONG_DOUBLE;
                break;
            default:
                break;
            }
        }
        MGE_THROW(illegal_argument) << "Invalid data type definition: " << sv;
    }

} // namespace mge