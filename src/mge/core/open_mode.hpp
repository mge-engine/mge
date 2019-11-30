#pragma once
#include "mge/core/dllexport.hpp"
#include <iosfwd>
namespace mge {

    /**
     * @brief Open mode for files or archives.
     * 
     */
    enum class open_mode : uint8_t
    {
        READ,       //!< Opened read only.
        WRITE,      //!< Opened write only.
        READ_WRITE  //!< Opened for read and write.
    };

    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os, 
                                              const open_mode& m);
}