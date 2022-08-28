#pragma once
#include "python.hpp"

#include <optional>
#include <typeindex>

namespace mge::python {

    class value_classification
    {
    public:
        enum type_class
        {
            TC_NONE,
            TC_BOOL,
            TC_LONG,
            TC_FLOAT,
            TC_UNICODE,
            TC_OBJECT
        };

        enum match_type
        {
            NO_MATCH,
            MATCH,
            MATCH_EXACT
        };

        value_classification(PyObject* obj);

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

        type_class                     m_type_class;
        std::optional<std::type_index> m_type_index;
    };
} // namespace mge::python