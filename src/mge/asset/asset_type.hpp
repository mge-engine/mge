// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_operators.hpp"
#include "mge/asset/dllexport.hpp"
#include <iosfwd>
#include <string>
#include <string_view>

namespace mge {

    /**
     * @brief Asset type.
     *
     * An asset type helps in identifying the loader for a specific
     * asset. Asset types are modeled after mime types, i.e. they
     * have a type and a subtype.
     */
    class MGEASSET_EXPORT asset_type : public boost::totally_ordered<asset_type>
    {
    public:
        static const asset_type UNKNOWN;

        /**
         * @brief Construct empty/unknown asset type.
         */
        asset_type();

        /**
         * @brief Construct asset type.
         *
         * @param type      asset type
         * @param subtype   asset subtype
         */
        asset_type(std::string_view type, std::string_view subtype);

        /**
         * @brief Copy constructor.
         * @param t copied type
         */
        asset_type(const asset_type& t) = default;

        /**
         * @brief Move constructor.
         * @param t moved type
         */
        asset_type(asset_type&& t) = default;

        ~asset_type() = default;

        asset_type& operator=(const asset_type& t) = default;
        asset_type& operator=(asset_type&& t) = default;

        /**
         * @brief Access asset type.
         *
         * @return type
         */
        std::string_view type() const;

        /**
         * @brief Access asset subtype.
         *
         * @return subtype
         */
        std::string_view subtype() const;

        bool operator==(const asset_type& t) const;
        bool operator<(const asset_type& t) const;

        static asset_type parse(std::string_view s);

    private:
        std::string m_type;
        std::string m_subtype;
    };

    MGEASSET_EXPORT std::ostream& operator<<(std::ostream&     os,
                                             const asset_type& t);

    namespace literals {
        MGEASSET_EXPORT asset_type operator""_at(const char* s, size_t l);
    }

} // namespace mge