#pragma once
#include <type_traits>

#define MGE_SFINAE_CHECK(X) typename std::enable_if_t<X>* = nullptr