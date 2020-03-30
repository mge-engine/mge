// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_type_mapper.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/small_vector.hpp"
#include "mge/core/stdexceptions.hpp"
#include <boost/algorithm/string.hpp>

namespace mge {

    class asset_type_map
    {
    public:
        asset_type_map()  = default;
        ~asset_type_map() = default;

        void add_mapping(const asset_type &t, const char *suffixes)
        {
            std::vector<std::string> splitted;
            boost::split(splitted, suffixes, boost::is_any_of(","));
            for (auto &suffix : splitted) {
                boost::trim(suffix);
                auto it = m_suffix_map.find(suffix);
                if (it != m_suffix_map.end()) {
                    MGE_THROW(mge::illegal_state)
                        << "Asset type " << t << " already mapped to suffix "
                        << it->first;
                }
                m_suffix_map[suffix] = t;
            }
        }

        asset_type type(const std::string &extension)
        {
            auto it = m_suffix_map.find(extension);
            if (it == m_suffix_map.end()) {
                return asset_type::UNKNOWN;
            } else {
                return it->second;
            }
        }

    private:
        std::map<std::string, asset_type> m_suffix_map;
    };

    singleton<asset_type_map> s_asset_types;

    asset_type_mapper::asset_type_mapper(const asset_type &type,
                                         const char *      suffixes)
    {
        s_asset_types->add_mapping(type, suffixes);
    }

    asset_type_mapper::~asset_type_mapper() {}

    asset_type asset_type_mapper::type(const mge::path &path)
    {
        auto extension = path.extension().generic_string();
        if (!extension.empty() && extension[0] == '.') {
            extension = std::string(extension.begin() + 1, extension.end());
        }
        return s_asset_types->type(extension);
    }

} // namespace mge
