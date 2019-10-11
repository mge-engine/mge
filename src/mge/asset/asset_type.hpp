#pragma once
#include "mge/asset/dllexport.hpp"
#include <string>
#include <iosfwd>
namespace mge {

    /**
     * An asset type can help in identifying the loader needed to 
     * load an asset. Asset type is modeled after mime type, but 
     * with limited options and complexity.
     */
    class MGE_ASSET_EXPORT asset_type 
    {
    public:
        asset_type(const char *type, const char *subtype);
        asset_type(const std::string& type, const std::string& subtype);
        asset_type(const asset_type& t);
        asset_type(asset_type&& t);
        asset_type& operator =(const asset_type& t);
        asset_type& operator =(asset_type&& t);

        const std::string& type() const { return m_type; }
        const std::string& subtype() const { return m_subtype; }

        bool operator ==(const asset_type& t) const;
        bool operator !=(const asset_type& t) const;

    private:
        std::string m_type;
        std::string m_subtype;
    }

    MGE_ASSET_EXPORT std::ostream& operator <<(std::ostream& os, const asset_type& t);
}