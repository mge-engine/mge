// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_fwd.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/gist.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/path.hpp"
#include "mge/core/properties.hpp"

namespace mge {

    /**
     * An asset. An asset has a path, potentially a type,
     * and a size and input stream. The implementation is
     * delegated to an asset access instance.
     */
    class MGEASSET_EXPORT asset
    {
    public:
        /**
         * Construct from path string.
         *
         * @param path asset path
         */
        asset(const char* path)
            : m_path(path)
        {}

        /**
         * Construct from path string.
         *
         * @param path asset path
         */
        asset(const std::string& path)
            : m_path(path)
        {}

        /**
         * Construct from path.
         *
         * @param p asset path
         */
        asset(const path& p)
            : m_path(p)
        {}

        /**
         * Copy constructor.
         *
         * @param a copied asset
         */
        asset(const asset& a) = default;

        /**
         * Move constructor.
         *
         * @param a moved asset
         */
        asset(asset&& a) = default;

        /**
         * Destructor.
         */
        ~asset() = default;

        /**
         * Assignment.
         *
         * @param a assigned asset
         * @return @c *this
         */
        asset& operator=(const asset& a) = default;

        /**
         * Move assignment.
         *
         * @param a assigned asset
         * @return @c *this
         */
        asset& operator=(asset&& a) = default;

        /**
         * Get asset path.
         *
         * @return asset path
         */
        const mge::path& path() const
        {
            return m_path;
        }

        /**
         * Get whether this asset exists.
         *
         * @return @c true if the asset exists
         */
        bool exists() const;

        /**
         * @brief Load the asset.
         *
         * @return loaded asset, depends on asset type
         */
        std::any load() const;

        /**
         * @brief Store an asset.
         *
         * @param type  asset type
         * @param asset asset to be stored
         */
        void store(const asset_type& type, const std::any& asset) const;

        /**
         * @brief Adds an asset handler.
         *
         * @param handler handler that is added.
         */
        static void add_handler(const asset_handler_ref& handler);

        /**
         * Get whether path can be found as asset.
         *
         * @param path asset path
         * @return @c true if asset exists
         */
        static bool exists(const mge::path& path);

        /**
         * Get asset size in bytes.
         *
         * @return asset size
         */
        size_t size() const;

        /**
         * Get a stream to the input data.
         *
         * @return input data
         */
        mge::input_stream_ref data() const;

        /**
         * Get a stream to write the output data.
         *
         * @return output stream
         */
        mge::output_stream_ref output_stream() const;

        /**
         * Get the asset type.
         *
         * @return asset type
         */
        asset_type type() const;

        /**
         * Supply gist info on asset.
         *
         * @param os output stream
         */
        void gist(std::ostream& os) const;

        /**
         * Check if the asset has properties.
         * @return @c true if properties are present
         */
        bool has_properties() const;

        /**
         * Get the asset's properties.
         * @return asset properties
         */
        properties_ref properties() const;

        /**
         * Locates an asset by name. E.g. locate a font family by
         * name.
         * @param name name of asset to look up
         * @param type asset type
         * @return asset that was located
         * @throw @c asset_not_found if asset could not be looked up
         */
        static asset locate(std::string_view name, const asset_type& type);

        /**
         * @brief mount a specific asset factory at a mount point.
         *
         * @param mount_point mount point
         * @param type        asset access type
         * @param options     mount options
         */
        static void mount(const mge::path&         mount_point,
                          const std::string&       type,
                          const ::mge::properties& options);

        /**
         * @brief mount a specific asset factory at a mount point.
         *
         * @param mount_point mount point
         * @param type        asset access type
         * @param mode        access mode
         * @param options     mount options
         */
        static void mount(const mge::path&          mount_point,
                          const std::string&        type,
                          asset_source::access_mode mode,
                          const ::mge::properties&  options);

        /**
         * @brief Unmount a mount point.
         *
         * @param mount_point mount point
         */
        static void umount(const mge::path& mount_point);

        /**
         * @brief  Unmount a mount point.
         *
         * @param mount_point
         */
        static inline void unmount(const mge::path& mount_point)
        {
            umount(mount_point);
        }

    private:
        bool       resolve() const;
        asset_type magic() const;

        mge::path                         m_path;
        mutable asset_access_ref          m_access;
        mutable std::optional<asset_type> m_type;
    };

    namespace string_literals {
        /**
         * Literal to define asset in source.
         *
         * @return asset created from string
         */
        MGEASSET_EXPORT asset operator""_asset(const char*, size_t);
    } // namespace string_literals
} // namespace mge
