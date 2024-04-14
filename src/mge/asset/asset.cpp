// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_not_found.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"

#include <map>
#include <string>

namespace mge {
    MGE_DEFINE_TRACE(ASSET);

    // Asset configuration is in the executable configuration
    // under the "asset" section.
    // The configuration is a list of repositories, each with
    // a type and a mount point.
    // Example:
    // {
    //    "asset": {
    //        "repositories": [
    //            {
    //                "type": "file",
    //                "mount_point": "/",
    //                "directory": "."
    //            },
    //            {
    //                "type": "file",
    //                "mount_point": "/assets"
    //                "directory": "assets"
    //            },
    //            {
    //                "type": "zip",
    //                "mount_point": "/zipassets",
    //                "file": "assets.zip"
    //            }
    //        ]
    //    }
    // }
    MGE_DEFINE_PARAMETER((std::vector<std::map<std::string, std::string>>),
                         asset,
                         repositories,
                         "Asset configuration");

    class mount_table
    {
    public:
        mount_table()
        {
            MGE_PARAMETER(asset, repositories).set_change_handler([this]() {
                configure();
            });
        }

        ~mount_table() {}

        asset_access_ref resolve(const mge::path& p) const
        {
            if (m_mounts.empty()) {
                return asset_access_ref();
            } else {
                auto it = m_mounts.lower_bound(p);
                if (it == m_mounts.begin()) {
                    return asset_access_ref();
                } else {
                    --it;
                    return it->second.factory->access(p);
                }
            }

            return asset_access_ref();
        }

    private:
        void configure();

        struct mount_info
        {
            asset_source_ref  factory;
            std::string       type;
            mge::path         mount_point;
            ::mge::properties properties;
        };

        std::map<mge::path, mount_info> m_mounts;
    };

    void mount_table::configure()
    {
        MGE_DEBUG_TRACE(ASSET) << "Configuring mounted assets";
        std::map<path, std::string> mount_types;
        std::map<path, properties>  mount_properties;

        const auto& entries = MGE_PARAMETER(asset, repositories).get();
        for (const auto& e : entries) {
            if (e.find("mount_point") == e.end()) {
                MGE_ERROR_TRACE(ASSET)
                    << "Repository entry without mount point";
            } else if (e.find("type") == e.end()) {
                MGE_ERROR_TRACE(ASSET) << "Repository entry without type";
            } else {
                path mount_point(e.find("mount_point")->second);
                mount_types[mount_point] = e.find("type")->second;
                properties& mp = mount_properties[mount_point];
                for (const auto [key, value] : e) {
                    if (key != "mount_point" && key != "type") {
                        mp.put(key, value);
                    }
                }
            }
        }
        std::map<mge::path, mount_info> new_mounts;
        for (const auto& [mount_point, type] : mount_types) {
            auto it = m_mounts.find(mount_point);
            if (it != m_mounts.end()) {
                if (it->second.type == type) {
                    if (it->second.properties ==
                        mount_properties[mount_point]) {
                        new_mounts[mount_point] = it->second;
                        continue;
                    }
                }
            }
            mount_info mi;
            mi.mount_point = mge::path(mount_point);
            mi.type = type;
            mi.properties = mount_properties[mount_point];
            mi.factory = component<asset_source>::create(type);
            if (!mi.factory) {
                MGE_THROW(illegal_state)
                    << "Invalid mount point type for mount point '"
                    << mount_point << "' : " << type;
            } else {
                mi.factory->configure(mi.properties);
                mi.factory->set_mount_point(mount_point);
                new_mounts[mount_point] = mi;
            }
        }
        m_mounts.swap(new_mounts);
    }

    static ::mge::singleton<mount_table> mtab;

    bool asset::exists() const { return m_access || resolve(); }

    bool asset::exists(const mge::path& p)
    {
        asset tmp(p);
        return tmp.exists();
    }

    bool asset::resolve() const
    {
        m_access = mtab->resolve(m_path);
        return m_access.operator bool();
    }

} // namespace mge
