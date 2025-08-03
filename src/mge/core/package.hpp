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

    MGE_DECLARE_REF(package);

    /**
     * @brief Software package interface.
     *
     * This interface is used to identify a software package. It provides
     * the version and build information of the package.
     */
    class MGECORE_EXPORT package : public component<package>
    {
    public:
        package() = default;
        package(const package&) = delete;
        package(package&&) = delete;
        package& operator=(const package&) = delete;
        package& operator=(package&&) = delete;
        virtual ~package() = default;

        /**
         * @brief Get the name of the package.
         *
         * @return name of the package
         */
        virtual std::string name() const = 0;
        /**
         * @brief Get the version of the package.
         *
         * @return version of the package
         */
        virtual semantic_version version() const = 0;

        /**
         * @brief Get the build information of the package.
         *
         * @return build information of the package, like git hash
         */
        virtual std::string build() const = 0;

        /**
         * @brief Get a reference to a package by name.
         *
         * @param name Name of the package
         * @return Reference to the package
         */
        static package_ref get(std::string_view name);
    };

#define MGE_DEFINE_PACKAGE(NAME, VERSION, BUILD)                               \
    class NAME##_package : public package                                      \
    {                                                                          \
    public:                                                                    \
        NAME##_package() = default;                                            \
        NAME##_package(const NAME##_package&) = delete;                        \
        NAME##_package(NAME##_package&&) = delete;                             \
        NAME##_package& operator=(const NAME##_package&) = delete;             \
        NAME##_package& operator=(NAME##_package&&) = delete;                  \
        ~NAME##_package() override = default;                                  \
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
    MGE_REGISTER_IMPLEMENTATION(NAME##_package, package)

} // namespace mge
