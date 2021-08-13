// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
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

    size_t vertex_format::binary_size() const { return data_type_size(m_type) * m_size; }

    std::ostream& operator<<(std::ostream& os, const vertex_format& fmt)
    {
        if (fmt.size() == 1) {
            os << fmt.type();
        } else {
            os << fmt.type() << "[" << static_cast<uint32_t>(fmt.size()) << "]";
        }
        return os;
    }

    vertex_format parse_vertex_format(std::string_view sv)
    {
        auto bpos = sv.find('[');
        if (bpos == std::string_view::npos) {
            try {
                auto dt = parse_data_type(sv);
                return vertex_format(dt);
            } catch (const mge::exception& ex) {
                MGE_THROW_WITH_CAUSE(illegal_argument, ex) << "Invalid vertex format: " << sv;
            }
        } else {
            std::string_view dtsv = std::string_view(sv.begin(), sv.begin() + bpos);
            data_type        dt = data_type::UNKNOWN;
            try {
                dt = parse_data_type(dtsv);
            } catch (const mge::exception& ex) {
                MGE_THROW_WITH_CAUSE(illegal_argument, ex) << "Invalid vertex format: " << sv;
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
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if (it == e) {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if ('0' <= *it && *it <= '9') {
                    size *= 10;
                    size += (*it) - '0';
                    ++it;
                } else if (*it == ']') {
                    ++it;
                    if (it == e) {
                        if (size == 0) {
                            MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                        }
                        return vertex_format(dt, size);
                    } else {
                        MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                    }
                } else {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if (it == e) {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if ('0' <= *it && *it <= '9') {
                    size *= 10;
                    size += (*it) - '0';
                    ++it;
                } else if (*it == ']') {
                    ++it;
                    if (it == e) {
                        if (size == 0) {
                            MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                        }
                        return vertex_format(dt, size);
                    } else {
                        MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                    }
                } else {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if (it == e) {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if (size > 255 || size == 0) {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }

                if (*it == ']') {
                    ++it;
                    if (it == e) {
                        return vertex_format(dt, size);
                    } else {
                        MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                    }
                } else {
                    MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
                }
            } else {
                MGE_THROW(illegal_argument) << "Invalid vertex format: " << sv;
            }
        }
    }

} // namespace mge