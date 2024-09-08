#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/type_data.hpp"

#include "pyobject_ref.hpp"
#include "python_fwd.hpp"

namespace mge::python {

    class python_type
    {
    public:
        python_type(const mge::script::type_data_ref& type);
        virtual ~python_type() = default;

        void materialize();

    private:
        void init_enum();
        void materialize_enum();

        std::string m_name_in_module; // name as it appaers in the module
        std::string m_name;           // name for type spec, qualified fully
        PyType_Spec m_spec;
        std::map<std::string, pyobject_ref> m_attributes;
        mge::script::type_data_ref          m_type;
    };
} // namespace mge::python
