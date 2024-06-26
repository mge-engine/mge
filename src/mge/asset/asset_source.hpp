// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_access.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/component.hpp"
#include "mge/core/path.hpp"

namespace mge {
    MGE_DECLARE_REF(asset_source);

    /**
     * @brief An asset access factory.
     *
     * An asset access factory is 'mounted' below a specific path, and serves
     * all assets that are looked for beneath this path.
     */
    class MGEASSET_EXPORT asset_source : public component<asset_source>
    {
    public:
        asset_source() = default;
        virtual ~asset_source() = default;

        /**
         * @brief Configure factory.
         *
         * @param p configuration properties
         */
        virtual void configure(const mge::properties& p) = 0;

        /**
         * @brief Check whether factory is configured.
         *
         * @return @c true if configured
         */
        virtual bool is_configured() const = 0;

        /**
         * @brief Resolve path into asset object.
         *
         * @param p path, absolute (includes the mount point)
         * @return asset located at path
         */
        virtual asset_access_ref access(const mge::path& p) = 0;
        /**
         * @brief Check whether asset at given path exists.
         *
         * @param p path
         * @return true if asset exists
         */
        virtual bool asset_exists(const mge::path& p) = 0;

        /**
         * @brief Get factory details.
         *
         * @param context format context
         */
        virtual void gist(std::format_context& context) const = 0;

        /**
         * @brief Set the mount point.
         *
         * The mount point is a path prefix of all assets managed by this
         * factory.
         *
         * @param mountpoint new mount point
         */
        void set_mount_point(const mge::path& mountpoint);

        /**
         * @brief Get current mount point.
         *
         * @return mount point
         */
        const mge::path& mount_point() const noexcept { return m_mount_point; }

    protected:
        virtual void on_set_mount_point(const mge::path& mount_point);

        mge::path m_mount_point;
    };
} // namespace mge