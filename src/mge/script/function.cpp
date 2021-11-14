#include "mge/script/function.hpp"
#include "mge/script/function_details.hpp"
#include <tuple>

namespace mge::script {
    namespace details {
        void function_base::create_details(const std::string& name, void* fptr)
        {
            m_details = std::make_shared<function_details>(name);
        }

        const std::string& function_base::name() const
        {
            return m_details->name();
        }
    } // namespace details
} // namespace mge::script