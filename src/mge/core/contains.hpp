// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include <algorithm>
#include <array>
#include <vector>

namespace mge {

    /**
     * @brief Check whether an element is contained in a container.
     *
     * @tparam Container container type
     * @tparam Element container element type
     * @param c container
     * @param e element
     * @return @c true if the container contains the element
     */
    template <typename Container, typename Element>
    inline bool contains(const Container& c, const Element& e)
    {
        return c.find(e) != c.end();
    }

    template <typename Element, class Alloc>
    inline bool contains(const std::vector<Element, Alloc>& c, const Element& e)
    {
        auto it = c.begin();
        while (it != c.end()) {
            if (*it == e) {
                return true;
            }
        }
        return false;
    }

    template <typename Element, size_t Size, class Alloc>
    inline bool contains(const small_vector<Element, Size, Alloc>& c,
                         const Element&                            e)
    {
        auto it = c.begin();
        while (it != c.end()) {
            if (*it == e) {
                return true;
            }
        }
        return false;
    }

    template <typename Element, size_t S>
    inline bool contains(const std::array<Element, S>& c, const Element& e)
    {
        auto it = c.begin();
        while (it != c.end()) {
            if (*it == e) {
                return true;
            }
        }
        return false;
    }

} // namespace mge
