#pragma once
#include <type_traits>
namespace mge::script {

    /**
     * @brief Proxy class for implementing interfaces by script methods.
     *
     * @tparam T implemented interface
     */
    template <typename T> class proxy : public T
    {
    public:
        using T::T;
    };

} // namespace mge::script