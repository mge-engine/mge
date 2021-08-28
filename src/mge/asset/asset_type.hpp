#pragma once
#include "mge/asset/dllexport.hpp"
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
    class MGEASSET_EXPORT asset_type
    {
    public:
        asset_type();
        asset_type(std::string_view type, std::string_view subtype);
        asset_type(const asset_type&) = default;
        asset_type(asset_type&&) = default;
        ~asset_type() = default;

        std::string_view type() const;
        std::string_view subtype() const;

        bool operator==(const asset_type& t) const;
        bool operator<(const asset_type& t) const;

    private:
        std::string m_type;
        std::string m_subtype;
    };

} // namespace mge