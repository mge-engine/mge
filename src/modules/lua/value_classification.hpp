#pragma once
#include "lua.hpp"
#include "lua_type.hpp"

#include <typeindex>

namespace mge::lua {

    class value_classification
    {
    public:
        enum type_class
        {
            TC_NONE,
            TC_BOOL,
            TC_NUMBER,
            TC_STRING,
            TC_OBJECT
        };

        enum match_type
        {
            NO_MATCH,
            MATCH,
            MATCH_EXACT
        };

        value_classification();
        value_classification(lua_State* L, int index);

        value_classification(const value_classification&) = default;
        value_classification(value_classification&&) = default;
        value_classification& operator=(const value_classification&) = default;
        value_classification& operator=(value_classification&&) = default;

        /**
         * @brief Whether this classification matches with the supplied type
         * index.
         *
         * @param arg_type type index of argument to be matched
         * @return whether there is a match
         */
        match_type match(const std::type_index& arg_type) const;

        type_class m_type_class;
        lua::type* m_lua_type;
    };
} // namespace mge::lua