#include "mge/script/dependency.hpp"
#include "mge/core/overloaded.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    bool dependency::operator<(const dependency& other) const
    {
        if (m_data.index() < other.m_data.index()) {
            return true;
        } else if (m_data.index() > other.m_data.index()) {
            return false;
        } else {
            return m_data < other.m_data;
        }
    }

    std::ostream& operator<<(std::ostream& os, const dependency& d)
    {
        std::visit(
            overloaded{
                [&os](const std::monostate&) { os << "???"; },
                [&os](const module_data_ref& m) {
                    os << "module: " << m->name();
                },
                [&os](const function_data_ref& f) {
                    os << "function: " << f->name();
                },
                [&os](const type_data_ref& t) { os << "type: " << t->name(); },
            },
            d.data());
        return os;
    }

    dependency_set& dependency::builtin_dependencies()
    {
        static dependency_set s_builtin_dependencies{
            type<void>().data(),
            type<bool>().data(),
            type<uint8_t>().data(),
            type<int8_t>().data(),
            type<signed char>().data(),
            type<uint16_t>().data(),
            type<int16_t>().data(),
            type<uint32_t>().data(),
            type<int32_t>().data(),
            type<uint64_t>().data(),
            type<int64_t>().data(),
            type<float>().data(),
            type<double>().data(),
            type<long double>().data(),
            type<std::string>().data(),
            type<std::string_view>().data(),
            type<const char*>().data()};
        return s_builtin_dependencies;
    }

} // namespace mge::script