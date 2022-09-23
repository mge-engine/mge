// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "value_classification.hpp"
#include "lua_type.hpp"
#include "mge/core/singleton.hpp"

#include <map>

namespace mge::lua {

    class match_map
    {
    public:
        match_map()
        {
#define EXACT_MATCH(type, c_type)                                              \
    type##_map[std::type_index(typeid(c_type))] =                              \
        value_classification::MATCH_EXACT
#define MATCH(type, c_type)                                                    \
    type##_map[std::type_index(typeid(c_type))] = value_classification::MATCH

#define ALL_EXACT_MATCH(type, c_type)                                          \
    EXACT_MATCH(type, c_type);                                                 \
    EXACT_MATCH(type, const c_type);                                           \
    EXACT_MATCH(type, volatile c_type);                                        \
    EXACT_MATCH(type, const volatile c_type);                                  \
    EXACT_MATCH(type, const c_type&);                                          \
    EXACT_MATCH(type, const volatile c_type&)

#define ALL_MATCH(type, c_type)                                                \
    MATCH(type, c_type);                                                       \
    MATCH(type, const c_type);                                                 \
    MATCH(type, volatile c_type);                                              \
    MATCH(type, const volatile c_type);                                        \
    MATCH(type, const c_type&);                                                \
    MATCH(type, const volatile c_type&)

            ALL_EXACT_MATCH(bool, bool);
            ALL_MATCH(bool, int8_t);
            ALL_MATCH(bool, uint8_t);
            ALL_MATCH(bool, int16_t);
            ALL_MATCH(bool, uint16_t);
            ALL_MATCH(bool, int32_t);
            ALL_MATCH(bool, uint32_t);
            ALL_MATCH(bool, int64_t);
            ALL_MATCH(bool, uint64_t);

            //---
            ALL_EXACT_MATCH(number, int8_t);
            ALL_EXACT_MATCH(number, uint8_t);
            ALL_EXACT_MATCH(number, int16_t);
            ALL_EXACT_MATCH(number, uint16_t);
            ALL_EXACT_MATCH(number, int32_t);
            ALL_EXACT_MATCH(number, uint32_t);
            ALL_EXACT_MATCH(number, int64_t);
            ALL_EXACT_MATCH(number, uint64_t);
            ALL_EXACT_MATCH(number, float);
            ALL_EXACT_MATCH(number, double);
            //---
            EXACT_MATCH(string, std::string);
            EXACT_MATCH(string, const std::string&);
            EXACT_MATCH(string, const char*);
            EXACT_MATCH(string, std::string_view);
            EXACT_MATCH(string, const std::string_view&);

#undef ALL_EXACT_MATCH
#undef ALL_MATCH
#undef EXACT_MATCH
#undef MATCH
        }

        using type_map =
            std::map<std::type_index, value_classification::match_type>;

        type_map bool_map;
        type_map number_map;
        type_map string_map;
    };

    static mge::singleton<match_map> match_maps;

    value_classification::value_classification()
        : m_type_class(TC_NONE)
        , m_lua_type(nullptr)
    {}

    value_classification::value_classification(lua_State* L, int index)
        : m_type_class(TC_NONE)
        , m_lua_type(nullptr)
    {
        auto t = lua_type(L, index);
        switch (t) {
        case LUA_TBOOLEAN:
            m_type_class = TC_BOOL;
            break;
        case LUA_TNIL:
        case LUA_TNONE:
            m_type_class = TC_NONE;
            break;
        case LUA_TNUMBER:
            m_type_class = TC_NUMBER;
            break;
        case LUA_TSTRING:
            m_type_class = TC_STRING;
            break;
        case LUA_TTABLE:
        case LUA_TUSERDATA:
        case LUA_TLIGHTUSERDATA:
            m_type_class = TC_OBJECT;
            break;
        }
    }

    value_classification::match_type
    value_classification::match(const std::type_index& arg_type) const
    {
        switch (m_type_class) {
        case TC_BOOL: {
            auto it = match_maps->bool_map.find(arg_type);
            if (it != match_maps->bool_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_NUMBER: {
            auto it = match_maps->number_map.find(arg_type);
            if (it != match_maps->number_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_STRING: {
            auto it = match_maps->string_map.find(arg_type);
            if (it != match_maps->string_map.end()) {
                return it->second;
            }
            break;
        }
        case TC_NONE:
            return NO_MATCH;
        case TC_OBJECT:
            if (m_lua_type) {
                return m_lua_type->type_index() == arg_type ? MATCH_EXACT
                                                            : NO_MATCH;
            } else {
                return NO_MATCH;
            }
        }
        return NO_MATCH;
    }
} // namespace mge::lua
