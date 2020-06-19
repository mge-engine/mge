// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/artifact.hpp"
#include "mge/core/log.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/version.hpp"
#include <boost/version.hpp>
#include <mutex>

MGE_DEFINE_LOG(ARTIFACT);

namespace mge {
    artifact::artifact() {}

    artifact::artifact(std::string_view    name_,
                       std::string_view    attribution_name_,
                       std::string_view    attribution_url_,
                       const mge::version &version_,
                       const mge::license &license_)
        : m_name(name_), m_attribution_name(attribution_name_),
          m_attribution_url(attribution_url_), m_version(version_),
          m_license(license_)
    {}

    const std::string &artifact::name() const { return m_name; }

    const std::string &artifact::attribution_name() const
    {
        return m_attribution_name;
    }

    const std::string &artifact::attribution_url() const
    {
        return m_attribution_url;
    }

    const mge::version &artifact::version() const { return m_version; }

    class artifact_collection
    {
    public:
        artifact_collection()  = default;
        ~artifact_collection() = default;

        mge::artifact_ref register_artifact(std::string_view name,
                                            std::string_view attribution_name,
                                            std::string_view attribution_url,
                                            const mge::version &version,
                                            const mge::license &license)
        {
            std::lock_guard<std::mutex> guard(m_lock);
            auto it = m_artifacts.find(std::string(name));
            if (it != m_artifacts.end()) {
                MGE_THROW(mge::illegal_state) << "Try to register artifact '"
                                              << name << "' a second time";
            }
            auto result = std::make_shared<artifact>(
                name, attribution_name, attribution_url, version, license);
            m_artifacts[result->name()] = result;
            return result;
        }

        void set_dependency(std::string_view artifact,
                            std::string_view dependency)
        {
            MGE_DEBUG_LOG(ARTIFACT)
                << "Register artifact dependency: " << artifact
                << " depends on " << dependency;

            {
                std::lock_guard<std::mutex> guard(m_lock);
                m_dependencies.emplace(artifact, dependency);
            }
        }

        std::map<std::string, mge::artifact_ref>      m_artifacts;
        std::set<std::pair<std::string, std::string>> m_dependencies;
        std::mutex                                    m_lock;
    };

    static singleton<artifact_collection> all_artifacts;

    mge::artifact_ref artifact::register_artifact(
        std::string_view name, std::string_view attribution_name,
        std::string_view attribution_url, const mge::version &version,
        const mge::license &license)
    {
        MGE_DEBUG_LOG(ARTIFACT)
            << "Register artifact: " << name << " " << version;
        return all_artifacts->register_artifact(
            name, attribution_name, attribution_url, version, license);
    }

    mge::artifact_ref artifact::depends_on(std::string_view dependency)
    {
        all_artifacts->set_dependency(name(), dependency);
        return shared_from_this();
    }

    mge::artifact_ref artifact::used_by(std::string_view user)
    {
        all_artifacts->set_dependency(user, name());
        return shared_from_this();
    }

} // namespace mge

MGE_ARTIFACT(boost, "", "https://www.boost.org",
             mge::version(BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000,
                          BOOST_VERSION % 100),
             mge::license(mge::license_type::BOOST));

MGE_ARTIFACT(mge, "Alexander Schroeder", "https://github.com/mge-engine/mge",
             MGE_VERSION_STRING, mge::license(mge::license_type::MIT))
    ->depends_on("sqlite3")
    ->depends_on("boost");