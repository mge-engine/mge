#pragma once
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace mge::reflect {

    template <typename T> class type_details
    {
    public:
        type_details() {}

        std::string_view name() const;
    };

#define MGE_DEFINE_POD_TYPE_DETAILS(type)                                      \
    template <> class type_details<type>                                       \
    {                                                                          \
    public:                                                                    \
        type_details() {}                                                      \
        std::string_view name() const { return std::string_view(#type); }      \
    }

    MGE_DEFINE_POD_TYPE_DETAILS(char);
    MGE_DEFINE_POD_TYPE_DETAILS(signed char);
    MGE_DEFINE_POD_TYPE_DETAILS(unsigned char);
    MGE_DEFINE_POD_TYPE_DETAILS(short);
    MGE_DEFINE_POD_TYPE_DETAILS(unsigned short);
    MGE_DEFINE_POD_TYPE_DETAILS(int);
    MGE_DEFINE_POD_TYPE_DETAILS(unsigned int);
    MGE_DEFINE_POD_TYPE_DETAILS(long);
    MGE_DEFINE_POD_TYPE_DETAILS(unsigned long);
    MGE_DEFINE_POD_TYPE_DETAILS(void);
    MGE_DEFINE_POD_TYPE_DETAILS(bool);
    MGE_DEFINE_POD_TYPE_DETAILS(float);
    MGE_DEFINE_POD_TYPE_DETAILS(double);
    MGE_DEFINE_POD_TYPE_DETAILS(long double);

#undef MGE_DEFINE_POD_DETAILS
} // namespace mge::reflect