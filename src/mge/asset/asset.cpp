// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/asset/asset_access_factory.hpp"
#include "mge/asset/asset_locator.hpp"
#include "mge/asset/asset_not_found.hpp"
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
            return asset_access_ref();
        }

    private:
        void configure();

        std::map<path, asset_access_factory_ref> m_mounts;
        std::map<path, std::string>              m_mount_points;
        std::map<path, properties>               m_mount_properties;
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
