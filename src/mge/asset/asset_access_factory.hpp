// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_access.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/component.hpp"
#include "mge/core/path.hpp"

namespace mge {
    MGE_DECLARE_REF(asset_access_factory);

    /**
     * @brief An asset access factory.
     *
     * An asset access factory is 'mounted' below a specific path, and serves
     * all assets that are looked for beneath this path.
     */
    class MGEASSET_EXPORT asset_access_factory
        : public component<asset_access_factory>
    {
    public:
        asset_access_factory() = default;
        virtual ~asset_access_factory() = default;

        /**
         * @brief Resolve path into asset object.
         *
         * @param p path
         * @return asset located at path
         */
        virtual asset_access_ref create_asset_access(const mge::path& p) = 0;
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
         * @param os output stream
         */
        virtual void gist(std::ostream& os) const = 0;

        /**
         * @brief Set the mount point.
         *
         * The mount point is a path prefix of all assets managed by this
         * factory.
         *
         * @param mountpoint new mount point
         */
        void set_mountpoint(const mge::path& mountpoint);

        /**
         * @brief Get current mount point.
         *
         * @return mount point
         */
        const mge::path& mountpoint() const noexcept { return m_mountpoint; }

    private:
        mge::path m_mountpoint;
    };
} // namespace mge