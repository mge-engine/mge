// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/visitor.hpp"

namespace mge::script {
    void visitor::start(const module_details_ref&) {}

    void visitor::finish(const module_details_ref&) {}

    void visitor::start(const type_details_ref&) {}

    void visitor::finish(const type_details_ref&) {}

    void visitor::enum_value(const std::string& name, int64_t value) {}

    void visitor::field(const std::string&      name,
                        const type_details_ref& type,
                        const invoke_function&  setter,
                        const invoke_function&  getter)
    {}

    void visitor::constructor(const signature&       sig,
                              const invoke_function& new_at,
                              const invoke_function& new_shared)
    {}

    void visitor::destructor(const invoke_function& delete_ptr,
                             const invoke_function& delete_shared_ptr)
    {}

    void visitor::method(const std::string&     name,
                         bool                   is_static,
                         const std::type_index& return_type,
                         const signature&       sig,
                         const invoke_function& invoke)
    {}

} // namespace mge::script