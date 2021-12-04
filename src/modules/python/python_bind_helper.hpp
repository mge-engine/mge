// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/visitor.hpp"

namespace mge::python {

    class python_context;

    class python_bind_helper : public mge::script::visitor
    {
    public:
        python_bind_helper(python_context& context);
        virtual ~python_bind_helper();

        void begin(const mge::script::module_details& m) override;
        void end(const mge::script::module_details& m) override;

        void begin(const mge::script::type_details& t) override;
        void end(const mge::script::type_details& t) override;

        void begin(const mge::script::function_details& t) override;
        void end(const mge::script::function_details& t) override;

    private:
        python_context& m_context;
    };

} // namespace mge::python