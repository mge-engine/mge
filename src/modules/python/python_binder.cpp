#include "python_binder.hpp"
#include "mge/script/module.hpp"
#include "python_context.hpp"

namespace mge::python {

    class module_binder : public mge::script::visitor
    {
    public:
        using mge::script::visitor::start;

        module_binder(python_binder& binder)
            : m_binder(binder)
        {}

        virtual void start(const mge::script::module_details_ref& m) override
        {
            mge::script::module mod(m);
            m_binder.context().get_or_add_module(mod);
        }

        python_binder& m_binder;
    };

    python_binder::python_binder(python_context& context)
        : m_context(context)
    {}

    python_binder::~python_binder() {}

    void python_binder::bind(const mge::script::module& m) {}

} // namespace mge::python