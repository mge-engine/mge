// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/common.hpp"
#include <iostream>

namespace mge {

    /**
     * A vector expression of type @c E using value type @c V having
     * @c C components.
     */
    template <typename E, typename V, int C>
    class vector_expression
    {
    public:
        /// Expression value type.
        typedef V value_type;

        /**
         * Access base.
         * @return this as base type @c E
         */
        inline const E& base() const
        {
            return static_cast<const E&>(*this);
        }

        /**
         * Access member.
         * @param index member index
         * @return member value
         */
        inline value_type operator ()(int index) const
        {
            return base()(index);
        }

        /**
         * Access member.
         * @param index member index
         * @return member value
         */
        inline value_type get(int index) const
        {
            return base()(index);
        }

        /// Type of square length operation.
        typedef V square_length_type;

        /**
         * Compute square length.
         * @return square length of vector
         */
        inline square_length_type square_length() const
        {
            square_length_type result = (square_length_type) 0;
            for (int i = 0; i < C; ++i) {
                value_type v = get(i);
                result += v * v;
            }
            return result;
        }

        /// Type of length operation.
        typedef typename sqrt_type<value_type>::type length_type;

        /**
         * Get vector length.
         * @return vector length
         */
        inline length_type length() const
        {
            return moge::sqrt(square_length());
        }

        template <typename E2, typename V2>
        inline square_length_type square_distance(const vector_expression<E2, V2, C>& v) const
        {
            square_length_type r = 0;
            for(int i=0; i<C; ++i) {
                square_length_type d = get(i);
                d-= v.get(i);
                r += d*d;
            }
            return r;
        }

        /**
         * Comparison.
         * @param v vector to compare
         * @return @c true if vectors are equal
         */
        template <typename E2, typename V2>
        inline bool operator ==(const vector_expression<E2, V2, C>& v) const
        {
            for (int i = 0; i < C; ++i) {
                if (!(get(i) == v.get(i))) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Comparison.
         * @param v vector to compare
         * @return @c true if vectors are not equal
         */
        template <typename E2, typename V2>
        inline bool operator !=(const vector_expression<E2, V2, C>& v) const
        {
            for (int i = 0; i < C; ++i) {
                if (get(i) != v.get(i)) {
                    return true;
                }
            }
            return false;
        }

        template <typename E2, typename V2>
        inline bool operator <(const vector_expression<E2, V2, C>& v) const
        {
            for(int i=0; i<C; ++i) {
                V self = get(i);
                V2 other = v.get(i);
                if(self < other) {
                    return true;
                } else if(self == other) {
                    continue;
                } else {
                    return false;
                }
            }
            return false;
        }

        template <typename E2, typename V2>
        inline bool operator <=(const vector_expression<E2, V2, C>& v) const
        {
            return operator ==(v) || operator <(v);
        }

        template <typename E2, typename V2>
        inline bool operator >=(const vector_expression<E2, V2, C>& v) const
        {
            return !operator <(v);
        }

        template <typename E2, typename V2>
        inline bool operator >(const vector_expression<E2, V2, C>& v) const
        {
            return operator !=(v) || !operator <(v);
        }
    };

    template <typename E, typename V, int C>
    inline std::ostream& operator <<(std::ostream& os,
                                     const vector_expression<E, V, C>& v)
    {
        os << "[" << v(0);
        for (int i = 1; i < C; ++i) {
            os << ", " << v(i);
        }
        return os << "]";
    }
}
