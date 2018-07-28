// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/math/vector_math.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    /**
     * Two element vector.
     */
    template <typename T>
    class vec2 : public vector_expression<mge::vec2<T>, T, 2>
    {
    public:
        typedef T value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* pointer;
        typedef const T* const_pointer;

        vec2()
        {}

        explicit vec2(T scalar)
            :x(scalar), y(scalar)
        {}

        vec2(T x_, T y_)
        :x(x_), y(y_)
        {}

        vec2(const vec2<T>& v)
            :x(v.x), y(v.y)
        {}

        template <typename U>
        vec2(const vec2<U>& v)
            :x(v.x), y(v.y)
        {}

        template <typename E>
        explicit vec2(const vector_expression<E, T, 2>& e)
        {
            x = e(0);
            y = e(1);
        }

        explicit vec2(const T* data)
            :x(data[0]), y(data[1])
        {}

        explicit vec2(const std::initializer_list<T>& l)
        {
            if(l.size() < 2) {
                throw MOGE_EXCEPTION(out_of_range) << "initializer list has too few elements";
            }
            x = *l.begin();
            y = *(l.begin() + 1);
        }

        vec2<T>& operator=(const vec2<T>& v)
        {
            x = v.x;
            y = v.y;
            return *this;
        }

        template <typename E>
        vec2<T>& operator=(const vector_expression<E, T, 2>& e)
        {
            T t0 = e(0);
            T t1 = e(1);
            x = t0;
            y = t1;
            return *this;
        }

        template <typename U>
        vec2<T>& operator +=(U scalar)
        {
             x += scalar;
             y += scalar;
             return *this;
        }

        template <typename U>
        vec2<T>& operator +=(const vec2<U>& v)
        {
             x += v.x;
             y += v.y;
             return *this;
        }

        template <typename U>
        vec2<T>& operator -=(U scalar)
        {
             x -= scalar;
             y -= scalar;
             return *this;
        }

        template <typename U>
        vec2<T>& operator -=(const vec2<U>& v)
        {
             x -= v.x;
             y -= v.y;
             return *this;
        }

        template <typename U>
        vec2<T>& operator *=(U scalar)
        {
             x *= static_cast<T>(scalar);
             y *= static_cast<T>(scalar);
             return *this;
        }

        template <typename U>
        vec2<T>& operator /=(U scalar)
        {
             x /= static_cast<T>(scalar);
             y /= static_cast<T>(scalar);
             return *this;
        }

        template <typename U>
        vec2<T>& operator %=(U scalar)
        {
             x %= static_cast<T>(scalar);
             y %= static_cast<T>(scalar);
             return *this;
        }

        template <typename U>
        vec2<T>& operator |=(U scalar)
        {
             x |= static_cast<T>(scalar);
             y |= static_cast<T>(scalar);
             return *this;
        }

        template <typename U>
        vec2<T>& operator ^=(U scalar)
        {
             x ^= static_cast<T>(scalar);
             y ^= static_cast<T>(scalar);
             return *this;
        }

        vec2<T>& operator ++()
        {
            ++x;
            ++y;
            return *this;
        }

        vec2<T>& operator --()
        {
            --x;
            --y;
            return *this;
        }

        vec2<T> operator ++(int)
        {
            vec2<T> result(*this);
            ++x;
            ++y;
            return result;
        }

        vec2<T> operator --(int)
        {
            vec2<T> result(*this);
            --x;
            --y;
            return result;
        }

        vec2<T> operator -() const
        {
            return vec2<T>(-x, -y);
        }


        /**
         * Return dot product of this vector and @c v.
         * @param v argument to dot product
         * @return dot product
         */
        template <typename U>
        auto dot(const vec2<U>& v) const
        {
            return x * v.x + y * v.y;
        }

        reference at(const size_type i)
        {
            if(i>=2) {
                MGE_THROW(out_of_range());
            }
            return (&x)[i];
        }

        const_reference at(const size_type i) const
        {
            if(i>=2) {
                MGE_THROW(out_of_range());
            }
            return (&x)[i];
        }

        const_reference operator[](const size_type i) const
        {
            return (&x)[i];
        }

        reference operator[](const size_type i)
        {
            return (&x)[i];
        }

        /**
         * Access vector element.
         * @param index element index
         * @return request element
         */
        T operator() (std::size_t index) const
        {
            return  (&x)[index];
        }

        T x;
        T y;
    };

    template <typename T>
    inline vec2<T> abs(const vec2<T>& v)
    {
        using namespace std;
        return vec2<T>(std::abs(v.x), std::abs(v.y));
    }

    /**
     * Integer vector.
     */
    typedef vec2<int32_t> vec2i;

    /**
     * Float vector.
     */
    typedef vec2<float> vec2f;

    /**
     * Double vector.
     */
    typedef vec2<double> vec2d;
}

