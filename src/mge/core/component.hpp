// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"

namespace mge {


    class MGE_CORE_EXPORT component_registry_entry_base
            : noncopyable
    {
    public:
        component_registry_entry_base() = default;
        virtual ~component_registry_entry_base() = default;
        virtual const char *name() const noexcept = 0;
    protected:
        void register_component(component_registry_entry_base* c);
    };

    class MGE_CORE_EXPORT implementation_registry_entry_base
    {
    public:
        implementation_registry_entry_base() = default;
        virtual ~implementation_registry_entry_base() = default;
        virtual const char *component_name() const noexcept = 0;
        virtual const char *name() const noexcept = 0;
        virtual const char *alias_names() const noexcept = 0;
    protected:
        void register_implementation(implementation_registry_entry_base *i);
    };

}
