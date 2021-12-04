#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {
    class MGESCRIPT_EXPORT variable_details
        : public std::enable_shared_from_this<variable_details>
    {
    public:
        variable_details(const std::string& name);
        virtual ~variable_details() = default;

        const std::string& name() const;

        virtual void apply(visitor& v);

    private:
        std::string m_name;
    };
} // namespace mge::script