// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
namespace mge::script {
    /**
     * @brief Primitive type classification.
     *
     * Basic types, expected to be built-in.
     */
    struct type_classification
    {
        bool is_pod() const
        {
            return is_char || is_short || is_int || is_long || is_float ||
                   is_double || is_bool;
        }

        unsigned int is_void : 1;
        unsigned int is_unsigned : 1;
        unsigned int is_enum : 1;
        unsigned int is_char : 1;
        unsigned int is_short : 1;
        unsigned int is_int : 1;
        unsigned int is_long : 1;
        unsigned int is_bool : 1;
        unsigned int is_float : 1;
        unsigned int is_double : 1;
        unsigned int is_class : 1;
    };

} // namespace mge::script