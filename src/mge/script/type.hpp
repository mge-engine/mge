#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    class type_base
    {
    public:
    private:
        type_details* m_details;
    };

    template <typename T, typename = void> class type : public type_base
    {
    public:
        inline type()
        {
            static_assert(false, "Default expansion of type detected");
        }
    };

} // namespace mge::script