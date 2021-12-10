#pragma once
#include "python.hpp"
#include <string>
namespace mge::python {

    class python_type_object
    {
    public:
        python_type_object(const std::string& name, size_t size);
        ~python_type_object() = default;

    private:
        std::string m_name;
    };

} // namespace mge::python