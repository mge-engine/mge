// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 4003)
#endif
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 4003)
#endif
