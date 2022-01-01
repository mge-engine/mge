#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    class MGESCRIPT_EXPORT type_base
    {
    public:
        const std::string&     name() const;
        mge::script::module    module() const;
        const std::type_index& type_index() const;
        type_details*          details() const;

    protected:
        void init_details(const std::type_index& index);

    private:
        type_details* m_details;
    };

    template <typename T, typename = void> class type : public type_base
    {
    public:
        inline type()
        {
            static_assert(false, "Default expansion of type detected");
        }
    };

    template <> class type<void, void> : public type_base
    {
    public:
        inline type() { init_details(std::type_index(typeid(void))); }

        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;
    };

} // namespace mge::script