#include "mge/script/module.hpp"
#include "mge/script/visitor.hpp"

namespace mge::python {
    class python_context;

    class python_binder
    {
    public:
        python_binder(python_context& context);
        virtual ~python_binder();

        void bind(const mge::script::module& m);

        python_context&       context() { return m_context; }
        const python_context& context() const { return m_context; }

    private:
        python_context& m_context;
    };

} // namespace mge::python