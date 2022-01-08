#include "mge/script/function.hpp"
#include "mge/script/function_details.hpp"

namespace mge::script {

    const std::string& function_base::name() const { return m_details->name(); }

    function_details_ref
    function_base::create_details(const std::string&                  name,
                                  void*                               fptr,
                                  const mge::script::invoke_function& function,
                                  const std::type_index          return_type,
                                  std::vector<std::type_index>&& argument_types)
    {
        return function_details::create_details(name,
                                                fptr,
                                                function,
                                                return_type,
                                                std::move(argument_types));
    }

} // namespace mge::script