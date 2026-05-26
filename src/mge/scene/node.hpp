// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/math/mat.hpp"
#include "mge/math/quat.hpp"
#include "mge/math/vec3.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/entity.hpp"

namespace mge {

    /**
     * @brief Position component.
     */
    struct MGESCENE_EXPORT node_position
    {
        fvec3 value{0.f, 0.f, 0.f};
    };

    /**
     * @brief Rotation component.
     */
    struct MGESCENE_EXPORT node_rotation
    {
        fquat value{1.f, 0.f, 0.f, 0.f}; // identity
    };

    /**
     * @brief Scale component.
     */
    struct MGESCENE_EXPORT node_scale
    {
        fvec3 value{1.f, 1.f, 1.f};
    };

    /**
     * @brief Cached TRS matrix component.
     */
    struct MGESCENE_EXPORT node_transform
    {
        fmat4 matrix{1.f};
    };

    /**
     * @brief Node entity-
     *
     * A scene entity that always has a position,
     * orientation and scale, and a cached transform matrix. The transform is
     * recomputed whenever one of the components changes.
     */
    class MGESCENE_EXPORT node : public entity
    {
    public:
        struct tag : public entity::tag
        {};

        node() = default;
        node(const node&) = default;
        node(node&&) = default;
        node& operator=(const node&) = default;
        node& operator=(node&&) = default;
        ~node() = default;

        explicit node(entity e);

        /**
         * Get node position.
         * @return node position
         */
        const fvec3& position() const;
        /**
         * Set node position.
         * @param pos node position
         */
        void set_position(const fvec3& pos);

        /**
         * Get node rotation.
         * @return node rotation
         */
        const fquat& rotation() const;
        /**
         * Set node rotation.
         * @param rot node rotation
         */
        void set_rotation(const fquat& rot);

        /**
         * Get node scale.
         * @return node scale
         */
        const fvec3& scale() const;
        /**
         * Set node scale.
         * @param s node scale
         */
        void set_scale(const fvec3& s);

        /**
         * Get node transform matrix.
         * @return node transform matrix
         */
        const fmat4& transform() const;

    private:
        void recompute_transform();
    };

} // namespace mge
