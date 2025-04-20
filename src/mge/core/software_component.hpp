// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/semantic_version.hpp"
#include <string>

namespace mge {

    MGE_DECLARE_REF(software_component);

    /**
     * @brief Software component interface.
     *
     * This interface is used to identify a software component. It provides
     * the version and build information of the component.
     */
    class MGECORE_EXPORT software_component
        : public component<software_component>
    {
    public:
        software_component() = default;
        software_component(const software_component&) = delete;
        software_component(software_component&&) = delete;
        software_component& operator=(const software_component&) = delete;
        software_component& operator=(software_component&&) = delete;
        virtual ~software_component() = default;

        /**
         * @brief Get the name of the software component.
         *
         * @return name of the software component
         */
        virtual std::string name() const = 0;
        /**
         * @brief Get the version of the software component.
         *
         * @return version of the software component
         */
        virtual semantic_version version() const = 0;

        /**
         * @brief Get the build information of the software component.
         *
         * @return build information of the software component, like git hash
         */
        virtual std::string build() const = 0;

        /**
         * @brief Get a reference to the software component of the mge itself.
         * @return mge software component
         */
        static software_component_ref mge();
    };

#define MGE_DEFINE_SOFTWARE_COMPONENT(NAME, VERSION, BUILD)                    \
    class NAME##_software_component : public software_component                \
    {                                                                          \
    public:                                                                    \
        NAME##_software_component() = default;                                 \
        NAME##_software_component(const NAME##_software_component&) = delete;  \
        NAME##_software_component(NAME##_software_component&&) = delete;       \
        NAME##_software_component&                                             \
        operator=(const NAME##_software_component&) = delete;                  \
        NAME##_software_component&                                             \
        operator=(NAME##_software_component&&) = delete;                       \
        ~NAME##_software_component() override = default;                       \
                                                                               \
        std::string name() const override                                      \
        {                                                                      \
            return #NAME;                                                      \
        }                                                                      \
        semantic_version version() const override                              \
        {                                                                      \
            return VERSION;                                                    \
        }                                                                      \
        std::string build() const override                                     \
        {                                                                      \
            return BUILD;                                                      \
        }                                                                      \
    };                                                                         \
    MGE_REGISTER_IMPLEMENTATION(NAME##_software_component, software_component)

} // namespace mge