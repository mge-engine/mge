// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/vector_expression.hpp"

namespace mge {

    /**
     * Vector expression for unary minus.
     */
    template <typename E, typename V, int C>
    class vector_minus_expression: public vector_expression<
            vector_minus_expression<E, V, C>,
            V, C>
    {
    private:
        const vector_expression<E, V, C>& m_expr;

    public:
        /// Expression value type.
        typedef V value_type;

        inline vector_minus_expression(const vector_expression<E, V, C>& expr)
            : m_expr(expr)
        {}

        inline value_type operator()(int index) const
        {
            return -m_expr(index);
        }
    };

    /**
     * Unary minus operator on a vector expression.
     * @param expr vector expression parameter
     * @return expression that yields the negated value for each element
     */
    template <typename E, typename V, int C>
    inline vector_minus_expression<E, V, C> operator -(
            const vector_expression<E, V, C>& expr)
    {
        return vector_minus_expression<E, V, C>(expr);
    }
}
