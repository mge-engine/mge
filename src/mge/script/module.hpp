#pragma once
#include "mge/script/dllexport.hpp"

namespace mge::script {

    /**
     * @brief Module
     *
     * A module wraps a namespace or alike compound of objects. Modules, like
     * namespaces. are hierarchically organized.
     */
    class MGESCRIPT_EXPORT module
    {
    public:
        /**
         * Root module.
         */
        module();
        /**
         * @brief Construct a new module object
         *
         * @param identifier module identifie
         */
        module(const std::string& identifier);
        /**
         * @brief Destroy the module object
         */
        ~module();
        /**
         * @brief Copy constructor.
         */
        module(const module&) = default;
        /**
         * @brief Move constructor.
         */
        module(module&&) = default;
        /**
         * @brief Constructor.
         * @param details module details
         */
        module(module_details* details);

        /**
         * @brief Assignment.
         *
         * @return @c *this
         */
        module& operator=(const module&) = default;

        /**
         * @brief Move assignment.
         *
         * @return @c *this
         */
        module& operator=(module&&) = default;

        /**
         * @brief Add elements to the module.
         *
         * @tparam T        type of first element
         * @tparam Args     other element types
         * @param arg0      first element
         * @param args      other elements
         *
         * @return @c *this
         */
        template <typename T, typename... Args>
        module& operator()(T&& arg0, Args&&... args)
        {
            add_member(std::forward<T>(arg0));
            operator()(std::forward<Args>(args)...);
            return *this;
        }

        bool                is_root() const;
        mge::script::module parent() const;
        const std::string&  name() const;
        std::string         full_name() const;

        void apply(visitor& v);

        module_details& details();

    private:
        template <typename T> void add_member(T&& m)
        {
            static_assert(false, "Try to add unsupported type to module");
        }

        module_details* m_details;
    }

} // namespace mge::script