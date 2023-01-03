// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {

    class type_dictionary
    {
    public:
        type_dictionary();
        ~type_dictionary() = default;

        type_details_ref get_first_match(const std::type_index& ti)
        {
            auto it = m_types.find(ti);
            if (it == m_types.end()) {
                MGE_THROW(no_such_element)
                    << "Type details not found for:" << ti.name();
            }
            return it->second;
        }
        /*
                type_details_ref get_nothrow(const std::type_index& ti)
                        {
                    auto it = m_types.find(ti);
                    if (it == m_types.end()) {
                        return nullptr;
                    }
                    return it->second;
                }
                */

        type_details_ref add(const std::type_index& ti,
                             const std::string&     name,
                             const traits&          tr,
                             size_t                 size)
        {
            if (tr.is_enum) {
                type_details_ref new_type =
                    std::make_shared<enum_type_details>(name, ti, tr, size);
                m_types.emplace(new_type->type_index(), new_type);
                return new_type;
            }
            if (tr.is_class) {
                auto new_type =
                    std::make_shared<class_type_details>(name, ti, tr, size);
                m_types.emplace(new_type->type_index(), new_type);
                return new_type;
            }
            return type_details_ref();
        }

    private:
        template <typename T>
        void add_pod_type_details(const char* used_name = nullptr)
        {
            if constexpr (std::is_void_v<T>) {
                auto new_type =
                    std::make_shared<type_details>(mge::type_name<T>(),
                                                   std::type_index(typeid(T)),
                                                   traits_of<T>(),
                                                   0,
                                                   used_name);
                m_types.emplace(new_type->type_index(), new_type);
                module_details::get("")->add_type(new_type);
            } else {
                auto new_type =
                    std::make_shared<type_details>(mge::type_name<T>(),
                                                   std::type_index(typeid(T)),
                                                   traits_of<T>(),
                                                   sizeof(T),
                                                   used_name);
                m_types.emplace(new_type->type_index(), new_type);
                module_details::get("")->add_type(new_type);
            }
        }

        template <typename T>
        void add_type_details(const char* module, const char* used_name)
        {
            auto new_type =
                std::make_shared<type_details>(mge::type_name<T>(),
                                               std::type_index(typeid(T)),
                                               traits_of<T>(),
                                               sizeof(T),
                                               used_name);
            m_types.emplace(new_type->type_index(), new_type);
            module_details::get(module)->add_type(new_type);
        }

        std::multimap<std::type_index, type_details_ref> m_types;
    };

    type_dictionary::type_dictionary()
    {
        add_pod_type_details<void>();
        add_pod_type_details<signed char>();
        add_pod_type_details<unsigned char>();
        add_pod_type_details<char>();
        add_pod_type_details<short>();
        add_pod_type_details<unsigned short>();
        add_pod_type_details<int>();
        add_pod_type_details<unsigned int>();
        add_pod_type_details<long>();
        add_pod_type_details<unsigned long>();
        add_pod_type_details<int64_t>("int64_t");
        add_pod_type_details<uint64_t>("uint64_t");
        add_pod_type_details<float>();
        add_pod_type_details<double>();
        add_pod_type_details<long double>();
        add_pod_type_details<bool>();

        add_type_details<std::string>("std", "string");
        add_type_details<std::wstring>("std", "wstring");
    }

    static mge::singleton<type_dictionary> s_type_dictionary;

    type_details::type_details(const std::string&         automatic_name,
                               const std::type_index&     ti,
                               const mge::script::traits& t,
                               size_t                     size,
                               const char*                name)
        : m_automatic_name(automatic_name)
        , m_name(name == nullptr ? "" : name)
        , m_unique_name(ti.name())
        , m_type_index(ti)
        , m_traits(t)
        , m_size(size)
    {}

    const std::string& type_details::name() const
    {
        return m_name.empty() ? m_automatic_name : m_name;
    }

    const std::string& type_details::unique_name() const
    {
        return m_unique_name;
    }

    const module_details_weak_ref& type_details::module() const
    {
        return m_module;
    }

    const std::type_index& type_details::type_index() const
    {
        return m_type_index;
    }

    const mge::script::traits& type_details::traits() const { return m_traits; }

    size_t type_details::size() const { return m_size; }

    const type_details_ref& type_details::enclosing_type() const
    {
        return m_enclosing_type;
    }

    bool type_details::is_subtype() const
    {
        return m_enclosing_type.operator bool();
    }

    void type_details::set_module(const module_details_ref& m) { m_module = m; }

    void type_details::set_enclosing_type(const type_details_ref& t)
    {
        m_enclosing_type = t;
    }

    module_details_ref type_details::enclosing_module() const
    {
        auto ret = m_module.lock();
        if (!ret) {
            if (!m_enclosing_type) {
                MGE_THROW(mge::illegal_state)
                    << "Type '" << name() << "' has no module or parent type";
            }
            return m_enclosing_type->enclosing_module();
        }
        return ret;
    }

    type_details_ref type_details::get_first_match(const std::type_index& ti)
    {
        return s_type_dictionary->get_first_match(ti);
    }

    type_details_ref type_details::create(const std::type_index&     ti,
                                          const std::string&         name,
                                          const mge::script::traits& tr,
                                          size_t                     size)
    {
        auto result = s_type_dictionary->add(ti, name, tr, size);
        return result;
    }

    void type_details::apply(const type_details_ref& self, visitor& v)
    {
        if (self.get() != this) {
            MGE_THROW(mge::illegal_argument)
                << "Passed type details reference must be same as this";
        }
        v.start(self);
        v.finish(self);
    }

    enum_type_details::enum_type_details(const std::string&         name,
                                         const std::type_index&     ti,
                                         const mge::script::traits& t,
                                         size_t                     size,
                                         const char*                used_name)
        : type_details(name, ti, t, size, used_name)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Create enum '" << name << "'";
    }

    void enum_type_details::apply(const type_details_ref& self, visitor& v)
    {

        if (self.get() != this) {
            MGE_THROW(mge::illegal_argument)
                << "Passed type details reference must be same as this";
        }
        v.start(self);
        for (const auto& value : m_values) {
            v.enum_value(std::get<0>(value), std::get<1>(value));
        }
        v.finish(self);
    }

    void enum_type_details::enum_value(const std::string& name, int64_t value)
    {
        m_values.emplace_back(std::make_tuple(name, value));
    }

    class_type_details::class_type_details(const std::string&         name,
                                           const std::type_index&     ti,
                                           const mge::script::traits& t,
                                           size_t                     size,
                                           const char*                used_name)
        : type_details(name, ti, t, size, used_name)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Creating class '" << name << "'";
    }

    void class_type_details::apply(const type_details_ref& self, visitor& v)
    {
        if (self.get() != this) {
            MGE_THROW(mge::illegal_argument)
                << "Passed type details reference must be same as this";
        }
        v.start(self);

        for (const auto& [ti, details_vec] : m_types) {
            for (const auto& details : details_vec) {
                details->apply(details, v);
            }
        }

        for (const auto& f : m_fields) {
            v.field(f.name, f.type, f.getter, f.setter);
        }
        for (const auto& c : m_constructors) {
            v.constructor(c.signature, c.new_at, c.new_shared);
        }
        for (const auto& m : m_methods) {
            v.method(m.name, m.is_static, m.result_type, m.signature, m.invoke);
        }
        if (!m_destructor.empty()) {
            v.destructor(m_destructor.delete_ptr,
                         m_destructor.delete_shared_ptr);
        }
        v.finish(self);
    }

    void class_type_details::set_base(const type_details_ref& base_details)
    {
        for (const auto& b : m_bases) {
            if (b.get() == base_details.get()) {
                return;
            }
        }
        m_bases.push_back(base_details);
    }

    void
    class_type_details::set_destructor(const invoke_function& delete_ptr,
                                       const invoke_function& delete_shared_ptr)
    {
        m_destructor = destructor(delete_ptr, delete_shared_ptr);
    }

    void class_type_details::add_constructor(const signature&       s,
                                             const invoke_function& ctor,
                                             const invoke_function& make_shared)
    {
        m_constructors.emplace_back(s, ctor, make_shared);
    }

    void class_type_details::add_field(const std::string&      name,
                                       const type_details_ref& t,
                                       const invoke_function&  getter)
    {
        m_fields.emplace_back(name, t, getter);
    }

    void class_type_details::add_field(const std::string&      name,
                                       const type_details_ref& t,
                                       const invoke_function&  getter,
                                       const invoke_function&  setter)
    {
        m_fields.emplace_back(name, t, getter, setter);
    }

    void class_type_details::add_method(const std::string&     name,
                                        const std::type_index& return_type,
                                        const signature&       sgn,
                                        const invoke_function& invoke)
    {
        m_methods.emplace_back(name, return_type, sgn, invoke);
    }

    void
    class_type_details::add_static_method(const std::string&     name,
                                          const std::type_index& return_type,
                                          const signature&       sgn,
                                          const invoke_function& invoke)
    {
        m_methods.emplace_back(name, return_type, sgn, invoke, true);
    }

    void class_type_details::add_type(const type_details_ref& t)
    {
        m_types[t->type_index()].push_back(t);
        t->set_enclosing_type(shared_from_this());
    }

} // namespace mge::script