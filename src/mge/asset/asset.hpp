// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_fwd.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/gist.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/path.hpp"
#include "mge/core/properties.hpp"

namespace mge {

    /**
     * An asset. An asset has a path, potentially a type,
     * and a size and input stream.
     */
    class MGE_ASSET_EXPORT asset
    {
    public:
        /**
         * Construct from path string.
         *
         * @param path asset path
         */
        asset(const char *path) : m_path(path) {}

        /**
         * Construct from path string.
         *
         * @param path asset path
         */
        asset(const std::string &path) : m_path(path) {}

        /**
         * Construct from path.
         *
         * @param path asset path
         */
        asset(const path &p) : m_path(p) {}

        /**
         * Copy constructor.
         *
         * @param a copied asset
         */
        asset(const asset &a) : m_path(a.m_path) {}

        /**
         * Move constructor.
         *
         * @param a moved asset
         */
        asset(asset &&a) : m_path(std::move(a.m_path)) {}

        /**
         * Destructor.
         */
        ~asset() = default;

        /**
         * Move ssignment.
         *
         * @param a assigned asset
         * @return @c *this
         */
        asset &operator=(asset &&a)
        {
            m_path   = std::move(a.m_path);
            m_access = std::move(a.m_access);
            return *this;
        }
        /**
         * Get asset path.
         *
         * @return asset path
         */
        const mge::path &path() const { return m_path; }

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
        void gist(std::ostream &os) const;

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
         * @throws @c asset_not_found if asset could not be looked up
         */
        static asset locate(std::string_view name, const asset_type &type);

    private:
        mge::path                m_path;
        mutable asset_access_ref m_access;
    };

    namespace string_literals {
        /**
         * Literal to define asset in source.
         *
         * @return asset created from string
         */
        MGE_ASSET_EXPORT asset operator""_asset(const char *, size_t);
    } // namespace string_literals
} // namespace mge
