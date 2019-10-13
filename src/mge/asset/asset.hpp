#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/core/path.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/asset/asset_fwd.hpp"
#include "mge/core/gist.hpp"
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
        asset(const char *path)
            :m_path(path)
        {}

        /**
         * Construct from path string.
         * 
         * @param path asset path
         */
        asset(const std::string& path)
            :m_path(path)
        {}

        /**
         * Construct from path.
         * 
         * @param path asset path
         */
        asset(const path& p)
            :m_path(p)
        {}

        /**
         * Copy constructor.
         * 
         * @param a copied asset
         */
        asset(const asset& a)
            :m_path(a.m_path)
        {}

        /**
         * Move constructor.
         * 
         * @param a moved asset
         */
        asset(asset&& a)
            :m_path(std::move(a.m_path))
        {}

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
        asset& operator=(asset&& a)
        {
            m_path = std::move(a.m_path);
            m_access = std::move(a.m_access);
            return *this;
        }
        /**
         * Get asset path.
         * 
         * @return asset path
         */
        const mge::path& path() const { return m_path; }
        
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
         * Supply gist info on asset.
         * 
         * @param os output stream
         */
        void gist(std::ostream& os) const;
    private:
        mge::path        m_path;
        mutable asset_access_ref m_access;
    };

    namespace string_literals {
        /**
         * Literal to define asset in source.
         * 
         * @return asset created from string
         */
        MGE_ASSET_EXPORT asset operator ""_asset(const char *, size_t);
    }
}