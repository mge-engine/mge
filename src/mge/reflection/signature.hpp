// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/type_identifier.hpp"

#include <vector>

namespace mge::reflection {

    class signature
    {
    public:
        signature()
            : m_return_type(make_type_identifier<void>())
        {}

        signature(const type_identifier& return_type)
            : m_return_type(return_type)
        {}

        signature(const type_identifier&              return_type,
                  const std::vector<type_identifier>& parameter_types)
            : m_return_type(return_type)
            , m_parameter_types(parameter_types)
        {}

        const type_identifier& return_type() const
        {
            return m_return_type;
        }

        const std::vector<type_identifier>& parameter_types() const
        {
            return m_parameter_types;
        }

        bool operator==(const signature& other) const
        {
            return m_return_type == other.m_return_type &&
                   m_parameter_types == other.m_parameter_types;
        }

        bool operator!=(const signature& other) const
        {
            return !(*this == other);
        }

    private:
        type_identifier              m_return_type;
        std::vector<type_identifier> m_parameter_types;
    };
} // namespace mge::reflection

namespace std {
    template <> struct hash<mge::reflection::signature>
    {
        size_t operator()(const mge::reflection::signature& sig) const noexcept
        {
            size_t result =
                hash<mge::reflection::type_identifier>{}(sig.return_type());
            for (const auto& param : sig.parameter_types()) {
                result ^= hash<mge::reflection::type_identifier>{}(param) +
                          0x9e3779b9 + (result << 6) + (result >> 2);
            }
            return result;
        }
    };
} // namespace std

namespace fmt {
    template <>
    struct formatter<mge::reflection::signature>
        : public formatter<std::string_view>
    {
        template <typename FormatContext>
        auto format(const mge::reflection::signature& sig,
                    FormatContext&                    ctx) const
        {
            std::string result;
            result += fmt::format("{}", sig.return_type());
            result += "(";
            for (size_t i = 0; i < sig.parameter_types().size(); ++i) {
                if (i > 0) {
                    result += ", ";
                }
                result += fmt::format("{}", sig.parameter_types()[i]);
            }
            result += ")";
            return formatter<std::string_view>::format(result, ctx);
        }
    };
} // namespace fmt
