#pragma once
namespace mge::script {
    class MGESCRIPT_EXPORT visitor
    {
    protected:
        visitor() = default;

    public:
        virtual ~visitor() = default;
    };
} // namespace mge::script