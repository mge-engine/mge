// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file buffer_type.hpp
 *  @brief buffer type
 */
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>


namespace mge {

    /**
     * @brief The buffer type describes what data is inside a buffer.
     */
    enum class MGEGRAPHICS_EXPORT buffer_type : uint8_t
    {
        VERTEX,  //!< buffer containing vertex data
        INDEX,   //!< buffer containing indices, offsets into vertex data
        CONSTANT //!< buffer containing shader input values
    };
} // namespace mge