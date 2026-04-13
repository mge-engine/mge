// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/markdown_document.hpp"

#include <memory_resource>
#include <string>

namespace mge {

    /**
     * @brief Component interface for providing sections in crash dumps.
     *
     * Implementations contribute a named section with diagnostic
     * information to the markdown crash dump document. Each provider
     * is discovered and instantiated via the component system during
     * dump generation.
     *
     * Implementations should derive from this class, register via
     * @c MGE_REGISTER_IMPLEMENTATION, and override @c section_name
     * and @c dump_info.
     */
    class MGECORE_EXPORT dump_info_provider
        : public component<dump_info_provider>
    {
    protected:
        /** @brief Default constructor. */
        dump_info_provider() = default;

        /**
         * @brief Construct with a specific memory resource.
         * @param resource memory resource for PMR allocations
         */
        dump_info_provider(std::pmr::memory_resource* resource)
            : m_resource(resource)
        {}

    public:
        virtual ~dump_info_provider() = default;

        /**
         * @brief Section heading for this provider's dump output.
         * @return section name string allocated with @c m_resource
         */
        virtual std::pmr::string section_name() const = 0;

        /**
         * @brief Append diagnostic information to the dump document.
         * @param doc markdown document to write into
         */
        virtual void dump_info(markdown_document& doc) const = 0;

    protected:
        /** @brief Memory resource for PMR allocations. */
        std::pmr::memory_resource* m_resource =
            std::pmr::get_default_resource();
    };

} // namespace mge
