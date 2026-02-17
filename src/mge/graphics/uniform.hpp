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

        /**
         * @brief Get the name of the uniform.
         */
        const std::string_view& name() const noexcept
        {
            return m_name;
        }

        /**
         * @brief Get the type of the uniform value.
         */
        virtual uniform_data_type type() const noexcept = 0;

        /**
         * @brief Get a pointer to the uniform value data.
         */
        virtual const void* data() const noexcept = 0;

        /**
         * @brief Get the size of the uniform value in bytes.
         */
        virtual size_t data_size() const noexcept = 0;

        /**
         * @brief Find a global uniform by name.
         *
         * @param name the uniform name
         * @return pointer to the uniform, or nullptr if not found
         */
        static uniform_base* find(const std::string_view& name);

        /**
         * @brief Get the current generation of the uniform registry.
         *
         * The generation is incremented each time a uniform is registered
         * or unregistered, allowing consumers to detect registry changes.
         *
         * @return the current registry generation number
         */
        static uint64_t registry_generation();

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

        /**
         * @brief Uniform data type.
         *
         * @return data type of this uniform
         */
        uniform_data_type type() const noexcept override
        {
            return uniform_data_type_of<T>::value;
        }

        /**
         * @brief Pointer to the uniform value.
         *
         * @return pointer to stored value
         */
        const void* data() const noexcept override
        {
            return &m_value;
        }

        /**
         * @brief Size of the uniform value in bytes.
         *
         * @return size in bytes
         */
        size_t data_size() const noexcept override
        {
            return sizeof(T);
        }

        inline uniform<T>& operator=(const T& value) noexcept
        {
            m_value = value;
            ++m_version;
            return *this;
        }

        T m_value; //!< value of the uniform
    };

} // namespace mge