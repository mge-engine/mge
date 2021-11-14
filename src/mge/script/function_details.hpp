#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
namespace mge::script {

    class MGESCRIPT_EXPORT function_details
    {
    public:
        function_details(const std::string& name);
        virtual ~function_details() = default;

        const std::string&  name() const;
        mge::script::module module() const;

        virtual void apply(visitor& v);

    private:
        std::string                            m_name;
        type_details_ref                       m_result_type;
        mge::small_vector<type_details_ref, 5> m_parameter_types;
        module_details_weak_ref                m_module;
    };

} // namespace mge::script