// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type.hpp"
#include "mge/script/visitor.hpp"

#include <vector>
namespace mge::script {

    module::module()
        : m_details(module_details::root())
    {}

    module::module(const std::string& identifier)
    {
        auto it =
            boost::make_split_iterator(identifier,
                                       boost::algorithm::first_finder("::"));
        auto details = module_details::root();
        while (!it.eof()) {
            if (!it->empty()) {
                std::string current_name(it->begin(), it->end());
                details = details->get_or_add_child(current_name);
            }
            ++it;
        }
        m_details = details;
    }

    module::module(const module_details_ref& details)
        : m_details(details)
    {}

    void module::add_child(module&& m) { m_details->add_child(m.m_details); }

    bool module::is_root() const { return m_details->is_root(); }

    mge::script::module module::parent() const
    {
        if (is_root()) {
            MGE_THROW(mge::illegal_state) << "Root module has no parent";
        }
        return module(m_details->parent());
    }

    void module::add_type(const type_details_ref& t) { m_details->add_type(t); }

    const std::string& module::name() const { return m_details->name(); }

    std::string module::full_name() const
    {
        if (is_root()) {
            return "";
        } else {
            return parent().full_name() + "::" + name();
        }
    }

} // namespace mge::script