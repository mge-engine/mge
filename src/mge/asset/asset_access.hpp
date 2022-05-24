// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_type.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/properties.hpp"

namespace mge {
    MGE_DECLARE_REF(asset_access);

    /**
     * @brief Access to an asset.
     *
     * An asset access factory creates instances of this interface which
     * implement the actual properties of an asset.
     */
    class MGEASSET_EXPORT asset_access : public noncopyable
    {
    public:
        asset_access();
        virtual ~asset_access();
        /**
         * @brief Total size of asset data, in bytes.
         *
         * @return asset size
         */
        virtual size_t size() const = 0;
        /**
         * @brief Asset data.
         *
         * @return input stream of asset data
         */
        virtual input_stream_ref data() const = 0;
        /**
         * @brief Asset type.
         *
         * @return asset type
         */
        virtual asset_type type() const = 0;
        /**
         * @brief Get whether the asset has properties.
         *
         * @return true if the asset has properties
         */
        virtual bool has_properties() const = 0;

        /**
         * @brief Asset properties.
         *
         * @return properties
         */
        virtual properties_ref properties() const = 0;
    };
} // namespace mge
