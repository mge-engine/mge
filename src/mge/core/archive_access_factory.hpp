#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/archive_access.hpp"
#include "mge/core/component.hpp"
#include "mge/core/file.hpp"
namespace mge {
    
    class MGE_CORE_EXPORT archive_access_factory
        : public component<archive_access_factory>
    {
    public:
        archive_access_factory() = default;
        virtual ~archive_access_factory() = default;

        virtual archive_access_ref create_archive_access(const file& f, open_mode m) = 0;
    };
}