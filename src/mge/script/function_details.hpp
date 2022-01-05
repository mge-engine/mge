#pragma once
#include "mge/script/dllexport.hpp"

#include <string>

namespace mge::script {

    class MGESCRIPT_EXPORT function_details
    {
    public:
        function_details(const std::string& name);
        virtual ~function_details();

    private:
        std::string m_name;
    };

} // namespace mge::script