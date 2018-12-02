#include "mge/reflection/module.hpp"
#include "mge/reflection/type.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/file.hpp"
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

            type<mge::file>()
                .constructor<const char *>()
                .constructor<const std::string&>()
                .constructor<const std::string&, const std::string&>()
                .destructor()
                .method("path", &mge::file::path)
                .method("exists", &mge::file::exists)
                .method("is_directory", &mge::file::is_directory)
                .method("is_file", &mge::file::is_file)
                .method("is_system_file", &mge::file::is_system_file)
                .method("mkdir", &mge::file::mkdir)
                .method("rmdir", &mge::file::rmdir)
                .method("list", &mge::file::list)
                .method("open_for_input", &mge::file::open_for_input);

            type<mge::input_stream>()
                .destructor()
                .explicit_method(mge::input_stream::streamsize_type,
                                 mge::input_stream,
                                 read,
                                 ())
                .explicit_method(mge::input_stream::streamsize_type,
                                 mge::input_stream,
                                 read,
                                 (void *, mge::input_stream::streamsize_type))
                .method("position", &mge::input_stream::position)
                .method("seek", &mge::input_stream::seek)
                .method("istream", &mge::input_stream::istream)
                .method("add_read_listener", &mge::input_stream::add_read_listener)
                .method("remove_read_listener", &mge::input_stream::remove_read_listener);

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
