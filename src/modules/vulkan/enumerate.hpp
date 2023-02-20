#pragma once
#include <vector>

namespace mge::vulkan {

    template <typename F, typename C = std::vector>
    void enumerate(const F& f, C& container)
    {
        uint32_t count = 0;
        f(&count, nullptr);
        container.resize(count);
        if (count) {
            f(&count, container.data());
        }
    }

} // namespace mge::vulkan