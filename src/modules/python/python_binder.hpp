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

        void start(const mge::script::module_details& m) override;
        void finish(const mge::script::module_details& m) override;
        void start(const mge::script::type_details& m) override;
        void finish(const mge::script::type_details& m) override;

    private:
        void resolve_global_module();
        void resolve_mge_module();

        python_context& m_context;
    };

} // namespace mge::python