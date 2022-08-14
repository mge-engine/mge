// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    /**
     * @brief Visit a module.
     */
    class MGESCRIPT_EXPORT visitor
    {
    protected:
        visitor() = default;

    public:
        virtual ~visitor() = default;

        virtual void start(const module_details_ref& m);
        virtual void finish(const module_details_ref& m);
        virtual void start(const type_details_ref& t);
        virtual void finish(const type_details_ref& t);
        virtual void enum_value(const std::string& name, int64_t value);
        virtual void field(const std::string&      name,
                           const type_details_ref& type,
                           const invoke_function&  setter,
                           const invoke_function&  getter);
        virtual void constructor(const signature&       sig,
                                 const invoke_function& new_at,
                                 const invoke_function& make_shared);
        virtual void destructor(const invoke_function& delete_ptr,
                                const invoke_function& delete_shared);
    };

} // namespace mge::script