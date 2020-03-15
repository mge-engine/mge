// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/dllexport.hpp"
#include <boost/operators.hpp>
#include <string>
#include <string_view>
#include <iosfwd>
namespace mge {

    /**
     * An asset type can help in identifying the loader needed to 
     * load an asset. Asset type is modeled after mime type, but 
     * with limited options and complexity.
     */
    class MGE_ASSET_EXPORT asset_type 
        : boost::totally_ordered<asset_type>
    {
    public:
        static const asset_type UNKNOWN;

        asset_type();

        asset_type(const char *type, const char *subtype)
            :m_type(type)
            ,m_subtype(subtype)
        {}

        asset_type(const std::string& type, const std::string& subtype)
            :m_type(type)
            ,m_subtype(subtype)
        {}

        asset_type(const std::string_view& type, const std::string_view& subtype)
            :m_type(type.begin(), type.end())
            ,m_subtype(subtype.begin(), subtype.end())
        {}

        asset_type(const asset_type& t)
            :m_type(t.m_type)
            ,m_subtype(t.m_subtype)
        {}

        asset_type(asset_type&& t)
            :m_type(std::move(t.m_type))
            ,m_subtype(std::move(t.m_subtype))
        {}

        asset_type& operator =(const asset_type& t)
        {
            m_type    = t.m_type;
            m_subtype = t.m_subtype;
            return *this;
        }

        asset_type& operator =(asset_type&& t)
        {
            m_type    = std::move(t.m_type);
            m_subtype = std::move(t.m_subtype);
            return *this;
        }

        const std::string& type() const { return m_type; }
        const std::string& subtype() const { return m_subtype; }

        bool operator ==(const asset_type& t) const
        {
            return m_type == t.m_type && m_subtype == t.m_subtype;
        }

        bool operator <(const asset_type& t) const
        {
            return m_type < t.m_type 
                ? true
                : m_type == t.m_type 
                    ? m_subtype < t.m_subtype
                    : false;
        }
    private:
        std::string m_type;
        std::string m_subtype;
    };

    MGE_ASSET_EXPORT std::ostream& operator <<(std::ostream& os, const asset_type& t);

    namespace string_literals {
        /**
         * Literal to define asset type in source.
         *
         * @return asset created from string
         */
        MGE_ASSET_EXPORT asset_type operator ""_asset_type(const char *, size_t);
    }

}
