// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/uniform_data_type.hpp"

#include <cstddef>
#include <cstdint>

namespace mge {

    /**
     * @brief std140 layout rules for uniform block members.
     *
     * Implements the std140 layout rules as specified in the OpenGL
     * specification (and compatible with Vulkan/DirectX constant buffers).
     *
     * Key rules:
     * - Scalars: base alignment = size of scalar (4 bytes for float/int)
     * - vec2: base alignment = 2N (8 bytes for float)
     * - vec3/vec4: base alignment = 4N (16 bytes for float)
     * - mat NxM (column-major): array of N column vectors, each vec of M
     *   components, each column aligned as vec4 (16 bytes for float matrices)
     * - Arrays: each element rounded up to vec4 alignment
     * - Structs: aligned to largest member alignment, rounded up to vec4
     */

    /**
     * @brief Get the std140 base alignment for a uniform data type.
     *
     * @param t the uniform data type
     * @return alignment in bytes
     */
    inline size_t std140_base_alignment(uniform_data_type t)
    {
        switch (t) {
        // Scalars: alignment = N (size of the scalar)
        case uniform_data_type::INT8:
        case uniform_data_type::UINT8:
            return 4; // std140 rounds up to 4
        case uniform_data_type::INT16:
        case uniform_data_type::UINT16:
            return 4; // std140 rounds up to 4
        case uniform_data_type::HALF:
            return 4; // std140 rounds up to 4
        case uniform_data_type::INT32:
        case uniform_data_type::UINT32:
        case uniform_data_type::FLOAT:
            return 4;
        case uniform_data_type::INT64:
        case uniform_data_type::UINT64:
        case uniform_data_type::DOUBLE:
            return 8;

        // vec2: alignment = 2N
        case uniform_data_type::FLOAT_VEC2:
            return 8;
        case uniform_data_type::INT_VEC2:
        case uniform_data_type::UINT_VEC2:
            return 8;
        case uniform_data_type::DOUBLE_VEC2:
            return 16;

        // vec3, vec4: alignment = 4N
        case uniform_data_type::FLOAT_VEC3:
        case uniform_data_type::FLOAT_VEC4:
            return 16;
        case uniform_data_type::INT_VEC3:
        case uniform_data_type::INT_VEC4:
        case uniform_data_type::UINT_VEC3:
        case uniform_data_type::UINT_VEC4:
            return 16;
        case uniform_data_type::DOUBLE_VEC3:
        case uniform_data_type::DOUBLE_VEC4:
            return 32;

        // Matrices (column-major): each column is a vec, aligned as vec4
        // alignment = alignment of column vector rounded to vec4
        case uniform_data_type::FLOAT_MAT2:
        case uniform_data_type::FLOAT_MAT3:
        case uniform_data_type::FLOAT_MAT4:
        case uniform_data_type::FLOAT_MAT2x3:
        case uniform_data_type::FLOAT_MAT2x4:
        case uniform_data_type::FLOAT_MAT3x2:
        case uniform_data_type::FLOAT_MAT3x4:
        case uniform_data_type::FLOAT_MAT4x2:
        case uniform_data_type::FLOAT_MAT4x3:
            return 16; // column vector aligned as vec4

        case uniform_data_type::DOUBLE_MAT2:
        case uniform_data_type::DOUBLE_MAT3:
        case uniform_data_type::DOUBLE_MAT4:
        case uniform_data_type::DOUBLE_MAT2x3:
        case uniform_data_type::DOUBLE_MAT2x4:
        case uniform_data_type::DOUBLE_MAT3x2:
        case uniform_data_type::DOUBLE_MAT3x4:
        case uniform_data_type::DOUBLE_MAT4x2:
        case uniform_data_type::DOUBLE_MAT4x3:
            return 32; // dvec4 alignment

        default:
            MGE_THROW(illegal_argument)
                << "Cannot compute std140 alignment for type " << t;
        }
    }

    /**
     * @brief Get the std140 size for a uniform data type (single element, not
     * array).
     *
     * For matrices, the size is columns * column_stride where each column is
     * padded to vec4 alignment.
     *
     * @param t the uniform data type
     * @return size in bytes
     */
    inline size_t std140_type_size(uniform_data_type t)
    {
        switch (t) {
        // Scalars: stored as 4 bytes minimum in std140
        case uniform_data_type::INT8:
        case uniform_data_type::UINT8:
        case uniform_data_type::INT16:
        case uniform_data_type::UINT16:
        case uniform_data_type::HALF:
        case uniform_data_type::INT32:
        case uniform_data_type::UINT32:
        case uniform_data_type::FLOAT:
            return 4;
        case uniform_data_type::INT64:
        case uniform_data_type::UINT64:
        case uniform_data_type::DOUBLE:
            return 8;

        // Vectors
        case uniform_data_type::FLOAT_VEC2:
            return 8;
        case uniform_data_type::FLOAT_VEC3:
            return 12; // 3 floats, but 16-byte aligned
        case uniform_data_type::FLOAT_VEC4:
            return 16;
        case uniform_data_type::INT_VEC2:
        case uniform_data_type::UINT_VEC2:
            return 8;
        case uniform_data_type::INT_VEC3:
        case uniform_data_type::UINT_VEC3:
            return 12;
        case uniform_data_type::INT_VEC4:
        case uniform_data_type::UINT_VEC4:
            return 16;
        case uniform_data_type::DOUBLE_VEC2:
            return 16;
        case uniform_data_type::DOUBLE_VEC3:
            return 24;
        case uniform_data_type::DOUBLE_VEC4:
            return 32;

        // Matrices (column-major): N columns, each column padded to vec4
        // stride matCxR has C columns of R rows
        // column stride = round_up(R * scalar_size, 16) for float
        //               = round_up(R * scalar_size, 32) for double
        case uniform_data_type::FLOAT_MAT2:   // 2 cols of vec2, stride=16
            return 2 * 16;
        case uniform_data_type::FLOAT_MAT3:   // 3 cols of vec3, stride=16
            return 3 * 16;
        case uniform_data_type::FLOAT_MAT4:   // 4 cols of vec4, stride=16
            return 4 * 16;
        case uniform_data_type::FLOAT_MAT2x3: // 2 cols of vec3, stride=16
            return 2 * 16;
        case uniform_data_type::FLOAT_MAT2x4: // 2 cols of vec4, stride=16
            return 2 * 16;
        case uniform_data_type::FLOAT_MAT3x2: // 3 cols of vec2, stride=16
            return 3 * 16;
        case uniform_data_type::FLOAT_MAT3x4: // 3 cols of vec4, stride=16
            return 3 * 16;
        case uniform_data_type::FLOAT_MAT4x2: // 4 cols of vec2, stride=16
            return 4 * 16;
        case uniform_data_type::FLOAT_MAT4x3: // 4 cols of vec3, stride=16
            return 4 * 16;

        case uniform_data_type::DOUBLE_MAT2:   // 2 cols of dvec2, stride=32
            return 2 * 32;
        case uniform_data_type::DOUBLE_MAT3:   // 3 cols of dvec3, stride=32
            return 3 * 32;
        case uniform_data_type::DOUBLE_MAT4:   // 4 cols of dvec4, stride=32
            return 4 * 32;
        case uniform_data_type::DOUBLE_MAT2x3: // 2 cols of dvec3, stride=32
            return 2 * 32;
        case uniform_data_type::DOUBLE_MAT2x4: // 2 cols of dvec4, stride=32
            return 2 * 32;
        case uniform_data_type::DOUBLE_MAT3x2: // 3 cols of dvec2, stride=32
            return 3 * 32;
        case uniform_data_type::DOUBLE_MAT3x4: // 3 cols of dvec4, stride=32
            return 3 * 32;
        case uniform_data_type::DOUBLE_MAT4x2: // 4 cols of dvec2, stride=32
            return 4 * 32;
        case uniform_data_type::DOUBLE_MAT4x3: // 4 cols of dvec3, stride=32
            return 4 * 32;

        default:
            MGE_THROW(illegal_argument)
                << "Cannot compute std140 size for type " << t;
        }
    }

    /**
     * @brief Get the std140 array stride for a type.
     *
     * In std140, each array element is rounded up to a multiple of vec4
     * alignment (16 bytes for float types, 32 for double types).
     *
     * @param t the uniform data type
     * @return array stride in bytes
     */
    inline size_t std140_array_stride(uniform_data_type t)
    {
        size_t base_align = std140_base_alignment(t);
        size_t type_size = std140_type_size(t);
        // Round up to vec4 alignment (base alignment for arrays is
        // max(16, base_alignment))
        size_t array_align = (base_align < 16) ? 16 : base_align;
        return ((type_size + array_align - 1) / array_align) * array_align;
    }

    /**
     * @brief Align an offset to a given alignment.
     *
     * @param offset current offset
     * @param alignment required alignment (must be power of 2)
     * @return aligned offset
     */
    inline size_t std140_align(size_t offset, size_t alignment)
    {
        return ((offset + alignment - 1) / alignment) * alignment;
    }

} // namespace mge
