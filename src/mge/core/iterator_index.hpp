// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

namespace mge {

    template <typename C>
    typename C::size_type inline iterator_index(
        const C& container, const typename C::const_iterator& it)
    {
        return std::distance(container.cbegin(), it);
    }
} // namespace mge