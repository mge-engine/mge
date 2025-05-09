// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_format.hpp"
#include <iostream>

namespace mge {
    vertex_format::vertex_format()
        : m_type(data_type::UNKNOWN)
        , m_size(0u)
    {}

    vertex_format::vertex_format(data_type t, size_t s)
        : m_type(t)
        , m_size(static_cast<uint8_t>(s))
    {
        if (s > 255u) {
            MGE_THROW(illegal_argument) << "Unsupported size " << s;
        }
    }

    size_t vertex_format::binary_size() const
    {
        return data_type_size(m_type) * m_size;
    }

    vertex_format parse_vertex_format(std::string_view sv)
    {
        auto bpos = sv.find('[');
        if (bpos == std::string_view::npos) {
            auto dt = mge::enum_cast<data_type>(sv);
            if (dt.has_value()) {
                return vertex_format(dt.value());
            }
            MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
        } else {
            std::string_view dtsv =
                std::string_view(sv.begin(), sv.begin() + bpos);
            auto dt = mge::enum_cast<data_type>(dtsv);
            if (!dt.has_value()) {
                MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
            }
            auto it = sv.begin() + bpos + 1;
            auto e = sv.end();
            if (it != e) {
                uint32_t size = 0;

                if ('0' <= *it && *it <= '9') {
                    size *= 10;
                    size += (*it) - '0';
                    ++it;
                } else {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if (it == e) {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if ('0' <= *it && *it <= '9') {
                    size *= 10;
                    size += (*it) - '0';
                    ++it;
                } else if (*it == ']') {
                    ++it;
                    if (it == e) {
                        if (size == 0) {
                            MGE_THROW(illegal_argument)
                                << "Invalid vertex format: " << sv;
                        }
                        return vertex_format(dt.value(), size);
                    } else {
                        MGE_THROW(illegal_argument)
                            << "Invalid vertex format: " << sv;
                    }
                } else {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if (it == e) {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if ('0' <= *it && *it <= '9') {
                    size *= 10;
                    size += (*it) - '0';
                    ++it;
                } else if (*it == ']') {
                    ++it;
                    if (it == e) {
                        if (size == 0) {
                            MGE_THROW(illegal_argument)
                                << "Invalid vertex format: " << sv;
                        }
                        return vertex_format(dt.value(), size);
                    } else {
                        MGE_THROW(illegal_argument)
                            << "Invalid vertex format: " << sv;
                    }
                } else {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if (it == e) {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if (size > 255 || size == 0) {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }

                if (*it == ']') {
                    ++it;
                    if (it == e) {
                        return vertex_format(dt.value(), size);
                    } else {
                        MGE_THROW(illegal_argument)
                            << "Invalid vertex format: " << sv;
                    }
                } else {
                    MGE_THROW(illegal_argument)
                        << "Invalid vertex format: " << sv;
                }
            } else {
                MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
            }
        }
    }

} // namespace mge