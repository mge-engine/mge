// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"

#include "gil_lock.hpp"
#include "mge/core/component.hpp"
#include "pyobject_ref.hpp"
#include "python_context.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"
#include "python_function.hpp"
#include "python_module.hpp"
#include "python_type.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

static const char* prelude = R"(
def init():
    import mge
    import __mge__
    class component:
        
        @staticmethod
        def register(interface, name, cls):
            #__mge__.register_component(interface, name, cls)
            pass

        @staticmethod
        def create(interface, implementation):
            #__mge__.create_component(sname)
            pass

    mge.component = component
    
init()
init = None
)";

namespace mge::python {

    python_context*              python_context::s_global_context;
    thread_local python_context* python_context::s_thread_context;

    python_context::python_context(const python_engine_ref& engine)
        : m_engine(engine)
    {
        {
            gil_lock guard;
            if (s_global_context == nullptr) {

                s_global_context = this;
            }
        }
        s_thread_context = this;
    }

    python_context::~python_context()
    {
        {
            gil_lock guard;
            if (s_global_context == this) {
                s_global_context = nullptr;
            }
        }
        s_thread_context = nullptr;
    }

    void python_context::eval(const std::string& code)
    {
        if (!m_engine->interpreter_initialized()) {
            m_engine->initialize_interpreter();
        }
        gil_lock     guard;
        PyObject*    main_module = PyImport_AddModule("__main__");
        PyObject*    global_dict = PyModule_GetDict(main_module);
        pyobject_ref local_dict(PyDict_New());
        pyobject_ref result(PyRun_String(code.c_str(),
                                         Py_file_input,
                                         global_dict,
                                         local_dict.get()));
        if (!result) {
            error::check_error();
        }
    }

    void interpreter_lost()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Py_AtExit handler, intepreter lost";
        python_engine::interpreter_lost();
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = 0;
        {
            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();
            Py_AtExit(interpreter_lost);
            rc = Py_BytesMain(argc, const_cast<char**>(argv));
            // no release as interpreter thread state already lost
        }
        MGE_DEBUG_TRACE(PYTHON) << "Python main returned " << rc;
        return rc;
    }

    bool python_context::is_builtin(const mge::script::type_data_ref& t) const
    {
        return mge::script::dependency::builtin_dependencies().find(t) !=
               mge::script::dependency::builtin_dependencies().end();
    }

    const python_module_ref&
    python_context::module(const std::string& name) const
    {
        auto it = m_modules.find(name);
        if (it == m_modules.end()) {
            MGE_THROW(python::error) << "Module " << name << " not found";
        }
        return it->second;
    }

    const python_type_ref&
    python_context::type(const mge::script::type_data_ref& t) const
    {
        auto it = m_types.find(t);
        if (it == m_types.end()) {
            MGE_THROW(python::error) << "Type " << t->name() << " not found";
        }
        return it->second;
    }

    void python_context::bind()
    {
        mge::script::module root = mge::script::module::root();

        bind_helper_module();
        for (const auto& m : root.data()->modules()) {
            bind_module(m);
        }
        for (const auto& [id, t] : mge::script::type_data::all()) {
            if (is_builtin(t)) {
                MGE_DEBUG_TRACE(PYTHON)
                    << "Type " << t->name() << " is builtin";
                continue;
            }
            if (t->is_class() &&
                !t->class_specific().interface_type.expired()) {
                MGE_DEBUG_TRACE(PYTHON)
                    << "Type " << t->name() << " is proxy type";
                continue;
            }
            python_type_ref pt = std::make_shared<python_type>(*this, t);
            m_types[t] = pt;
        }
        for (const auto& m : root.data()->modules()) {
            bind_module_functions(m);
        }
        for (auto& [t, pt] : m_types) {
            MGE_DEBUG_TRACE(PYTHON) << "Defining type " << t->name();
            pt->define_in_interpreter();
        }
        evaluate_prelude();
    }

    void python_context::bind_module_functions(
        const mge::script::module_data_ref& data)
    {
        for (const auto& f : data->functions()) {
            MGE_DEBUG_TRACE(PYTHON) << "Binding function " << f->name();
            python_function_ref pf =
                std::make_shared<python_function>(*this, f);
            m_functions.push_back(pf);
        }
        for (const auto& m : data->modules()) {
            bind_module_functions(m);
        }
    }

    void python_context::bind_module(const mge::script::module_data_ref& data)
    {
        auto mod = std::make_shared<python_module>(*this, data);
        m_modules[data->full_name()] = mod;
        m_all_modules.push_back(mod);
        for (const auto& m : data->modules()) {
            bind_module(m);
        }
    }

    void python_context::bind_helper_module()
    {
        auto mod = std::make_shared<python_module>(*this, "__mge__");
        m_modules["__mge__"] = mod;
        m_all_modules.push_back(mod);
        create_function_helper_type(mod);

        struct register_component_closure : function_closure
        {
            register_component_closure(python_context& context)
                : m_context(context)
            {}

            PyObject* execute(PyObject* self, PyObject* args)
            {
                return m_context.register_component(self, args);
            }

            python_context& m_context;
        };

        struct create_component_closure : function_closure
        {
            create_component_closure(python_context& context)
                : m_context(context)
            {}

            PyObject* execute(PyObject* self, PyObject* args)
            {
                return m_context.create_component(self, args);
            }

            python_context& m_context;
        };

        m_register_component =
            std::make_shared<register_component_closure>(*this);
        m_create_component = std::make_shared<create_component_closure>(*this);

        m_methods.clear();
        m_methods.push_back({"register_component",
                             m_register_component->function(),
                             METH_VARARGS,
                             "Register a component implementation"});
        m_methods.push_back({"create_component",
                             m_create_component->function(),
                             METH_VARARGS,
                             "Create a component implementation"});
        m_methods.push_back({nullptr, nullptr, 0, nullptr});

        PyObject* module = mod->pymodule().get();
        PyModule_AddFunctions(module, m_methods.data());
        python::error::check_error();
    }

    PyObject* python_context::register_component(PyObject* self, PyObject* args)
    {
        return Py_None;
    }

    PyObject* python_context::create_component(PyObject* self, PyObject* args)
    {
        return Py_None;
    }

    void
    python_context::create_function_helper_type(const python_module_ref& mod)
    {
        python_function::register_function_type(mod);
    }

    void python_context::on_interpreter_loss()
    {
        for (auto& m : m_all_modules) {
            m->on_interpreter_loss();
        }
        for (auto& t : m_types) {
            t.second->on_interpreter_loss();
        }
        for (auto& f : m_functions) {
            f->on_interpreter_loss();
        }
    }

    void python_context::on_interpreter_restore()
    {
        for (auto& m : m_all_modules) {
            m->on_interpreter_restore();
        }
        for (auto& t : m_types) {
            t.second->on_interpreter_restore();
        }
        for (auto& f : m_functions) {
            f->on_interpreter_restore();
        }
    }

    void python_context::evaluate_prelude()
    {
        eval(prelude);
    }

    python_type_ref
    python_context::find_component_type(const std::vector<PyObject*>& types)
    {
        if (types.empty()) {
            return nullptr;
        }

        // Search through all registered types
        for (const auto& [type_data, python_type] : m_types) {
            // Skip non-component types
            if (!type_data->is_component()) {
                continue;
            }
            // For each base type
            for (PyObject* base_type : types) {
                // Check if this type's Python type object matches the base type
                if (python_type->type_object().get() == base_type) {
                    return python_type;
                }
            }
        }

        return nullptr;
    }

    void python_context::implementations(
        std::string_view                             component_name,
        const std::function<void(std::string_view)>& callback)
    {}

    std::shared_ptr<mge::component_base>
    python_context::create(std::string_view component_name,
                           std::string_view implementation_name)
    {
        std::shared_ptr<mge::component_base> result;
        return result;
    }

    class python_component_registry : public mge::component_registry
    {
    public:
        void implementations(
            std::string_view                             component_name,
            const std::function<void(std::string_view)>& callback) override
        {
            python_context* ctx = python_context::s_thread_context;
            if (!ctx) {
                ctx = python_context::s_global_context;
            }
            if (ctx) {
                ctx->implementations(component_name, callback);
            }
        }

        std::shared_ptr<mge::component_base>
        create(std::string_view component_name,
               std::string_view implementation_name) override
        {
            python_context* ctx = python_context::s_thread_context;
            if (!ctx) {
                ctx = python_context::s_global_context;
            }
            if (ctx) {
                return ctx->create(component_name, implementation_name);
            } else {
                return std::shared_ptr<mge::component_base>();
            }
        }
    };

    MGE_REGISTER_IMPLEMENTATION(python_component_registry,
                                mge::component_registry);
} // namespace mge::python