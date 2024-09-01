#include "python_context.hpp"
#include "gil_lock.hpp"
#include "pyobject_ref.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"

#include "mge/core/trace.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context(const python_engine_ref& engine)
        : m_engine(engine)
    {}

    python_context::~python_context() {}

    void python_context::eval(const std::string& code)
    {
        try {
            std::lock_guard<gil_lock> lock(gil_lock::instance());
            pyobject_ref main_module(PyImport_AddModule("__main__"));
            pyobject_ref main_dict(PyModule_GetDict(main_module.get()));
            error::check_error();
        } catch (...) {
            throw;
        }
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = Py_BytesMain(argc, const_cast<char**>(argv));
        MGE_DEBUG_TRACE(PYTHON) << "Python main returned " << rc;
        return rc;
    }

} // namespace mge::python