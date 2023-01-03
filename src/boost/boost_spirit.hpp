// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#    pragma warning(disable : 4459)
#endif
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_auto.hpp>
#include <boost/spirit/include/qi_optional.hpp>
#include <boost/spirit/include/qi_sequence.hpp>