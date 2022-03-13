#pragma once
#include <type_traits>
namespace mge::script {

    template <typename T> class proxy : public T
    {
    public:
        using T::T;
    }

} // namespace mge::script