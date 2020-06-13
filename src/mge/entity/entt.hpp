// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 4464)
#    pragma warning(disable : 4800)
#    pragma warning(disable : 4458)
#    pragma warning(disable : 4293)
#endif
#ifdef min
#    undef min
#endif
#define ENTT_ID_TYPE std::uint64_t
#include <entt/entity/entity.hpp>
#include <entt/entt.hpp>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 4464)
#    pragma warning(default : 4800)
#    pragma warning(default : 4458)
#    pragma warning(default : 4293)
#endif
