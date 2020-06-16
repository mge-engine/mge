// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/license.hpp"
#include "mge/core/types.hpp"
#include "mge/core/version.hpp"

#include <string>
#include <string_view>

namespace mge {

    MGE_DECLARE_REF(artifact);

    /**
     * An artifact. An artifact is a component or piece of work that can
     * be individually attributed.
     */
    class MGE_CORE_EXPORT artifact
    {
    public:
        /**
         * Unknown, uninitialized artifact.
         */
        artifact();
        /**
         * Constructor.
         * @param name_                 unique artificat name
         * @param attribution_name_     attribution name
         * @param attribution_url_      attribution url
         * @param version_              artifact version
         */
        artifact(std::string_view name_, std::string_view attribution_name_,
                 std::string_view    attribution_url_,
                 const mge::version &version_, const mge::license &license_);
        ~artifact() = default;

        /**
         * Artifact name.
         * @return name of artifact
         */
        const std::string &name() const;

        /**
         * Attribution name.
         * @return attribution name
         */
        const std::string &attribution_name() const;
        /**
         * Attribution URL.
         * @return attribution url
         */
        const std::string &attribution_url() const;

        /**
         * Artifact version.
         * @return artifact version
         */
        const mge::version &version() const;

        /**
         * Return artifact license.
         * @return artifact license
         */
        const mge::license &license() const;

        /**
         * @brief
         *
         * @param name_
         * @param attribution_name
         * @param attribution_url
         * @param version
         * @param license
         * @return registered artifact;
         */
        static mge::artifact_ref register_artifact(
            std::string_view name, std::string_view attribution_name,
            std::string_view attribution_url, const mge::version &version,
            const mge::license &license);

    private:
        std::string  m_name;
        std::string  m_attribution_name;
        std::string  m_attribution_url;
        mge::version m_version;
        mge::license m_license;
    };

} // namespace mge

/**
 * @brief Register a new artifact.
 * @param name artificat name
 * @param attribution_name name for attribution
 * @param attribution_url URL for attribution
 */
#define MGE_ARTIFACT(name, attribution_name, attribution_url, version,         \
                     license)                                                  \
    static auto mge_##name##_artifact = ::mge::artifact::register_artifact(    \
        #name, attribution_name, attribution_url, version, license)
