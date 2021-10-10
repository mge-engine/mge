// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include <cstdint>

namespace mge {

    /**
     * @brief Loop target pattern for interactive update.
     */
    class MGEAPPLICATION_EXPORT loop_target
    {
    public:
        virtual ~loop_target() = default;

        /**
         * @brief Retrieve whether the interactive loop shall quit.
         * @return @c true if the interaction loop shall quit
         */
        virtual bool is_quit() const = 0;

        /**
         * @brief Retrieve and process input in an loop step.
         * @param cycle frame counter
         */
        virtual void input(uint64_t cycle) = 0;

        /**
         * @brief Update state in a step.
         *
         * Note @c cycle may be the same for multiple calls.
         *
         * @param cycle frame counter
         * @param elapsed total elapsed time
         * @param delta delta in seconds
         */
        virtual void update(uint64_t cycle, double elapsed, double delta) = 0;

        /**
         * @brief Presents the state.
         * @param cycle frame counter
         * @param peek percentage of peek into next
         *   step if a fixed step mechanism is used,
         *   in range [0,1).
         */
        virtual void present(uint64_t cycle, double peek) = 0;
    };

} // namespace mge
