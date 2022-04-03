#include "mge/script/module.hpp"
#include "mge/script/visitor.hpp"

namespace mge::python {
    class python_context;

    class python_binder : public mge::script::visitor
    {
    public:
        python_binder(python_context& context);
        virtual ~python_binder();
        void bind(const mge::script::module& m);

    private:
        python_context& m_context;
    };

} // namespace mge::python