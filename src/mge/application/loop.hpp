// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/application/loop_target.hpp"
#include "mge/core/component.hpp"
#include "mge/core/noncopyable.hpp"

namespace mge {

    /**
     * @brief Interaction loop abstraction.
     *
     * While the loop target does not signal quitting, input is
     * collected from target, state is updated and presented.
     *
     */
    class MGEAPPLICATION_EXPORT loop : public component<loop>
    {
    public:
        loop() = default;
        virtual ~loop() = default;

        /**
         * @brief Run the loop
         * @param t loop target
         */
        virtual void run(loop_target& t) = 0;
    };

} // namespace mge
