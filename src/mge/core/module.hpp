// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/noncopyable.hpp"
#include "mge/core/shared_library.hpp"

namespace mge {

    MGE_DECLARE_REF(module);

    /**
     * @brief A module.
     *
     * Modules are loaded from a configured number of directories, and
     * integrate by providing implementations of components.
     */
    class MGECORE_EXPORT module : public noncopyable
    {
    public:
        /**
         * @brief Load new module from path.
         *
         * @param path path of module shared library
         */
        module(const std::filesystem::path& path);

        /**
         * @brief Destructor.
         */
        ~module();

        /**
         * @brief Load all modules as configured-
         */
        static void load_all();

        /**
         * @brief Module name.
         *
         * @return name of module
         */
        std::string_view name() const;

        /**
         * @brief Loaded module library.
         *
         * @return reference to library
         */
        const shared_library_ref& library() const;

    private:
        shared_library_ref m_library;
        std::string        m_name;
    };

} // namespace mge