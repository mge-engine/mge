// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/statistics.hpp"
#include "mge/core/singleton.hpp"
#include <cstdlib>

using namespace std::string_view_literals;
namespace mge {

    statistics::description::description(std::string_view name,
                                         std::string_view comment)
        : m_name(name), m_comment(comment)
    {}

    statistics::description::~description() {}

    class root_statistics : public statistics
    {
    public:
        root_statistics()          = default;
        virtual ~root_statistics() = default;
        virtual const statistics::description &describe() const
        {
            static statistics::description desc(
                ""sv, "Root of statistics object tree"sv);
            return desc;
        }
    };

    static root_statistics s_root;

    statistics::statistics() : m_name(""sv), m_owned(true) {}

    statistics::~statistics()
    {
        if (m_owned && this != &s_root) {
            // TODO: crash
            ::abort();
        }
    }

    statistics &statistics::root() { return s_root; }

    void statistics::release() { m_owned = false; }

    void statistics::add_child(statistics *s) { m_children.emplace_back(s); }

    const statistics::description &statistics::describe() const
    {
        static statistics::description desc(""sv, ""sv);
        return desc;
    }

} // namespace mge