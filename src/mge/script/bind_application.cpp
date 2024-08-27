#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    class application_script_binder : public script_binder
    {
    public:
        application_script_binder() = default;

        void bind() override { mge::script::module mge("mge"); }
    };

    MGE_REGISTER_IMPLEMENTATION(application_script_binder,
                                mge::script_binder,
                                mgeapplication,
                                application);

} // namespace mge::script