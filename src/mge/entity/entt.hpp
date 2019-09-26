#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#  pragma warning( disable : 4464 ) 
#  pragma warning( disable : 4800 ) 
#endif
#define ENTT_ID_TYPE std::uint64_t
#include <entt/entity/entity.hpp>
#include <entt/entt.hpp>
#ifdef MGE_COMPILER_MSVC
#  pragma warning( default : 4464 ) 
#  pragma warning( default : 4800 ) 
#endif
