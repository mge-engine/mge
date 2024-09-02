#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <variant>
#include <vector>

namespace mge::script {

    class MGESCRIPT_EXPORT dependency
    {
    public:
        using dependency_type = std::variant<std::monostate,
                                             module_data_ref,
                                             function_data_ref,
                                             type_data_ref>;

        dependency() = default;

        dependency(const module_data_ref& m)
            : m_data(m)
        {}

        dependency(const function_data_ref& f)
            : m_data(f)
        {}

        dependency(const type_data_ref& t)
            : m_data(t)
        {}

        dependency(const dependency& other) = default;
        dependency(dependency&& other) = default;

        dependency& operator=(const dependency& other) = default;
        dependency& operator=(dependency&& other) = default;

        ~dependency() = default;

        bool operator==(const dependency& other) const
        {
            return m_data == other.m_data;
        }

        bool operator!=(const dependency& other) const
        {
            return m_data != other.m_data;
        }

        bool operator<(const dependency& other) const;

    private:
        dependency_type m_data;
    };

} // namespace mge::script