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

    module::module(const std::string_view identifier)
    {
        auto it =
            boost::make_split_iterator(identifier,
                                       boost::algorithm::first_finder("::"));
        auto details = module_details::root();
        while (!it.eof()) {
            if (!it->empty()) {
                std::string_view current_name(it->begin(), it->end());
                details = details->get_or_add_child(current_name);
            }
            ++it;
        }
        m_details = details;
    }

} // namespace mge::script