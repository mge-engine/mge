// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <functional>

namespace mge::script {

    namespace details {
        class function_base;
    }

    class MGESCRIPT_EXPORT function_details
    {
    public:
        using invocation_function = std::function<void(call_context&)>;

        function_details(const std::string&         name,
                         const invocation_function& invoke);

        function_details(const std::string& name, invocation_function&& invoke);
        virtual ~function_details() = default;

        const std::string&         name() const;
        mge::script::module        module() const;
        const invocation_function& invoke() const;

        virtual void apply(visitor& v);

    private:
        friend class details::function_base;
        friend class module_details;

        std::string                            m_name;
        std::function<void(call_context&)>     m_invoke;
        type_details_ref                       m_result_type;
        mge::small_vector<type_details_ref, 5> m_parameter_types;
        module_details_weak_ref                m_module;
    };

} // namespace mge::script