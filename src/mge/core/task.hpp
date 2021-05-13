#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/noncopyable.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#    pragma warning(disable : 5220)
#endif
#include <future>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5220)
#    pragma warning(default : 5204)
#endif
#include <functional>

namespace mge {

    MGE_DECLARE_REF(task);

    class MGECORE_EXPORT task : noncopyable
    {
    public:
        explicit task(const std::function<void()> &f);
        explicit task(std::function<void()> &&f);
        ~task();

        void run();
        void wait();

    private:
        std::function<void()> m_function;
        std::promise<void>    m_result;
    };

} // namespace mge