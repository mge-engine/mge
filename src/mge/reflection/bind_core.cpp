#include "mge/reflection/module.hpp"
#include "mge/reflection/type.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/log_severity.hpp"
namespace mge {
    namespace reflection {
        void bind_core()
        {
            type<mge::configuration>()
                .constructor()
                .constructor<const std::string&>()
                .constructor<const configuration&>()
                .destructor()
                .explicit_method(bool,
                                 mge::configuration,
                                 contains_key,
                                 (const char *) const)
                .explicit_method(bool,
                                 mge::configuration,
                                 contains_key,
                                 (const std::string&) const)
                .method("keys", &mge::configuration::keys)
                .explicit_method(std::string,
                                 mge::configuration,
                                 value,
                                 (const char *) const)
                .explicit_method(std::string,
                                 mge::configuration,
                                 value,
                                 (const char *, const std::string&) const)
                .explicit_method(std::vector<std::string>,
                                 mge::configuration,
                                 list_value,
                                 (const char *) const)
                .method("empty", &mge::configuration::empty)
                .method("store", &mge::configuration::store);

            type<mge::log_severity>()
                .enum_value("NONE", log_severity::NONE)
                .enum_value("ERROR_SEVERITY", log_severity::ERROR_SEVERITY)
                .enum_value("WARNING_SEVERITY", log_severity::WARNING_SEVERITY)
                .enum_value("INFO_SEVERITY", log_severity::INFO_SEVERITY)
                .enum_value("DEBUG_SEVERITY", log_severity::DEBUG_SEVERITY)
                .enum_value("ALL", log_severity::ALL);

        }
    }
}
