// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/uniform_data_type.hpp"

#include <cstdint>

namespace mge {

    /**
     * @brief Base class for global uniforms.
     *
     * Global uniforms are exactly that - global. They are not associated with a
     * specific shader or program, and automatically bound into uniform blocks
     * when they appear by name.
     *
     * Global uniforms are useful for data that is shared across multiple
     * shaders and programs, such as camera parameters, lighting information, or
     * other scene-wide data. By using global uniforms, you can avoid the need
     * to manually bind the same data to multiple programs, and ensure that all
     * shaders have access to the same consistent data.
     */
    class MGEGRAPHICS_EXPORT uniform_base
    {
    public:
        uniform_base(const std::string_view& name);
        virtual ~uniform_base();

        /**
         * @brief Get the version number of the uniform.
         */
        uint64_t version() const noexcept
        {
            return m_version;
        }

    protected:
        std::string_view m_name; //!< name of the uniform
        uint64_t m_version{0};   //!< version number for tracking changes to the
                                 //!< uniform value
    };

    template <typename T> class uniform : public uniform_base
    {
        static_assert(uniform_data_type_of<T>::value !=
                          uniform_data_type::UNKNOWN,
                      "Uniform data type must be a supported type");

    public:
        uniform(const std::string_view& name)
            : uniform_base(name)
        {}

        constexpr uniform_data_type type() const noexcept
        {
            return uniform_data_type_of<T>::value;
        }

        inline T& operator=(const T& value) noexcept
        {
            if constexpr (std::is_integral_v<T> ||
                          std::is_floating_point_v<T>) {
                if (m_value != value) {
                    m_value = value;
                }
                ++m_version;
                return m_value;
            }
        }

        T m_value; //!< value of the uniform
    };

} // namespace mge