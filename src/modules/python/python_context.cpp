#include "python_context.hpp"
#include "python_engine.hpp"
namespace mge::python {

    python_context::python_context(const python_engine_ref& engine)
        : m_engine(engine)
    {}

    python_context::~python_context() {}

    void python_context::eval(const std::string& code) { (void)code; }

    int python_context::main(int argc, const char** argv)
    {
        (void)argc;
        (void)argv;
        return 0;
    }

} // namespace mge::python