#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/license.hpp"
#include "mge/core/version.hpp"

#include <string_view>
#include <string>

namespace mge {

    MGE_DECLARE_REF(artifact);

    /**
     * An artifact. An artificat is a component or piece of work that can
     * be individually attributed.
     */
    class MGE_CORE_EXPORT artifact
    {
    public:
        artifact();
        artifact(std::string_view name_,
                 std::string_view attribution_name_,
                 std::string_view attribution_url_,
                 const mge::version& version_);
        ~artifact() = default;

        const std::string& name() const;
        const std::string& attribution_name() const;
        const std::string& attribution_url() const;
        const mge::version& version() const;
    private:
        std::string m_name;
        std::string m_attribution_name;
        std::string m_attribution_url;
        mge::version m_version;
    };
}
