// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5243)
#endif
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5243)
#endif
