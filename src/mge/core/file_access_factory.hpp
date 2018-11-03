#pragma once
#include "mge/core/component.hpp"
#include "mge/core/file_access.hpp"

namespace mge {
    // @class file_access_factor_ref
    // @brief Shared pointer to @c file_access_factory.
    MGE_DECLARE_REF(file_access_factory);

    /**
     * Factor class for file access.
     */
    class MGE_CORE_EXPORT file_access_factory
        : public component<file_access_factory>
    {
    public:
        /**
         * Constructor.
         */
        file_access_factory() = default;

        /**
         * Destructor.
         */
        virtual ~file_access_factory() = default;

        /**
         * Retrieve file access object.
         * @param path path
         * @return file access object
         */
        virtual file_access_ref create_file_access(const std::string& path) = 0;

        /**
         * Retrieve file access object.
         * @param path path
         * @param name file name in path
         * @return file access object
         */
        virtual file_access_ref create_file_access(const std::string& path,
                                                   const std::string& name) = 0;
    };
}

