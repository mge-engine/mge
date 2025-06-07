// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <atomic>

namespace mge {

    template <typename T> bool increment_if_non_zero(std::atomic<T>& value)
    {
        T current = value.load(std::memory_order_relaxed);
        while (current != 0) {
            T desired = current + 1;
            if (value.compare_exchange_weak(current,
                                            desired,
                                            std::memory_order_acq_rel,
                                            std::memory_order_relaxed)) {
                return true;
            }
            // compare_exchange_weak failed but updated `current` to the
            // current value, so we can try again (after checking it is not
            // zero)
        }
        return false;
    }
} // namespace mge