#pragma once
namespace mge::script {

    /**
     * @brief Visit a module.
     */
    class MGESCRIPT_EXPORT visitor
    {
    protected:
        visitor() = default;

    public:
        virtual ~visitor() = default;
    };

} // namespace mge::script