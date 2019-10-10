#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/core/path.hpp"
namespace mge {

    /**
     * An untyped asset is just a pile of bytes that can be located by 
     * a path.
     */
    class MGE_ASSET_EXPORT untyped_asset
    {
    public:
        untyped_asset(const mge::path& path)
            :m_path(path)
        {}
        untyped_asset(const char *path)
            :m_path(path)
        {}
        untyped_asset(const std::string& path)
            :m_path(path)
        {}

        virtual ~untyped_asset() = default;

        /**
         * Get asset path.
         * 
         * @return asset path
         */
        const mge::path& path() const { return m_path; }
        
        size_t size() const;
    private:
        mge::path m_path;
    };
}