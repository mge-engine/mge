#include "mge/core/artifact.hpp"

namespace mge {
    artifact::artifact()
    {}

    artifact::artifact(std::string_view name_,
                       std::string_view attribution_name_,
                       std::string_view attribution_url_,
                       uint32_t year,
                       const license& license_,
                       const mge::version& version_)
        :m_name(name_)
        ,m_attribution_name(attribution_name_)
        ,m_attribution_url(attribution_url_)
        ,m_version(version_)
    {}

    const std::string&
    artifact::name() const
    {
        return m_name;
    }

    const std::string&
    artifact::attribution_name() const
    {
        return m_attribution_name;
    }

    const std::string&
    artifact::attribution_url() const
    {
        return m_attribution_url;
    }

    const mge::version&
    artifact::version() const
    {
        return m_version;
    }
}
