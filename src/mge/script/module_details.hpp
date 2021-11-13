#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace mge::script {

    class MGESCRIPT_EXPORT module_details
        : public std::enable_shared_from_this<module_details>
    {
    public:
        module_details(const module_details_ref& parent,
                       const std::string_view    name);
        ~module_details();

        std::string_view name() const;

        bool is_root() const;

        module_details_ref parent() const;

    private:
        module_details_weak_ref                        m_parent;
        std::map<std::string_view, module_details_ref> m_children;
        std::string                                    m_name;
    };

} // namespace mge::script