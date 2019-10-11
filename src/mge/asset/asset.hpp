#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/core/path.hpp"
#include "mge/core/input_stream.hpp"
namespace mge {

    class MGE_ASSET_EXPORT asset 
    {
    public:
        asset(const char *path)
            :m_path(path)
        {}

        asset(const std::string& path)
            :m_path(path)
        {}

        asset(const path& p)
            :m_path(p)
        {}

        asset(const asset& a)
            :m_path(a.path())
        {}

        virtual ~asset() = default;

        const mge::path& path() const { return m_path; }
        
        size_t size() const;

        mge::input_stream_ref data() const;
    private:
        mge::path m_path;
    };

    MGE_ASSET_EXPORT asset operator ""_asset(const char *, size_t);
}