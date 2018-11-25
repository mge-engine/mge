#include "mge/reflection/module.hpp"
#include "mge/reflection/type.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/log_severity.hpp"
namespace mge {
    namespace reflection {
        void bind_core()
        {
            type<mge::configuration> t_configuration;

#if 0

            type::get<mge::log_severity>()
                .enum_value("NONE", log_severity::NONE)
                .enum_value("ERROR_SEVERITY", log_severity::ERROR_SEVERITY)
                .enum_value("WARNING_SEVERITY", log_severity::WARNING_SEVERITY)
                .enum_value("INFO_SEVERITY", log_severity::INFO_SEVERITY)
                .enum_value("DEBUG_SEVERITY", log_severity::DEBUG_SEVERITY)
                .enum_value("ALL", log_severity::ALL);
#endif
        }
    }
}
