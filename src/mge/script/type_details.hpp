#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    class MGESCRIPT_EXPORT type_details
    {
    public:
        const std::string& name() const;
        module_details*    module() const;

    private:
        std::string     m_name;
        module_details* m_module;
    };
} // namespace mge::script