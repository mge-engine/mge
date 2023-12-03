// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/attribute_semantic.hpp"
#include "mge/core/stdexceptions.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& o, const attribute_semantic& s)
    {
        switch (s) {
        case attribute_semantic::POSITION:
            return o << "POSITION";
        case attribute_semantic::NORMAL:
            return o << "NORMAL";
        case attribute_semantic::TANGENT:
            return o << "TANGENT";
        case attribute_semantic::BITANGENT:
            return o << "BITANGENT";
        case attribute_semantic::COLOR:
            return o << "COLOR";
        case attribute_semantic::TEXCOORD:
            return o << "TEXCOORD";
        case attribute_semantic::ANY:
            return o << "ANY";
        default:
            return o << "INVALID(" << static_cast<uint32_t>(s) << ")";
        }
    }

    attribute_semantic parse_attribute_semantic(std::string_view sv)
    {
        using namespace std::literals;

        auto it = sv.begin();
        auto e = sv.end();
        if (it != e) {
            switch (*it) {
            case 'P':
                if (sv == "POSITION"sv) {
                    return attribute_semantic::POSITION;
                }
                break;
            case 'N':
                if (sv == "NORMAL"sv) {
                    return attribute_semantic::NORMAL;
                }
                break;
            case 'T':
                if (sv == "TANGENT"sv) {
                    return attribute_semantic::TANGENT;
                } else if (sv == "TEXCOORD"sv) {
                    return attribute_semantic::TEXCOORD;
                }
                break;
            case 'B':
                if (sv == "BITANGENT"sv) {
                    return attribute_semantic::BITANGENT;
                }
                break;
            case 'C':
                if (sv == "COLOR"sv) {
                    return attribute_semantic::COLOR;
                }
                break;
            case 'A':
                if (sv == "ANY"sv) {
                    return attribute_semantic::ANY;
                }
                break;
            default:
                break;
            }
        }
        MGE_THROW(illegal_state) << "Invalid attribute semantic " << sv;
    }
} // namespace mge