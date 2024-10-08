// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"
namespace mge::script {

    module::module() :m_data(module_data::root()) {}

    module::module(const std::string& name) :m_data(module_data::get(name)) {}

    module::~module() {}

    mge::script::module module::root() { return mge::script::module(); }

    bool module::is_root() const { return m_data->is_root(); }

    const std::string& module::name() const { return m_data->name(); }

    mge::script::module module::parent() const
    {
        if (is_root()) {
            MGE_THROW(illegal_state) << "Root module has no parent";
        }
        return module(m_data->parent());
    }

    void module::add(const function_data_ref& f) { m_data->add(f); }

    void module::add(const type_data_ref& t) { m_data->add(t); }

    const function_data& module::function(const char* name) const
    {
        return m_data->function(name);
    }

    const type_data& module::type(const char* name) const
    {
        return m_data->type(name);
    }

} // namespace mge::script