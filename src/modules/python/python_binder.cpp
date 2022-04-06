#include "python_binder.hpp"
#include "mge/script/module.hpp"
#include "python_context.hpp"

namespace mge::python {
    python_binder::python_binder(python_context& context)
        : m_context(context)
    {
        resolve_global_module();
        resolve_mge_module();
    }

    python_binder::~python_binder() {}

    void python_binder::bind(const mge::script::module& m) { m.apply(*this); }

    void python_binder::start(const mge::script::module_details& m) {}
    void python_binder::finish(const mge::script::module_details& m) {}
    void python_binder::start(const mge::script::type_details& m) {}
    void python_binder::finish(const mge::script::type_details& m) {}

    void python_binder::resolve_global_module()
    {
        auto root_module = mge::script::module();
        m_context.init_module(root_module);
    }

    void python_binder::resolve_mge_module() {}

} // namespace mge::python