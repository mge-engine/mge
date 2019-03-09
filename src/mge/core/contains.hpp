// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include <algorithm>
#include <vector>

namespace mge {

    template <typename Container, typename Element>
    inline bool contains(const Container& c, const Element& e)
    {
        return c.find(e) != c.end();
    }

    template <typename Element>
    inline bool contains(const std::vector<Element>& c, const Element& e)
    {
        auto it = c.begin();
        while(it != c.end()) {
            if(*it == e) {
                return true;
            }
        }
        return false;
    }

}
