// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/graphics/dllexport.hpp"
#include <cstdint>

namespace mge {

    /**
     * @brief Color write mask flags.
     *
     * Defines which color channels are written during rendering.
     * These flags can be combined using bitwise OR.
     */
    enum class color_write_mask : uint8_t
    {
        NONE = 0x00,                       //!< No color channels written
        RED = 0x01,                        //!< Write red channel
        GREEN = 0x02,                      //!< Write green channel
        BLUE = 0x04,                       //!< Write blue channel
        ALPHA = 0x08,                      //!< Write alpha channel
        RGB = RED | GREEN | BLUE,          //!< Write RGB channels
        RGBA = RED | GREEN | BLUE | ALPHA, //!< Write all channels
        ALL = RGBA
    };

    /**
     * @brief Bitwise OR operator for color write masks.
     */
    inline constexpr color_write_mask operator|(color_write_mask lhs,
                                                color_write_mask rhs)
    {
        return static_cast<color_write_mask>(static_cast<uint8_t>(lhs) |
                                             static_cast<uint8_t>(rhs));
    }

    /**
     * @brief Bitwise AND operator for color write masks.
     */
    inline constexpr color_write_mask operator&(color_write_mask lhs,
                                                color_write_mask rhs)
    {
        return static_cast<color_write_mask>(static_cast<uint8_t>(lhs) &
                                             static_cast<uint8_t>(rhs));
    }

    /**
     * @brief Bitwise XOR operator for color write masks.
     */
    inline constexpr color_write_mask operator^(color_write_mask lhs,
                                                color_write_mask rhs)
    {
        return static_cast<color_write_mask>(static_cast<uint8_t>(lhs) ^
                                             static_cast<uint8_t>(rhs));
    }

    /**
     * @brief Bitwise NOT operator for color write masks.
     */
    inline constexpr color_write_mask operator~(color_write_mask mask)
    {
        return static_cast<color_write_mask>(~static_cast<uint8_t>(mask) &
                                             0x0F);
    }

} // namespace mge
