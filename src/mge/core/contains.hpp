// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.


namespace mge {

    template <typename Container, typename Element>
    inline bool contains(const Container& c, const Element& e)
    {
        return c.find(e) != c.end();
    }

}
