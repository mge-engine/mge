#include "mge/reflection/function_signature.hpp"
namespace mge {
    namespace reflection {
        function_signature::function_signature()
            :m_return_value_type(std::type_index(typeid(void)))
        {}
    }
}
