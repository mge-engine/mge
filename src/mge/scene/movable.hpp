#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/math/mat4.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/quat.hpp"

#include <type_traits>

namespace mge {


    /**
     * An object that can be transformed.
     */
    class MGE_SCENE_EXPORT movable
    {
    public:
        movable()
        {
            set_position(0.0f);
            set_scale(1.0f);
            set_orientation(0.0f, 0.0f, 0.0f, 1.0f);
        }

        template <typename... ArgTypes>
        void set_position(ArgTypes&&...args) noexcept(std::is_nothrow_constructible<fvec3, ArgTypes...>::value)
        {
            m_position = fvec3(std::forward<ArgTypes>(args)...);
        }

        const fvec3& position() const noexcept
        {
            return m_position;
        }

        template <typename... ArgTypes>
        void set_scale(ArgTypes&&...args) noexcept(std::is_nothrow_constructible<fvec3, ArgTypes...>::value)
        {
            m_scale = fvec3(std::forward<ArgTypes>(args)...);
        }

        const fvec3& scale() const noexcept
        {
            return m_scale;
        }

        template <typename... ArgTypes>
        void set_orientation(ArgTypes&&...args) noexcept(std::is_nothrow_constructible<fquat, ArgTypes...>::value)
        {
            m_orientation = fquat(std::forward<ArgTypes>(args)...);
        }

        const fquat& orientation() const noexcept
        {
            return m_orientation;
        }

    private:
        fvec3 m_position;
        fvec3 m_scale;
        fquat m_orientation;
    };

}
