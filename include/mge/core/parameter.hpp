// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/make_string_view.hpp"
#include <boost/lexical_cast.hpp>

#include <any>
#include <sstream>
#include <string_view>

namespace mge {

    class MGECORE_EXPORT basic_parameter
    {
    public:
        basic_parameter(std::string_view section, std::string_view name,
                        std::string_view description);
        virtual ~basic_parameter();

        std::string_view section() const noexcept;
        std::string_view name() const noexcept;
        std::string_view description() const noexcept;

        virtual bool        has_value() const                     = 0;
        virtual void        from_string(const std::string &value) = 0;
        virtual std::string to_string() const                     = 0;

    private:
        std::string_view m_section;
        std::string_view m_name;
        std::string_view m_description;
    };

    template <typename T> class parameter : public basic_parameter
    {
    public:
        template <size_t SECTION_N, size_t NAME_N, size_t DESCRIPTION_N>
        parameter(const char (&section)[SECTION_N], const char (&name)[NAME_N],
                  const char (&description)[DESCRIPTION_N])
            : basic_parameter(make_string_view(section), make_string_view(name),
                              make_string_view(description))
        {}

        parameter(std::string_view section, std::string_view name,
                  std::string_view description)
            : basic_parameter(section, name, description)
        {}

        virtual ~parameter() = default;

        bool has_value() const override { return m_value.has_value(); }

        typename T get() const { return std::any_cast<T>(m_value); }

        void from_string(const std::string &value) override
        {
            T val   = boost::lexical_cast<T>(value);
            m_value = val;
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << get();
            return ss.str();
        }

    private:
        std::any m_value;
    };

#define MGE_DEFINE_PARAMETER(TYPE, SECTION, NAME, DESCRIPTION)                 \
    ::mge::parameter<TYPE> p_##SECTION##_##NAME(#SECTION, #NAME, DESCRIPTION);

#define MGE_PARAMETER(SECTION, NAME) p_##SECTION##_##NAME
} // namespace mge