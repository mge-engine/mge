// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_formatter.hpp"
#include <optional>

namespace mge {
    class MGECORE_EXPORT simple_trace_formatter : public trace_formatter
    {
    public:
        simple_trace_formatter();
        ~simple_trace_formatter() = default;
        void format(std::ostream &stream, const trace_record &r) override;

    private:
        std::optional<clock::time_point> m_first;
    };
} // namespace mge